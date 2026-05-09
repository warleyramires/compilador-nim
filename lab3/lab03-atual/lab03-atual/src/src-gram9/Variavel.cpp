#include "Variavel.hpp"
#include <iostream>
#include <stdlib.h>
#include "../debug-util.hpp"

using namespace std;

vector<Variavel*> Variavel::extrai_lista_nao_vazia_parametros(No_arv_parse* no) {
  if (no->simb == "ParamList") {
    vector<Variavel*> res;
    if (no->regra == 62) {
      vector<Variavel*> anterior = extrai_lista_nao_vazia_parametros(no->filhos[0]);
      res.insert(res.end(), anterior.begin(), anterior.end());
      res.push_back(extrai_variavel_P(no->filhos[2]));
      return res;
    }
    res.push_back(extrai_variavel_P(no->filhos[0]));
    return res;
  }

  // LPx. 4) LPx -> P, 5) LPx -> P , LPx
  vector<Variavel*> res;
  res.push_back(extrai_variavel_P(no->filhos[0]));
  if (no->regra == 4) return res;
  // Tem que ser regra 5. LPx-> P , LPx
  vector<Variavel*> restante = extrai_lista_nao_vazia_parametros(no->filhos[2]);
  res.insert(res.end(), restante.begin(), restante.end());
  return res;
}

vector<Variavel *> Variavel::extrai_lista_parametros(No_arv_parse* no) {
  if (no == NULL) return vector<Variavel*>();
  if (no->simb == "ParamList") return extrai_lista_nao_vazia_parametros(no);
  // LP
  if (no->regra == 2) return vector<Variavel*>();
  // Tem que ser regra 3, com unico filho.
  return extrai_lista_nao_vazia_parametros(no->filhos[0]);
}

Variavel* Variavel::extrai_variavel_P(No_arv_parse* no) {
  Variavel* res = new Variavel();
  res->eh_array = false;
  res->tamanho_array = 0;
  if (no->simb == "Param") {
    // Param -> ID COLON Type
    res->nome = ID::extrai_ID(no->filhos[0]);
    res->tipo = Tipo::extrai_Tipo(no->filhos[2]);
    return res;
  }
  if (no->simb == "VarDecl") {
    if (no->regra == 17) {
      // VarDecl -> KW_VAR ID COLON KW_ARRAY LEFT_BRACKET NUM_INTEGER COMMA Type RIGHT_BRACKET NEWLINE
      res->nome = ID::extrai_ID(no->filhos[1]);
      res->eh_array = true;
      res->tamanho_array = atoi(no->filhos[5]->dado_extra.c_str());
      res->tipo = Tipo::extrai_Tipo(no->filhos[7]);
      return res;
    }
    // VarDecl -> KW_VAR ID COLON Type ...
    res->nome = ID::extrai_ID(no->filhos[1]);
    res->tipo = Tipo::extrai_Tipo(no->filhos[3]);
    return res;
  }
  // 8) P -> Tipo ID
  res->tipo = Tipo::extrai_Tipo(no->filhos[0]);
  res->nome = ID::extrai_ID(no->filhos[1]);
  return res;
}

void Variavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << nome->nome << ":" << (eh_array ? "ARRAY " : "")
       << (tipo == NULL ? "TIPO_INVALIDO" : tipo->nome()) << "] Variavel Declarada" << endl;
}
