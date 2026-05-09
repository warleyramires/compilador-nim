#include "Funcao.hpp"
#include "../debug-util.hpp"
#include <iostream>

Funcao::Funcao() {
}

static No_arv_parse* primeira_funcdecl(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "FuncDecl") return no;
  for (int i = 0; i < no->filhos.size(); ++i) {
    No_arv_parse* achado = primeira_funcdecl(no->filhos[i]);
    if (achado != NULL) return achado;
  }
  return NULL;
}

static void coleta_funcdecls(No_arv_parse* no, vector<No_arv_parse*> &funcdecls) {
  if (no == NULL) return;
  if (no->simb == "FuncDecl") {
    funcdecls.push_back(no);
    return;
  }
  for (int i = 0; i < no->filhos.size(); ++i) {
    coleta_funcdecls(no->filhos[i], funcdecls);
  }
}

static Funcao* extrai_funcdecl_nim(No_arv_parse* funcdecl) {
  Funcao* res = new Funcao();
  // FuncDecl -> KW_PROC ID ( ParamList ) : Type = Block
  // FuncDecl -> KW_PROC ID ( ) : Type = Block
  res->nome_funcao = ID::extrai_ID(funcdecl->filhos[1]);
  if (funcdecl->filhos[3]->simb == "ParamList") {
    res->parametros = Variavel::extrai_lista_parametros(funcdecl->filhos[3]);
    res->tipo_retorno = Tipo::extrai_Tipo(funcdecl->filhos[6]);
    res->comandos = Comando::extrai_lista_comandos(funcdecl->filhos[8]);
  } else {
    res->tipo_retorno = Tipo::extrai_Tipo(funcdecl->filhos[5]);
    res->comandos = Comando::extrai_lista_comandos(funcdecl->filhos[7]);
  }
  return res;
}

vector<Funcao*> Funcao::extrai_funcoes(No_arv_parse *no) {
  vector<Funcao*> funcoes;
  if (no == NULL) return funcoes;
  if (no->simb != "S") {
    vector<No_arv_parse*> funcdecls;
    coleta_funcdecls(no, funcdecls);
    for (int i = 0; i < funcdecls.size(); ++i) {
      funcoes.push_back(extrai_funcdecl_nim(funcdecls[i]));
    }
    return funcoes;
  }
  funcoes.push_back(extrai_funcao(no));
  return funcoes;
}

Funcao* Funcao::extrai_funcao(No_arv_parse *no) {
  if (no != NULL && no->simb != "S") {
    No_arv_parse* funcdecl = primeira_funcdecl(no);
    if (funcdecl != NULL) {
      return extrai_funcdecl_nim(funcdecl);
    }
  }

  if (no->regra != 1)   return NULL;
  // Dependente da gramatica. Regra 1 = Funcao.
  // S -> ID ID ( LP ) { LC }
  Funcao* res = new Funcao();
  res->tipo_retorno = Tipo::extrai_Tipo(no->filhos[0]);
  res->nome_funcao = ID::extrai_ID(no->filhos[1]);
  res->parametros = Variavel::extrai_lista_parametros(no->filhos[3]);
  res->comandos = Comando::extrai_lista_comandos(no->filhos[6]);
  return res;
}

void debug_comandos(const vector<Comando*> &coms, int tab) {
  for (int ic = 0; ic < coms.size(); ++ic) {
    coms[ic]->debug_com_tab(tab+1);
  }  
}

void Funcao::debug() {
  if (tipo_retorno == NULL) cerr<< "TR NULL"<< endl;
  if (nome_funcao == NULL) cerr<< "NF NULL"<< endl;
  cerr << "Funcao:[retorno=" << tipo_retorno->nome() << "][nome=" << nome_funcao->nome << "]" << endl;
  cerr << "      (Param:(";
  fflush(stderr);
  for (int i_par = 0; i_par < parametros.size(); ++i_par) {
    cerr << ((parametros[i_par])->tipo == NULL ? "TIPO_INVALIDO" : (parametros[i_par])->tipo->nome()) << " " <<
      (parametros[i_par])->nome->nome << ", ";
  }
  cerr << ") { " << endl;
  debug_comandos(comandos, 1);
  cerr << "}" <<  endl;
}
