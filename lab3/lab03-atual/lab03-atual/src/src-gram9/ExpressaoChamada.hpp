#ifndef _EXPRESSAO_CHAMADA_HPP_
#define _EXPRESSAO_CHAMADA_HPP_
#include "Expressao.hpp"
#include "ID.hpp"
#include <vector>
using namespace std;

class ExpressaoChamada : public Expressao {
public:
  ID* nome;
  vector<Expressao*> argumentos;
  ExpressaoChamada();
  void debug_com_tab(int tab);
};

#endif
