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

  template<class T>
    class Output : TokenAccumulator {
      private:
	T output;
      public:
	T getRepresentation(void) {
	  return output;
	}
    };

  template<>
  class Output<string> {
    public:
      void addToken(string token) {
	output += "\n" + token;
      }
  };

  template<>
  class Output<list<string> > {
    public:
      void addToken(string token) {
	output.push_back(token);
      }
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
