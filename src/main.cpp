#include "tokenizador.h"
#include <iostream>

using namespace std;

int main (void) {
  Output<string>* os = new Output<string>();
  TokenAccumulator* ta = os;
  Tokenizador tk = new Tokenizador();
  string str = "hola estas&son\\palabras/o_al.menos-deberian!serlo";

  tk->Tokeniza(str, ta);

  cout << os->getRepresentation() << endl;
  delete os;
  delete tk;
}
