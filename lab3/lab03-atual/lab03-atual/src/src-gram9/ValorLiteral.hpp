#ifndef _VALOR_LITERAL_HPP_
#define _VALOR_LITERAL_HPP_
#include "Tipo.hpp"
#include "../Arvore.hpp"
#include <string>
#include <vector>
using namespace std;

class ValorLiteral {
public:
  Tipo* tipo;
  bool eh_array;
  Tipo* tipo_elemento;
  vector<ValorLiteral*> valores_array;
  int valor_int;
  float valor_float;
  bool valor_bool;

  ValorLiteral();
  string como_string() const;
  static ValorLiteral* extrai_valor_literal(No_arv_parse* no);
};

#endif
