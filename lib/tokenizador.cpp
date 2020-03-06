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
string to output<T> (devolver output)
*/

//string to list
void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const {
 /*
    depende de string to ouput<T>
    tokens = output<list>.representacion
 */
}

//file to file (custom name)
bool Tokenizador::Tokenizar(const string& i, const string& f) const {

  /* 
     depente de string to output<T>
     file.write(output<string>.representacion)
  */
  return false;
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

void Tokenizador::normalizarDelimitadores(string& delims){
  for (string::iterator it1=delims.begin(); it1 != delims.end(); ++it1) {
    for (string::iterator it2=delims.end(); it1 != it2; --it2) {
      if (*it1 == *it2) {
        delims.erase(it2);
      }
    }
  }
}
