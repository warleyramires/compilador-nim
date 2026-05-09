#include "ExpressaoUnaria.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoUnaria::ExpressaoUnaria(const string &operador) {
  this->operador = operador;
  expressao = NULL;
}

void ExpressaoUnaria::debug_com_tab(int tab) {
  tab3(tab);
  cerr << operador << " [ INI Unaria" << endl;
  if (expressao != NULL) expressao->debug_com_tab(tab+1);
  tab3(tab);
  cerr << operador << " ] FIM Unaria" << endl;
}
