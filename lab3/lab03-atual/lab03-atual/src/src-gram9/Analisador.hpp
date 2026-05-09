#ifndef _ANALISADOR_HPP_
#define _ANALISADOR_HPP_
#include "Funcao.hpp"
#include "ValorLiteral.hpp"
#include <map>
#include <vector>
#include <string>
using namespace std;

class Analisador {
  map<string, Funcao*> funcoes;
public:
  void registra_funcoes(const vector<Funcao*> &funcoes);
  ValorLiteral calcula_retorno(Funcao *f, const vector<ValorLiteral> &params);
};
  
#endif
