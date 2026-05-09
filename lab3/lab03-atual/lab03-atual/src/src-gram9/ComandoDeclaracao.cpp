#include "ComandoDeclaracao.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoDeclaracao::ComandoDeclaracao() {
  variavel = NULL;
  inicializador = NULL;
}

void ComandoDeclaracao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << variavel->nome->nome << ":" << (variavel->tipo == NULL ? "TIPO_INVALIDO" : variavel->tipo->nome()) << "] Declaracao" << endl;
  if (inicializador != NULL) {
    tab3(tab);
    cerr << "INI Inicializador" << endl;
    inicializador->debug_com_tab(tab+1);
    tab3(tab);
    cerr << "FIM Inicializador" << endl;
  }
}
