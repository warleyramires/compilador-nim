#ifndef _EXPRESSAO_BINARIA_HPP_
#define _EXPRESSAO_BINARIA_HPP_
#include "Expressao.hpp"
#include <string>
using namespace std;

class ExpressaoBinaria : public Expressao {
public:
  string operador;
  Expressao* esquerda;
  Expressao* direita;
  ExpressaoBinaria(const string &operador);
  void debug_com_tab(int tab);
};

#endif
