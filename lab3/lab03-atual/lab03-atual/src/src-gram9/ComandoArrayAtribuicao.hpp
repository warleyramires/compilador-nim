#ifndef _COMANDO_ARRAY_ATRIBUICAO_HPP_
#define _COMANDO_ARRAY_ATRIBUICAO_HPP_
#include "Comando.hpp"
#include "ID.hpp"
#include "Expressao.hpp"
using namespace std;

class ComandoArrayAtribuicao : public Comando {
public:
  ID* nome;
  Expressao* indice;
  Expressao* direita;
  ComandoArrayAtribuicao();
  void debug_com_tab(int tab);
};

#endif
