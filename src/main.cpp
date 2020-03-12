#include "tokenizador.h"
#include <iostream>

using namespace std;

int main (void) {
  Tokenizador* tk = new Tokenizador();
  string input_file = "small.corpus";
  string output_file = "salida.txt";
  tk->Tokenizar(input_file, "salida.txt");
  delete tk;
}
