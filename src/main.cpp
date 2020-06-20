#include "tokenizador.h"
#include <iostream>

int main (void) {
  Tokenizador a(":", false, false);
  list<string> tokens;
  a.Tokenizar("jeje:a ver que tal:quiero saber::que pasa por aqui!:", tokens);
  for(auto i=tokens.begin(); i != tokens.end(); ++i) {
    cout << *i << endl;
  }
  return 0;
}
