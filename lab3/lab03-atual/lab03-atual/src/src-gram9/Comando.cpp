#include "Comando.hpp"
#include "ComandoArrayAtribuicao.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoIf.hpp"
#include "ComandoLista.hpp"
#include "ComandoRetorno.hpp"
#include "ComandoWhile.hpp"
#include "ID.hpp"
#include "Variavel.hpp"
#include <iostream>
#include <utility>
#include "../debug-util.hpp"

static void coleta_elif(No_arv_parse* no, vector<pair<Expressao*, vector<Comando*> > > &itens);

static vector<Comando*> monta_elif(No_arv_parse* no, const vector<Comando*> &bloco_else_final) {
  vector<pair<Expressao*, vector<Comando*> > > itens;
  coleta_elif(no, itens);

  vector<Comando*> bloco_else = bloco_else_final;
  for (int i = (int)itens.size() - 1; i >= 0; --i) {
    ComandoIf* atual = new ComandoIf();
    atual->condicao = itens[i].first;
    atual->bloco_then = itens[i].second;
    atual->bloco_else = bloco_else;
    bloco_else.clear();
    bloco_else.push_back(atual);
  }
  return bloco_else;
}

static void coleta_elif(No_arv_parse* no, vector<pair<Expressao*, vector<Comando*> > > &itens) {
  if (no == NULL) return;
  if (no->regra == 24) {
    // ElifChain -> ElifChain KW_ELIF Expr COLON Block
    coleta_elif(no->filhos[0], itens);
    itens.push_back(make_pair(Expressao::extrai_expressao(no->filhos[2]),
                              Comando::extrai_lista_comandos(no->filhos[4])));
  } else if (no->regra == 25) {
    // ElifChain -> KW_ELIF Expr COLON Block
    itens.push_back(make_pair(Expressao::extrai_expressao(no->filhos[1]),
                              Comando::extrai_lista_comandos(no->filhos[3])));
  }
}

vector<Comando*> Comando::extrai_lista_comandos(No_arv_parse* no) {
  if (no == NULL) return vector<Comando*>();
  if (no->simb == "Block") {
    return extrai_lista_comandos(no->filhos[2]);
  }
  if (no->simb == "StmtList") {
    vector<Comando*> res;
    if (no->filhos.size() == 1) {
      Comando* c = extrai_comando(no->filhos[0]);
      if (c != NULL) res.push_back(c);
      return res;
    }
    vector<Comando*> anterior = extrai_lista_comandos(no->filhos[0]);
    res.insert(res.end(), anterior.begin(), anterior.end());
    Comando* c = extrai_comando(no->filhos[1]);
    if (c != NULL) res.push_back(c);
    return res;
  }

  // LC. 6) LC ->  7) LC -> C LC
  if (no->regra == 6) return vector<Comando*>();
  vector<Comando*> res;
  res.push_back(extrai_comando(no->filhos[0]));
  vector<Comando*> restante = extrai_lista_comandos(no->filhos[1]);
  res.insert(res.end(), restante.begin(), restante.end());
  return res;
}

Comando* Comando::extrai_comando(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "Stmt") return extrai_comando(no->filhos[0]);
  if (no->simb == "VarDecl") {
    ComandoDeclaracao* res = new ComandoDeclaracao();
    res->variavel = Variavel::extrai_variavel_P(no);
    if (no->filhos.size() >= 6 && no->filhos[4]->simb == "OP_ASSIGN") {
      res->inicializador = Expressao::extrai_expressao(no->filhos[5]);
    }
    return res;
  }
  if (no->simb == "AssignStmt") {
    ComandoAtribuicao* res = new ComandoAtribuicao();
    res->esquerda = ID::extrai_ID(no->filhos[0]);
    res->direita = Expressao::extrai_expressao(no->filhos[2]);
    return res;
  }
  if (no->simb == "ArrAssignStmt") {
    ComandoArrayAtribuicao* res = new ComandoArrayAtribuicao();
    res->nome = ID::extrai_ID(no->filhos[0]);
    res->indice = Expressao::extrai_expressao(no->filhos[2]);
    res->direita = Expressao::extrai_expressao(no->filhos[5]);
    return res;
  }
  if (no->simb == "IfStmt") {
    ComandoIf* res = new ComandoIf();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->bloco_then = extrai_lista_comandos(no->filhos[3]);
    if (no->regra == 21) {
      res->bloco_else = extrai_lista_comandos(no->filhos[6]);
    } else if (no->regra == 22) {
      res->bloco_else = monta_elif(no->filhos[4], vector<Comando*>());
    } else if (no->regra == 23) {
      vector<Comando*> bloco_else_final = extrai_lista_comandos(no->filhos[7]);
      res->bloco_else = monta_elif(no->filhos[4], bloco_else_final);
    }
    return res;
  }
  if (no->simb == "WhileStmt") {
    ComandoWhile* res = new ComandoWhile();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->bloco = extrai_lista_comandos(no->filhos[3]);
    return res;
  }
  if (no->simb == "ReturnStmt") {
    ComandoRetorno* res = new ComandoRetorno();
    res->expressao = Expressao::extrai_expressao(no->filhos[1]);
    return res;
  }

  // C. 9) C -> P ;  10) C -> ID = Ea ;  11) C -> { LC }  12) C -> RETURN Ea ;
  if (no->regra == 9) {
    ComandoDeclaracao* res9 = new ComandoDeclaracao();
    res9->variavel = Variavel::extrai_variavel_P(no->filhos[0]);
    return res9;
  } else if (no->regra == 10) {
    ComandoAtribuicao* res10 = new ComandoAtribuicao();
    res10->esquerda = ID::extrai_ID(no->filhos[0]);
    res10->direita = Expressao::extrai_expressao(no->filhos[2]);
    return res10;
  } else if (no->regra == 11) {
    ComandoLista* res11 = new ComandoLista();
    res11->lista_comandos = extrai_lista_comandos(no->filhos[1]);
    return res11;
  } else if (no->regra == 12) {
    ComandoRetorno* res12 = new ComandoRetorno();
    res12->expressao = Expressao::extrai_expressao(no->filhos[1]);
    return res12;
  }else {
    return NULL;
  }		      
}

void Comando::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Comando generico"<< endl;
}
