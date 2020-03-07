#ifndef OUTPUT_H
  #define OUTPUT_H
  #include<string>
  #include <list>

  using namespace std;

  //Interfaz de acumulador de tokens
  class TokenAccumulator {
    public:
      virtual void addToken(string token) = 0;
  };

  //overkill cuando podría ser solo OutputString & OutputList, pero:
  //  1. Los templates no te pierden eficiencia, el dynamic binding si
  //  2. Me molestaba repetir el codigo de getRepresentation
  //  3. Me emperré y quería aprender algo nuevo sobre c++
  template<class T>
    class Output : public TokenAccumulator {
      private:
	       T output;
      public:
	       T getRepresentation(void) { return output; }
         void addToken(string token);
    };


/*

https://es.cppreference.com/w/cpp/language/template_specialization
https://www.tutorialspoint.com/cplusplus/cpp_polymorphism.htm
http://www.cplusplus.com/doc/oldtutorial/templates/
https://stackoverflow.com/questions/611263/efficient-string-concatenation-in-c
https://brianbondy.com/blog/90/introducing-the-rope-data-structure

clase llamada output<T>, con el objetivo de trabajar both con strings y listas utilizando una misma interfaz

la interfaz deberia ser aproximadamente

public añadir token (recibe un string y lo mete)
public obtener representacion (devuelve la lista o string)

el formato del string es:

token1
token2
token3
token4
...

no olvidarse de crear output.h y hacer include directo del cpp en tokenizador para que compile


*/

#endif
