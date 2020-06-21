#include "tokenizador.h"

int main (void) {
  Tokenizador a(":", false, false);
  list<string> tokens;
  tokens.push_back("payback is a bitch");
  tokens.push_back("aint she");
  a.Tokenizar("jEJe:a ver QUe tal:quiEro saber::que pasa por aqui!:", tokens);
  for(auto i=tokens.begin(); i != tokens.end(); ++i) {
    cout << *i << endl;
  }
  a.TokenizarDirectorio("src/test");
  return 0;
}
