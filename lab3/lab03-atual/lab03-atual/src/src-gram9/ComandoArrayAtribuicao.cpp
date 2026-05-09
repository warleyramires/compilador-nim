#include "ComandoArrayAtribuicao.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ComandoArrayAtribuicao::ComandoArrayAtribuicao() {
  nome = NULL;
  indice = NULL;
  direita = NULL;
}

void ComandoArrayAtribuicao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI ARRAY ATRIB: " << (nome == NULL ? "ARRAY_INVALIDO" : nome->nome) << "[" << endl;
  if (indice != NULL) indice->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "] = " << endl;
  if (direita != NULL) direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "FIM ARRAY ATRIB" << endl;
}
