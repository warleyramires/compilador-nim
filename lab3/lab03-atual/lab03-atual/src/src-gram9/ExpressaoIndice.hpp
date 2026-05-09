#ifndef _EXPRESSAO_INDICE_HPP_
#define _EXPRESSAO_INDICE_HPP_
#include "Expressao.hpp"
using namespace std;

class ExpressaoIndice : public Expressao {
public:
  Expressao* base;
  Expressao* indice;
  ExpressaoIndice();
  void debug_com_tab(int tab);
};

#endif
