#include "ComandoWhile.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ComandoWhile::ComandoWhile() {
  condicao = NULL;
}

void ComandoWhile::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI WHILE" << endl;
  if (condicao != NULL) condicao->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "DO" << endl;
  for (int i = 0; i < bloco.size(); ++i) bloco[i]->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "FIM WHILE" << endl;
}
