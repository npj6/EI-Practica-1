#include "output.h"
#include <iostream>

using namespace std;

int main (void) {
  Output<string>* os = new Output<string>();
  TokenAccumulator* ta = os;
  ta->addToken("hello");
  ta->addToken("world");
  ta->addToken("this");
  ta->addToken("a test");
  cout << os->getRepresentation() << endl;
  delete os;
}
