#ifndef TOKENIZADOR_H
  #define TOKENIZADOR_H

  #include <fstream>
  #include <string>
  #include <iostream>
  #include <list>
  #include <vector>
  #include <algorithm>

  #include <sys/stat.h>


  using namespace std;

  class OutputIF {
    public:
      virtual void add(const string &word) = 0;
  };

  class OutputList : public OutputIF {
    public:
      list<string>& output;

      OutputList(list<string>& o);

      void add(const string& word);
  };

  class OutputString : public OutputIF {
    public:
      string &output;

      OutputString(string& o);

      void add(const string &word);
  };

  class Tokenizador {
      friend ostream& operator<<(ostream&, const Tokenizador&);
      /* cout << "DELIMITADORES: " << delimiters
       *      << " TRATA CASOS ESPECIALES: " << casosEspeciales
       *      << " PASAR A MINUSCULAS Y SIN ACENTOS: " << pasarAminuscSinAcentos;
       * Aunque se modifique el almacenamiento de los delimitadores por temas de eficiencia
       * el campo delimiters se imprimira con el string con el que se inicializo el tokenizador
       */
    public:

      Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos);
      /* Inicializa delimiters a delimitadoresPalabra filtrando que no se introduzcan delimitadores
       * repetidos (de izquierda a derecha, en cuyo caso se eliminarian los que hayan sido repetidos
       * por la derecha);
       * casosEspeciales a kcasosEspeciales;
       * pasarAminuscSinAcentos a minuscSinAcentos;
       */

      Tokenizador(const Tokenizador&);

      Tokenizador();
      /* Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
       * casosEspeciales a true;
       * pasarAminuscSinAcentos a false;
       */

      ~Tokenizador();
      // Pone delimiters=""

      Tokenizador& operator=(const Tokenizador&);

      //string to output
      void Tokenizar(const string& str, OutputIF& output);

      void Tokenizar(const string& str, list<string>& tokens);
      /* Tokeniza str devolviendo el resultado en tokens.
       * La lista tokens se vaciará antes de almacenar el resultado de la tokenizacion
       */

      bool Tokenizar(const string& i, const string& f);
      /* Tokeniza el fichero i guardando la salida en el fichero f (una palabra en cada linea del
       * fichero).
       * Devolverá true si se realiza la tokenizacion de forma correcta; false en caso contrario
       * enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
       */

      bool Tokenizar(const string& i);
      /* Tokeniza el fichero i guardando la salida en un fichero de nombre i añadiendole
       * extension .tk (sin eliminar previamente la extension de i por ejemplo, del archivo pp.txt se
       * generaria el resultado en pp.txt.tk), y que contendra una palabra en cada linea del fichero.
       * Devolvera true si se realiza la tokenizacion de forma correcta;
       * false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el
       * archivo i)
       */

      bool TokenizarListaFicheros(const string &i);
      /* Tokeniza el fichero i que contiene un nombre de fichero por linea guardando la salida en
       * ficheros (uno por cada linea de i) cuyo nombre sera el leido en i añadiendole la
       * extension .tk, y que contendra una palabra en cada line del fichero leido en i.
       * Devolvera true si se realiza la tokenizacion de forma correcta de todos los archivos que
       * contiene i;
       * devolvera false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no
       * exista el archivo i, o que se trate de un directorio, enviando a "cerr" los archivos de i
       * que no existan o que sean directorios; luego no se ha de interrumpir la ejecucion si hay
       * algun archivo en i que no exista)
       */

      bool TokenizarDirectorio(const string& i);
      /* Tokeniza todos los archivos que contenga el directorio i, incluyendo los de los
       * subdirectorios, guardando la salida en ficheros cuyo nombre sera el de entrada añadiendole
       * extension .tk, y que contendra una palabra en cada linea del fichero.
       * Devolvera true si se realiza la tokenizacion de forma correcta de todos los archivos;
       * devolvera false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no
       * exista el directorio i, o los ficheros que no se hayan podido tokenizar)
       */

      void DelimitadoresPalabra(const string& nuevoDelimiters);
      /* Cambia "delimiters" por "nuevoDelimiters" comprobando que no hayan delimitadores repetidos
       * (de izquierda a derecha), en cuyo caso se eliminarian los que hayan sido repetidos (por la
       * derecha)
       */

      string DelimitadoresPalabra() const;
      //Devuelve "delimiters"

      void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters);
      /* Añade al final de delimiters los nuevos delimitadores que aparezcan en nuevoDelimiters (no se
       * almacenaran caracteres repetidos)
       */

      void CasosEspeciales(const bool& nuevoCasosEspeciales);
      //Cambia la variable privada "casosEspeciales"

      bool CasosEspeciales() const;
      //Devuelve el contenido de la variable privada "casosEspeciales"

      void PasarAminuscSinAcentos(const bool& nuevoPasarAminuscSinAcentos);
      /* Cambia la variable privada "pasarAminuscSinAcentos". Atencion al formato de codificacion del
       * corpus (comando "file" de Linux). Para la correccion de la practica se utilizara el formato
       * actual ISO-8859).
       */

      bool PasarAminuscSinAcentos();
      // Devuelve el contenido de la variable privada "pasarAminuscSinAcentos"

    private:
      static void normalizarDelimitadores(string& delims);

      string delimiters;
      /* Delimitadores de terminos. Aunque se modifique la forma de almacenamiento interna para
       * mejorar la eficiencia este campo debe permanecer para indicar el orden en que se
       * introdujeron los delimitadores
       */

      bool casosEspeciales = false;
      /* Si true detectara palabras compuestas y casos especiales. Sino trabajar al igual que el
       * algoritmo propuesto en la seccion "Version del tokenizador vista en clase"
       */

      bool pasarAminuscSinAcentos;
      // Si true pasara el token a minusculas y quitara acentos antes de realizar la tokenizacion


      //automata de estados
      vector<unsigned (Tokenizador::*)(const char &, string &, OutputIF &) > funcionesDelim = {
        &Tokenizador::estado0_delimNoCasosEspeciales,
        &Tokenizador::estado1_delim,
        &Tokenizador::estado2_delim,
        &Tokenizador::estado3_delim,
        &Tokenizador::estado4_delim,
        &Tokenizador::estado5_delim,
        &Tokenizador::estado6_delim,
        &Tokenizador::estado7_delim,
        &Tokenizador::estado8_delim,
        &Tokenizador::estado9_delim,
        &Tokenizador::estado10_delim
      };

      vector<unsigned (Tokenizador::*)(const char &, string &, OutputIF &) > funcionesNoDelim = {
        &Tokenizador::estado0_noDelimNoCasosEspeciales,
        &Tokenizador::estado1_noDelim,
        &Tokenizador::estado2_noDelim,
        &Tokenizador::estado3_noDelim,
        &Tokenizador::estado4_noDelim,
        &Tokenizador::estado5_noDelim,
        &Tokenizador::estado6_noDelim,
        &Tokenizador::estado7_noDelim,
        &Tokenizador::estado8_noDelim,
        &Tokenizador::estado9_noDelim,
        &Tokenizador::estado10_noDelim
      };


      unsigned estado0_delimNoCasosEspeciales(const char& c, string& word, OutputIF& output);
      unsigned estado0_noDelimNoCasosEspeciales(const char& c, string& word, OutputIF& output);

      unsigned estado0_delim(const char& c, string& word, OutputIF& output);
      unsigned estado0_noDelim(const char& c, string& word, OutputIF& output);

      unsigned estado1_delim(const char& c, string& word, OutputIF& output);
      unsigned estado1_noDelim(const char& c, string& word, OutputIF& output);

      unsigned estado2_noDelim(const char& c, string& word, OutputIF& output);
      unsigned estado2_delim(const char& c, string& word, OutputIF& output);

      unsigned estado3_delim(const char& c, string& word, OutputIF& output);
      unsigned estado3_noDelim(const char& c, string& word, OutputIF& output);

      unsigned estado4_delim(const char& c, string& word, OutputIF& output);
      unsigned estado4_noDelim(const char& c, string& word, OutputIF& output);

      string prefijoNum = ""; string especulativoNum = ""; bool activoNum = false;
      string especulativoSimb = ""; unsigned estadoSimb = 0;

      unsigned estado5_delim(const char& c, string& word, OutputIF& output);
      unsigned estado5_noDelim(const char& c, string& word, OutputIF& output);

      unsigned estado6_delim(const char& c, string& word, OutputIF& output);
      unsigned estado6_noDelim(const char& c, string& word, OutputIF& output);

      unsigned estado7_delim(const char& c, string& word, OutputIF& output);
      unsigned estado7_noDelim(const char& c, string& word, OutputIF& output);

      unsigned estado8_delim(const char& c, string& word, OutputIF& output);
      unsigned estado8_noDelim(const char& c, string& word, OutputIF& output);

      unsigned estado9_delim(const char& c, string& word, OutputIF& output);
      unsigned estado9_noDelim(const char& c, string& word, OutputIF& output);

      unsigned estado10_delim(const char& c, string& word, OutputIF& output);
      unsigned estado10_noDelim(const char& c, string& word, OutputIF& output);


      //pasar a minusc por array
      char conversion[256];
      char normal[256];
      char* addChar;
      void rellenarConversion(void);

      //espacio como delimitador en casos especiales
      void comprobarDelimitadoresCasosEspeciales(void);

      //idx delimiters
      vector<char> idxDelims;
      vector<unsigned> idx;
      vector<float> idxCount;

      void encontradoDelimitador(unsigned pos);
  };

#endif
