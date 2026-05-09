#ifndef _EXPRESSAO_UNARIA_HPP_
#define _EXPRESSAO_UNARIA_HPP_
#include "Expressao.hpp"
#include <string>
using namespace std;

class ExpressaoUnaria : public Expressao {
public:
  string operador;
  Expressao* expressao;
  ExpressaoUnaria(const string &operador);
  void debug_com_tab(int tab);
};

#endif
