#include "ComandoIf.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ComandoIf::ComandoIf() {
  condicao = NULL;
}

void ComandoIf::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI IF" << endl;
  if (condicao != NULL) condicao->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "THEN" << endl;
  for (int i = 0; i < bloco_then.size(); ++i) bloco_then[i]->debug_com_tab(tab+1);
  if (!bloco_else.empty()) {
    tab3(tab);
    cerr << "ELSE" << endl;
    for (int i = 0; i < bloco_else.size(); ++i) bloco_else[i]->debug_com_tab(tab+1);
  }
  tab3(tab);
  cerr << "FIM IF" << endl;
}
