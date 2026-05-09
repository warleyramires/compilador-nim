#include "ExpressaoBinaria.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoBinaria::ExpressaoBinaria(const string &operador) {
  this->operador = operador;
  esquerda = NULL;
  direita = NULL;
}

void ExpressaoBinaria::debug_com_tab(int tab) {
  tab3(tab);
  cerr << operador << " [ INI Binaria" << endl;
  if (esquerda != NULL) esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << operador << endl;
  if (direita != NULL) direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << operador << " ] FIM Binaria" << endl;
}
