#include "ExpressaoIndice.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoIndice::ExpressaoIndice() {
  base = NULL;
  indice = NULL;
}

void ExpressaoIndice::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI INDICE" << endl;
  if (base != NULL) base->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "[" << endl;
  if (indice != NULL) indice->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "] FIM INDICE" << endl;
}
