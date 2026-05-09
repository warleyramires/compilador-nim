#include "ExpressaoChamada.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoChamada::ExpressaoChamada() {
  nome = NULL;
}

void ExpressaoChamada::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "CALL " << (nome == NULL ? "FUNCAO_INVALIDA" : nome->nome) << "(" << endl;
  for (int i = 0; i < argumentos.size(); ++i) {
    argumentos[i]->debug_com_tab(tab+1);
  }
  tab3(tab);
  cerr << ") FIM CALL" << endl;
}
