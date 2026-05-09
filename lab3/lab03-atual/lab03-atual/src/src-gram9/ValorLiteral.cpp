#include "ValorLiteral.hpp"
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

ValorLiteral::ValorLiteral() {
  tipo = NULL;
  eh_array = false;
  tipo_elemento = NULL;
  valor_int = 0;
  valor_float = 0.0f;
  valor_bool = false;
}

string ValorLiteral::como_string() const {
  if (eh_array) return "[array]";
  if (tipo == NULL) return "";
  stringstream ss;
  switch(tipo->valor) {
  case Tipo::INT:
    ss << valor_int;
    break;
  case Tipo::FLOAT:
    ss << fixed << setprecision(2) << valor_float;
    break;
  case Tipo::BOOL:
    ss << (valor_bool ? "true" : "false");
    break;
  }
  return ss.str();
}

bool texto_bool(string texto) {
  transform(texto.begin(), texto.end(), texto.begin(), ::toupper);
  return texto == "TRUE" || texto == "1";
}

ValorLiteral* ValorLiteral::extrai_valor_literal(No_arv_parse* no) {
  if (no == NULL) return NULL;
  ValorLiteral* res = new ValorLiteral();
  res->tipo = Tipo::extrai_tipo_literal(no);
  if (res->tipo == NULL) return res;

  switch(res->tipo->valor) {
  case Tipo::INT:
    res->valor_int = atoi(no->dado_extra.c_str());
    break;
  case Tipo::FLOAT:
    res->valor_float = atof(no->dado_extra.c_str());
    break;
  case Tipo::BOOL:
    res->valor_bool = texto_bool(no->dado_extra);
    break;
  }

  return res;
}
