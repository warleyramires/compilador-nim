#include <algorithm>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

#include "Analisador.hpp"
#include "Arvore.hpp"
#include "Funcao.hpp"
#include "Gramatica.hpp"
#include "Parser.hpp"
#include "TabelaLR1.hpp"

static string trim(const string &s) {
  size_t ini = s.find_first_not_of(" \t\r\n");
  if (ini == string::npos) return "";
  size_t fim = s.find_last_not_of(" \t\r\n");
  return s.substr(ini, fim - ini + 1);
}

static bool texto_bool_param(string texto) {
  transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
  return texto == "true" || texto == "1";
}

static ValorLiteral valor_parametro_por_tipo(const string &texto, Tipo* tipo) {
  ValorLiteral valor;
  valor.tipo = tipo;
  if (tipo == NULL) throw runtime_error("Tipo de parametro invalido");

  switch(tipo->valor) {
  case Tipo::INT:
    valor.valor_int = atoi(texto.c_str());
    break;
  case Tipo::FLOAT:
    valor.valor_float = (float)atof(texto.c_str());
    break;
  case Tipo::BOOL:
    valor.valor_bool = texto_bool_param(texto);
    break;
  }

  return valor;
}

static vector<ValorLiteral> le_parametros(const string &nome_arquivo, Funcao* func) {
  vector<ValorLiteral> params;
  if (nome_arquivo.empty()) return params;

  ifstream entrada(nome_arquivo);
  if (entrada.fail()) throw runtime_error("Falha ao abrir arquivo de parametros: " + nome_arquivo);

  string linha;
  int indice = 0;
  while (getline(entrada, linha)) {
    linha = trim(linha);
    if (linha.empty() || linha[0] == '#') continue;
    if (indice >= func->parametros.size()) throw runtime_error("Parametros em excesso");
    params.push_back(valor_parametro_por_tipo(linha, func->parametros[indice]->tipo));
    indice++;
  }

  return params;
}

int main(int argc, char * argv[]) {
  if (argc != 4 && argc != 3 && argc != 2 && argc != 1) {
    cerr << "Uso: compilador [params] < tokens" << endl;
    cerr << "  ou: compilador gramatica tabela [params] < tokens" << endl;
    return 1;
  }

  string nome_gramatica, nome_tab_lr1, nome_params;
  if (argc == 1 || argc == 2) {
    nome_gramatica = string("gramatica-nim/gramatica-nim.site");
    nome_tab_lr1 = string("gramatica-nim/tabela_lr1.conf");
    if (argc == 2) nome_params = string(argv[1]);
  } else {
    nome_gramatica = string(argv[1]);
    nome_tab_lr1 = string(argv[2]);
    if (argc == 4) nome_params = string(argv[3]);
  }

  ifstream arq_gramatica(nome_gramatica);
  ifstream arq_tabela_lr1(nome_tab_lr1);
  if (arq_tabela_lr1.fail() || arq_gramatica.fail()) {
    cerr << "Falha ao abrir arquivos: "
         << ((arq_gramatica.fail()) ? nome_gramatica : "") << ", "
         << ((arq_tabela_lr1.fail()) ? nome_tab_lr1 : "") << endl;
    return 1;
  }

  try {
    ios_base::iostate estado_cerr = cerr.rdstate();
    cerr.setstate(ios_base::failbit);
    Parser parser(arq_gramatica, arq_tabela_lr1);
    Arvore_parse arv = parser.executa_parse(cin);
    cerr.clear(estado_cerr);

    if (arv.raiz == NULL) {
      cout << "ERRO SINTATICO" << endl;
      return 1;
    }

    vector<Funcao*> funcoes = Funcao::extrai_funcoes(arv.raiz);
    Funcao* func = funcoes.empty() ? NULL : funcoes[0];
    if (func == NULL) {
      cout << "ERRO SEMANTICO: funcao invalida" << endl;
      return 1;
    }

    vector<ValorLiteral> parametros_passados = le_parametros(nome_params, func);
    if (parametros_passados.size() != func->parametros.size()) {
      throw runtime_error("Quantidade de parametros incompativel");
    }

    Analisador ana;
    ana.registra_funcoes(funcoes);
    cout << ana.calcula_retorno(func, parametros_passados).como_string() << endl;
  } catch (const exception &e) {
    cerr.clear();
    cout << "ERRO SEMANTICO: " << e.what() << endl;
    return 1;
  }

  return 0;
}
