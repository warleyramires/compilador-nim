#include "Expressao.hpp"
#include "ExpressaoDivisao.hpp"
#include "ExpressaoIgualdade.hpp"
#include "ExpressaoIndice.hpp"
#include "ExpressaoMenor.hpp"
#include "ExpressaoMod.hpp"
#include "ExpressaoMultiplicacao.hpp"
#include "ExpressaoNegacao.hpp"
#include "ExpressaoSoma.hpp"
#include "ExpressaoSubtracao.hpp"
#include "ExpressaoValor.hpp"
#include "ExpressaoVariavel.hpp"
#include "ExpressaoChamada.hpp"
#include "ExpressaoBinaria.hpp"
#include "ExpressaoUnaria.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

static bool eh_literal(No_arv_parse* no) {
  if (no == NULL) return false;
  return no->simb == "DEC_VAL" || no->simb == "FLOAT_VAL" || no->simb == "BOOL_VAL" ||
    no->simb == "NUM_INTEGER" || no->simb == "NUM_FLOAT" ||
    no->simb == "KW_TRUE" || no->simb == "KW_FALSE";
}

static bool eh_no_expressao_nim(No_arv_parse* no) {
  if (no == NULL) return false;
  return no->simb == "Expr" || no->simb == "AndExpr" || no->simb == "CmpExpr" ||
    no->simb == "AddExpr" || no->simb == "MulExpr" || no->simb == "UnaryExpr" ||
    no->simb == "PostfixExpr" || no->simb == "Primary";
}

static vector<Expressao*> extrai_arglist(No_arv_parse* no);

static vector<Expressao*> extrai_arglist(No_arv_parse* no) {
  vector<Expressao*> res;
  if (no == NULL || no->simb != "ArgList") return res;
  if (no->filhos.size() == 1) {
    res.push_back(Expressao::extrai_expressao(no->filhos[0]));
    return res;
  }
  vector<Expressao*> anteriores = extrai_arglist(no->filhos[0]);
  res.insert(res.end(), anteriores.begin(), anteriores.end());
  res.push_back(Expressao::extrai_expressao(no->filhos[2]));
  return res;
}

static Expressao* extrai_expressao_nim(No_arv_parse* no) {
  if (no == NULL) return NULL;

  if (no->simb == "ID") {
    ExpressaoVariavel* res = new ExpressaoVariavel();
    res->nome = ID::extrai_ID(no);
    return res;
  }

  if (eh_literal(no)) {
    ExpressaoValor* res = new ExpressaoValor();
    res->valor = ValorLiteral::extrai_valor_literal(no);
    return res;
  }

  if (!eh_no_expressao_nim(no)) return NULL;

  if (no->simb == "PostfixExpr" && no->filhos.size() == 4) {
    ExpressaoIndice* res = new ExpressaoIndice();
    res->base = extrai_expressao_nim(no->filhos[0]);
    res->indice = extrai_expressao_nim(no->filhos[2]);
    return res;
  }

  if (no->simb == "Primary") {
    if (no->filhos.size() == 1) return extrai_expressao_nim(no->filhos[0]);
    if (no->filhos.size() == 3 && no->filhos[0]->simb == "LEFT_PARENTHESIS") {
      return extrai_expressao_nim(no->filhos[1]);
    }
    if (no->filhos.size() >= 3 && no->filhos[0]->simb == "ID" && no->filhos[1]->simb == "LEFT_PARENTHESIS") {
      ExpressaoChamada* res = new ExpressaoChamada();
      res->nome = ID::extrai_ID(no->filhos[0]);
      if (no->filhos.size() == 4) res->argumentos = extrai_arglist(no->filhos[2]);
      return res;
    }
  }

  if (no->filhos.size() == 1) return extrai_expressao_nim(no->filhos[0]);

  if (no->filhos.size() == 2) {
    ExpressaoUnaria* res = new ExpressaoUnaria(no->filhos[0]->dado_extra.empty() ? no->filhos[0]->simb : no->filhos[0]->dado_extra);
    res->expressao = extrai_expressao_nim(no->filhos[1]);
    return res;
  }

  if (no->filhos.size() == 3) {
    ExpressaoBinaria* res = new ExpressaoBinaria(no->filhos[1]->dado_extra.empty() ? no->filhos[1]->simb : no->filhos[1]->dado_extra);
    res->esquerda = extrai_expressao_nim(no->filhos[0]);
    res->direita = extrai_expressao_nim(no->filhos[2]);
    return res;
  }

  return NULL;
}

// Tratei Ea, Eb, Ec, E, T e F na mesma funcao.
Expressao* Expressao::extrai_expressao(No_arv_parse* no) {
  if (eh_no_expressao_nim(no) || (no != NULL && (no->simb == "ID" || eh_literal(no)))) {
    return extrai_expressao_nim(no);
  }
  // Ea,Eb,Ec,E,T,F. 16) a 33)
  switch(no->regra) {
  case 16: { // 16) Ea -> ! Ea
    ExpressaoNegacao* res16 = new ExpressaoNegacao();
    res16->expressao = extrai_expressao(no->filhos[1]);
    return res16;
  }
  case 17: { // 17) Ea -> Eb
    return extrai_expressao(no->filhos[0]);
  }
  case 18: { // 18) Eb -> Ec == Ec
    ExpressaoIgualdade* res18 = new ExpressaoIgualdade();
    res18->esquerda = extrai_expressao(no->filhos[0]);
    res18->direita = extrai_expressao(no->filhos[2]);
    return res18;
  }
  case 19: { // 19) Eb -> Ec
    return extrai_expressao(no->filhos[0]);
  }
  case 20: { // 20) Ec -> E < E
    ExpressaoMenor* res20 = new ExpressaoMenor();
    res20->esquerda = extrai_expressao(no->filhos[0]);
    res20->direita = extrai_expressao(no->filhos[2]);
    return res20;
  }
  case 21: { // 21) Ec -> E
    return extrai_expressao(no->filhos[0]);
  }
  case 22: { // 22) E -> E + T
    ExpressaoSoma* res22 = new ExpressaoSoma();
    res22->esquerda = extrai_expressao(no->filhos[0]);
    res22->direita = extrai_expressao(no->filhos[2]);
    return res22;
  }
  case 23: { // 23) E -> E - T
    ExpressaoSubtracao* res23 = new ExpressaoSubtracao();
    res23->esquerda = extrai_expressao(no->filhos[0]);
    res23->direita = extrai_expressao(no->filhos[2]);
    return res23;
  }
  case 24: { // 24) E -> T
    return extrai_expressao(no->filhos[0]);
  }
  case 25: { // 25) T -> T * F
    ExpressaoMultiplicacao* res25 = new ExpressaoMultiplicacao();
    res25->esquerda = extrai_expressao(no->filhos[0]);
    res25->direita = extrai_expressao(no->filhos[2]);
    return res25;
  }
  case 26: {// 26) T -> T / F
    ExpressaoDivisao* res26 = new ExpressaoDivisao();
    res26->esquerda = extrai_expressao(no->filhos[0]);
    res26->direita = extrai_expressao(no->filhos[2]);
    return res26;
  }
  case 27: { // 27) T -> T % F
    ExpressaoMod* res27 = new ExpressaoMod();
    res27->esquerda = extrai_expressao(no->filhos[0]);
    res27->direita = extrai_expressao(no->filhos[2]);
    return res27;
  }
  case 28: { // 28) T -> F
    return extrai_expressao(no->filhos[0]);
  }
  case 29: { // 29) F -> ( Ea )
    return extrai_expressao(no->filhos[1]);
  }
  case 30: { // 30) F -> ID
    ExpressaoVariavel* res30 = new ExpressaoVariavel();
    res30->nome = ID::extrai_ID(no->filhos[0]);
    return res30;
  }
  case 31: // 31) F -> DEC_VAL
  case 32: // 32) F -> FLOAT_VAL
  case 33: { // 33) F -> BOOL_VAL
    ExpressaoValor* res = new ExpressaoValor();
    res->valor = ValorLiteral::extrai_valor_literal(no->filhos[0]);
    return res;
  }
  default: {
    return NULL;
  }
  }
}

void Expressao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Expressao generica" << endl;
}
