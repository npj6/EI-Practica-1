#include "tokenizador.h"
//output classes code
#include "output.cpp"

using namespace std;

ostream& operator<<(ostream& os, const Tokenizador& tokenizador) {
  os << "DELIMITADORES: " << tokenizador.delimiters
     << " TRATA CASOS ESPECIALES: " << tokenizador.casosEspeciales
     << " PASAR A MINUSCULAS Y SIN ACENTOS: " << tokenizador.pasarAminuscSinAcentos;
  return os;
}

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos) {
  delimiters = delimitadoresPalabra;
  normalizarDelimitadores(delimiters);
  casosEspeciales = kcasosEspeciales;
  pasarAminuscSinAcentos = minuscSinAcentos;
}

Tokenizador::Tokenizador(const Tokenizador& tokenizador) {
  delimiters = tokenizador.delimiters;
  casosEspeciales = tokenizador.casosEspeciales;
  pasarAminuscSinAcentos = tokenizador.pasarAminuscSinAcentos;
}

Tokenizador::Tokenizador() {
  delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
  casosEspeciales = true;
  pasarAminuscSinAcentos = false;
}

Tokenizador::~Tokenizador() {
  delimiters = "";
}

Tokenizador& Tokenizador::operator=(const Tokenizador& tokenizador) {
  delimiters = tokenizador.delimiters;
  casosEspeciales = tokenizador.casosEspeciales;
  pasarAminuscSinAcentos = tokenizador.pasarAminuscSinAcentos;
  return *this;
}

/*MAIN FUNCTIONS*/

/*
string to output<T> (devolver output por referencia)
*/


//crea otro visitor para el input, generaliza un "nextCharacter", haz que el visitor decida si se quitan caracteres especiales o no.
//el inputVisitor puede ser un archivo o un string
//considera un TokenAccumulator de file, comprueba si es mas lento que TokenAccumulator de string y escribirlo todo de una pasada.
//ten cuidado con las signatures de las funciones

void Tokenizador::Tokenizar(const string& str, TokenAccumulator &ta) const {
  string texto;
  normalizarTexto(str, texto);
  string token = "";
  for(int i=0; i<texto.size(); i++) {
    bool isDelimiter = false;
    for(int d=0; d<delimiters.size() && !isDelimiter; d++) {
      if(texto[i]==delimiters[d]) {
        if(token.size() != 0) {
          ta.addToken(token);
          token = "";
        }
        isDelimiter = true;
        break;
      }
    }
    if(!isDelimiter) {
      token.push_back(texto[i]);
    }
  }
}

//string to list
void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const {
  Output<list<string> >* os = new Output<list<string> >();
  Tokenizar(str, *os);
  tokens = os->getRepresentation();
  delete os;
}

//file to file (custom name)
bool Tokenizador::Tokenizar(const string& i, const string& f) const {
  ifstream input_file(i);
  if (!inputfile.is_open()) {
    //fallo al abrir input
    return false;
  }
  input_file.close();
  
  /*
     depente de string to output<T>
     file.write(output<string>.representacion)
  */
  ofstream output_file(f);
  if(!output_file.is_open()) {
    //fallo al abrir output
    return false;
  }
  output_file.close();
  return true;
}

//file to file (auto name)
bool Tokenizador::Tokenizar(const string& i) const {
  /*depende de Tokenizar custom name*/
  return false;
}

/*escribir todos los archivos de golpe vs escribir dentro del bucle para aprovechar ¿pipelining, parallel, idfk?*/
/*TokenizarIndex, recibe una list<string> de archivos*/
/*podria depender de tokenizar autoname si la implementacion es escribir cada fichero en cuanto se acabe*/

//files to files (from index)
bool Tokenizador::TokenizarListaFicheros(const string &i) const {
  /*depende de TokenizarIndex*/
  return false;
}

//files to files (from folder)
bool Tokenizador::TokenizarDirectorio(const string& i) const {
  /*crea un indice y lo envia a TokenizarIndex*/
  return false;
}



void Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters) {
  delimiters = nuevoDelimiters;
  normalizarDelimitadores(delimiters);
}

void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters) {
  delimiters += nuevoDelimiters;
  normalizarDelimitadores(delimiters);
}

string Tokenizador::DelimitadoresPalabra() const {
  return delimiters;
}

void Tokenizador::CasosEspeciales(const bool& nuevoCasosEspeciales) {
  casosEspeciales = nuevoCasosEspeciales;
}

bool Tokenizador::CasosEspeciales() const {
  return casosEspeciales;
}

void Tokenizador::PasarAminuscSinAcentos(const bool& nuevoPasarAminuscSinAcentos) {
  pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;
}

bool Tokenizador::PasarAminuscSinAcentos() {
  return pasarAminuscSinAcentos;
}

void Tokenizador::normalizarTexto(const string& str, string &salida) const {
  salida = str;
}

void Tokenizador::normalizarDelimitadores(string& delims){
  for (string::iterator it1=delims.begin(); it1 != delims.end(); ++it1) {
    for (string::iterator it2=delims.end(); it1 != it2; --it2) {
      if (*it1 == *it2) {
        delims.erase(it2);
      }
    }
  }
}
