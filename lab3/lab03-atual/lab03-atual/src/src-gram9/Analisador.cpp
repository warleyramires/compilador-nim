#include "Analisador.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoArrayAtribuicao.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoIf.hpp"
#include "ComandoLista.hpp"
#include "ComandoRetorno.hpp"
#include "ComandoWhile.hpp"
#include "ExpressaoBinaria.hpp"
#include "ExpressaoChamada.hpp"
#include "ExpressaoDivisao.hpp"
#include "ExpressaoIgualdade.hpp"
#include "ExpressaoIndice.hpp"
#include "ExpressaoMenor.hpp"
#include "ExpressaoMod.hpp"
#include "ExpressaoMultiplicacao.hpp"
#include "ExpressaoNegacao.hpp"
#include "ExpressaoSoma.hpp"
#include "ExpressaoSubtracao.hpp"
#include "ExpressaoUnaria.hpp"
#include "ExpressaoValor.hpp"
#include "ExpressaoVariavel.hpp"
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class Ambiente {
public:
  vector<map<string, ValorLiteral> > escopos;

  Ambiente() {
    abre();
  }

  void abre() {
    escopos.push_back(map<string, ValorLiteral>());
  }

  void fecha() {
    escopos.pop_back();
  }

  void declara(const string &nome, const ValorLiteral &valor) {
    map<string, ValorLiteral> &topo = escopos.back();
    if (topo.count(nome)) throw runtime_error("Variavel ja declarada no escopo: " + nome);
    topo[nome] = valor;
  }

  ValorLiteral& busca(const string &nome) {
    for (int i = (int)escopos.size() - 1; i >= 0; --i) {
      map<string, ValorLiteral>::iterator it = escopos[i].find(nome);
      if (it != escopos[i].end()) return it->second;
    }
    throw runtime_error("Variavel nao declarada: " + nome);
  }
};

static ValorLiteral valor_padrao(Tipo* tipo) {
  ValorLiteral v;
  v.tipo = tipo;
  return v;
}

static ValorLiteral valor_array(Tipo* tipo_elemento, int tamanho) {
  if (tamanho < 0) throw runtime_error("Tamanho de array invalido");
  ValorLiteral v;
  v.eh_array = true;
  v.tipo = tipo_elemento;
  v.tipo_elemento = tipo_elemento;
  for (int i = 0; i < tamanho; ++i) {
    v.valores_array.push_back(new ValorLiteral(valor_padrao(tipo_elemento)));
  }
  return v;
}

static bool eh_numerico(const ValorLiteral &v) {
  return !v.eh_array && v.tipo != NULL && (v.tipo->valor == Tipo::INT || v.tipo->valor == Tipo::FLOAT);
}

static float como_float(const ValorLiteral &v) {
  return v.tipo->valor == Tipo::FLOAT ? v.valor_float : (float)v.valor_int;
}

static ValorLiteral converte_para(const ValorLiteral &v, Tipo* tipo) {
  if (v.eh_array) throw runtime_error("Array nao pode ser usado como valor escalar");
  if (tipo == NULL || v.tipo == NULL) throw runtime_error("Tipo invalido");
  if (v.tipo->valor == tipo->valor) return v;
  if (tipo->valor == Tipo::FLOAT && v.tipo->valor == Tipo::INT) {
    ValorLiteral r;
    r.tipo = tipo;
    r.valor_float = (float)v.valor_int;
    return r;
  }
  throw runtime_error("Conversao de tipo invalida: " + v.tipo->nome() + " para " + tipo->nome());
}

static ValorLiteral executa_funcao(Funcao *f, const vector<ValorLiteral> &params, map<string, Funcao*> &funcoes);
static ValorLiteral eval_expr(Expressao* e, Ambiente &amb, map<string, Funcao*> &funcoes);

static ValorLiteral eval_binaria(const string &op, Expressao* esquerda, Expressao* direita, Ambiente &amb, map<string, Funcao*> &funcoes) {
  ValorLiteral a = eval_expr(esquerda, amb, funcoes);
  ValorLiteral b = eval_expr(direita, amb, funcoes);

  if (op == "+" || op == "-" || op == "*" || op == "/" || op == "div" || op == "mod" || op == "%") {
    if (!eh_numerico(a) || !eh_numerico(b)) throw runtime_error("Operador aritmetico com tipo incompativel: " + op);
    if (op == "mod" || op == "%" || op == "div") {
      if (a.tipo->valor != Tipo::INT || b.tipo->valor != Tipo::INT) throw runtime_error("Operador inteiro com tipo nao inteiro: " + op);
      ValorLiteral r;
      r.tipo = new Tipo(Tipo::INT);
      if (op == "mod" || op == "%") r.valor_int = a.valor_int % b.valor_int;
      else r.valor_int = a.valor_int / b.valor_int;
      return r;
    }
    ValorLiteral r;
    bool float_result = a.tipo->valor == Tipo::FLOAT || b.tipo->valor == Tipo::FLOAT || op == "/";
    r.tipo = new Tipo(float_result ? Tipo::FLOAT : Tipo::INT);
    if (float_result) {
      if (op == "+") r.valor_float = como_float(a) + como_float(b);
      else if (op == "-") r.valor_float = como_float(a) - como_float(b);
      else if (op == "*") r.valor_float = como_float(a) * como_float(b);
      else r.valor_float = como_float(a) / como_float(b);
    } else {
      if (op == "+") r.valor_int = a.valor_int + b.valor_int;
      else if (op == "-") r.valor_int = a.valor_int - b.valor_int;
      else r.valor_int = a.valor_int * b.valor_int;
    }
    return r;
  }

  if (op == "and" || op == "or") {
    if (a.tipo->valor != Tipo::BOOL || b.tipo->valor != Tipo::BOOL) throw runtime_error("Operador logico com tipo nao booleano: " + op);
    ValorLiteral r;
    r.tipo = new Tipo(Tipo::BOOL);
    r.valor_bool = op == "and" ? (a.valor_bool && b.valor_bool) : (a.valor_bool || b.valor_bool);
    return r;
  }

  if (op == "==" || op == "!=") {
    if (a.tipo->valor != b.tipo->valor && !(eh_numerico(a) && eh_numerico(b))) throw runtime_error("Comparacao de igualdade com tipos incompativeis");
    ValorLiteral r;
    r.tipo = new Tipo(Tipo::BOOL);
    if (eh_numerico(a) && eh_numerico(b)) r.valor_bool = como_float(a) == como_float(b);
    else if (a.tipo->valor == Tipo::BOOL) r.valor_bool = a.valor_bool == b.valor_bool;
    else r.valor_bool = a.valor_int == b.valor_int;
    if (op == "!=") r.valor_bool = !r.valor_bool;
    return r;
  }

  if (op == "<" || op == "<=" || op == ">" || op == ">=") {
    if (!eh_numerico(a) || !eh_numerico(b)) throw runtime_error("Comparacao relacional com tipo nao numerico: " + op);
    ValorLiteral r;
    r.tipo = new Tipo(Tipo::BOOL);
    if (op == "<") r.valor_bool = como_float(a) < como_float(b);
    else if (op == "<=") r.valor_bool = como_float(a) <= como_float(b);
    else if (op == ">") r.valor_bool = como_float(a) > como_float(b);
    else r.valor_bool = como_float(a) >= como_float(b);
    return r;
  }

  throw runtime_error("Operador nao implementado: " + op);
}

static ValorLiteral eval_expr(Expressao* e, Ambiente &amb, map<string, Funcao*> &funcoes) {
  if (ExpressaoValor* v = dynamic_cast<ExpressaoValor*>(e)) return *(v->valor);
  if (ExpressaoVariavel* v = dynamic_cast<ExpressaoVariavel*>(e)) return amb.busca(v->nome->nome);
  if (ExpressaoIndice* idx = dynamic_cast<ExpressaoIndice*>(e)) {
    ExpressaoVariavel* base_var = dynamic_cast<ExpressaoVariavel*>(idx->base);
    if (base_var == NULL) throw runtime_error("Acesso a indice exige array identificado por variavel");
    ValorLiteral &arr = amb.busca(base_var->nome->nome);
    if (!arr.eh_array) throw runtime_error("Variavel nao e array: " + base_var->nome->nome);
    ValorLiteral indice = eval_expr(idx->indice, amb, funcoes);
    if (indice.eh_array || indice.tipo == NULL || indice.tipo->valor != Tipo::INT) throw runtime_error("Indice de array deve ser inteiro");
    if (indice.valor_int < 0 || indice.valor_int >= arr.valores_array.size()) throw runtime_error("Indice de array fora dos limites");
    return *(arr.valores_array[indice.valor_int]);
  }

  if (ExpressaoChamada* c = dynamic_cast<ExpressaoChamada*>(e)) {
    if (c->nome == NULL || !funcoes.count(c->nome->nome)) throw runtime_error("Funcao nao declarada: " + (c->nome == NULL ? string("") : c->nome->nome));
    vector<ValorLiteral> args;
    for (int i = 0; i < c->argumentos.size(); ++i) {
      args.push_back(eval_expr(c->argumentos[i], amb, funcoes));
    }
    return executa_funcao(funcoes[c->nome->nome], args, funcoes);
  }

  if (ExpressaoBinaria* b = dynamic_cast<ExpressaoBinaria*>(e)) return eval_binaria(b->operador, b->esquerda, b->direita, amb, funcoes);
  if (ExpressaoSoma* b = dynamic_cast<ExpressaoSoma*>(e)) return eval_binaria("+", b->esquerda, b->direita, amb, funcoes);
  if (ExpressaoSubtracao* b = dynamic_cast<ExpressaoSubtracao*>(e)) return eval_binaria("-", b->esquerda, b->direita, amb, funcoes);
  if (ExpressaoMultiplicacao* b = dynamic_cast<ExpressaoMultiplicacao*>(e)) return eval_binaria("*", b->esquerda, b->direita, amb, funcoes);
  if (ExpressaoDivisao* b = dynamic_cast<ExpressaoDivisao*>(e)) return eval_binaria("/", b->esquerda, b->direita, amb, funcoes);
  if (ExpressaoMod* b = dynamic_cast<ExpressaoMod*>(e)) return eval_binaria("%", b->esquerda, b->direita, amb, funcoes);
  if (ExpressaoIgualdade* b = dynamic_cast<ExpressaoIgualdade*>(e)) return eval_binaria("==", b->esquerda, b->direita, amb, funcoes);
  if (ExpressaoMenor* b = dynamic_cast<ExpressaoMenor*>(e)) return eval_binaria("<", b->esquerda, b->direita, amb, funcoes);

  if (ExpressaoUnaria* u = dynamic_cast<ExpressaoUnaria*>(e)) {
    ValorLiteral v = eval_expr(u->expressao, amb, funcoes);
    if (u->operador == "not" || u->operador == "!") {
      if (v.tipo->valor != Tipo::BOOL) throw runtime_error("Negacao logica exige booleano");
      ValorLiteral r;
      r.tipo = new Tipo(Tipo::BOOL);
      r.valor_bool = !v.valor_bool;
      return r;
    }
    if (u->operador == "-") {
      if (!eh_numerico(v)) throw runtime_error("Menos unario exige numero");
      ValorLiteral r = v;
      if (r.tipo->valor == Tipo::FLOAT) r.valor_float = -r.valor_float;
      else r.valor_int = -r.valor_int;
      return r;
    }
  }
  if (ExpressaoNegacao* u = dynamic_cast<ExpressaoNegacao*>(e)) {
    ValorLiteral v = eval_expr(u->expressao, amb, funcoes);
    if (v.tipo->valor != Tipo::BOOL) throw runtime_error("Negacao logica exige booleano");
    ValorLiteral r;
    r.tipo = new Tipo(Tipo::BOOL);
    r.valor_bool = !v.valor_bool;
    return r;
  }

  throw runtime_error("Expressao nao implementada");
}

class ResultadoComando {
public:
  bool retornou;
  ValorLiteral valor;
  ResultadoComando() : retornou(false) {}
};

static ResultadoComando executa_comandos(const vector<Comando*> &comandos, Ambiente &amb, map<string, Funcao*> &funcoes);

static ResultadoComando executa_comando(Comando* c, Ambiente &amb, map<string, Funcao*> &funcoes) {
  ResultadoComando res;
  if (ComandoDeclaracao* d = dynamic_cast<ComandoDeclaracao*>(c)) {
    ValorLiteral valor = d->variavel->eh_array ? valor_array(d->variavel->tipo, d->variavel->tamanho_array) : valor_padrao(d->variavel->tipo);
    if (d->variavel->eh_array && d->inicializador != NULL) throw runtime_error("Inicializacao de array nao suportada");
    if (d->inicializador != NULL) valor = converte_para(eval_expr(d->inicializador, amb, funcoes), d->variavel->tipo);
    amb.declara(d->variavel->nome->nome, valor);
    return res;
  }
  if (ComandoAtribuicao* a = dynamic_cast<ComandoAtribuicao*>(c)) {
    ValorLiteral &destino = amb.busca(a->esquerda->nome);
    if (destino.eh_array) throw runtime_error("Atribuicao direta em array nao suportada: " + a->esquerda->nome);
    destino = converte_para(eval_expr(a->direita, amb, funcoes), destino.tipo);
    return res;
  }
  if (ComandoArrayAtribuicao* a = dynamic_cast<ComandoArrayAtribuicao*>(c)) {
    ValorLiteral &arr = amb.busca(a->nome->nome);
    if (!arr.eh_array) throw runtime_error("Variavel nao e array: " + a->nome->nome);
    ValorLiteral indice = eval_expr(a->indice, amb, funcoes);
    if (indice.eh_array || indice.tipo == NULL || indice.tipo->valor != Tipo::INT) throw runtime_error("Indice de array deve ser inteiro");
    if (indice.valor_int < 0 || indice.valor_int >= arr.valores_array.size()) throw runtime_error("Indice de array fora dos limites");
    *(arr.valores_array[indice.valor_int]) = converte_para(eval_expr(a->direita, amb, funcoes), arr.tipo_elemento);
    return res;
  }
  if (ComandoLista* l = dynamic_cast<ComandoLista*>(c)) {
    amb.abre();
    res = executa_comandos(l->lista_comandos, amb, funcoes);
    amb.fecha();
    return res;
  }
  if (ComandoIf* i = dynamic_cast<ComandoIf*>(c)) {
    ValorLiteral cond = eval_expr(i->condicao, amb, funcoes);
    if (cond.tipo->valor != Tipo::BOOL) throw runtime_error("Condicao do if deve ser booleana");
    amb.abre();
    res = executa_comandos(cond.valor_bool ? i->bloco_then : i->bloco_else, amb, funcoes);
    amb.fecha();
    return res;
  }
  if (ComandoWhile* w = dynamic_cast<ComandoWhile*>(c)) {
    int limite = 100000;
    while (limite-- > 0) {
      ValorLiteral cond = eval_expr(w->condicao, amb, funcoes);
      if (cond.tipo->valor != Tipo::BOOL) throw runtime_error("Condicao do while deve ser booleana");
      if (!cond.valor_bool) break;
      amb.abre();
      res = executa_comandos(w->bloco, amb, funcoes);
      amb.fecha();
      if (res.retornou) return res;
    }
    if (limite <= 0) throw runtime_error("Limite de iteracoes do while atingido");
    return res;
  }
  if (ComandoRetorno* r = dynamic_cast<ComandoRetorno*>(c)) {
    res.retornou = true;
    res.valor = eval_expr(r->expressao, amb, funcoes);
    return res;
  }
  throw runtime_error("Comando nao implementado");
}

static ResultadoComando executa_comandos(const vector<Comando*> &comandos, Ambiente &amb, map<string, Funcao*> &funcoes) {
  ResultadoComando res;
  for (int i = 0; i < comandos.size(); ++i) {
    res = executa_comando(comandos[i], amb, funcoes);
    if (res.retornou) return res;
  }
  return res;
}

static ValorLiteral executa_funcao(Funcao *f, const vector<ValorLiteral> &params, map<string, Funcao*> &funcoes) {
  if (f == NULL) throw runtime_error("Funcao invalida");
  Ambiente amb;
  if (params.size() != f->parametros.size()) throw runtime_error("Quantidade de parametros incompativel na chamada de " + f->nome_funcao->nome);
  for (int i = 0; i < f->parametros.size(); ++i) {
    amb.declara(f->parametros[i]->nome->nome, converte_para(params[i], f->parametros[i]->tipo));
  }
  ResultadoComando res = executa_comandos(f->comandos, amb, funcoes);
  if (!res.retornou) throw runtime_error("Funcao terminou sem return");
  return converte_para(res.valor, f->tipo_retorno);
}

void Analisador::registra_funcoes(const vector<Funcao*> &lista_funcoes) {
  funcoes.clear();
  for (int i = 0; i < lista_funcoes.size(); ++i) {
    if (lista_funcoes[i] == NULL || lista_funcoes[i]->nome_funcao == NULL) continue;
    string nome = lista_funcoes[i]->nome_funcao->nome;
    if (funcoes.count(nome)) throw runtime_error("Funcao declarada mais de uma vez: " + nome);
    funcoes[nome] = lista_funcoes[i];
  }
}

ValorLiteral Analisador::calcula_retorno(Funcao *f, const vector<ValorLiteral> &params) {
  if (funcoes.empty() && f != NULL && f->nome_funcao != NULL) funcoes[f->nome_funcao->nome] = f;
  return executa_funcao(f, params, funcoes);
}
