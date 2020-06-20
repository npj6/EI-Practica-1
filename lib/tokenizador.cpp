#include "tokenizador.h"

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

//string to list
void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const {
  string::size_type lastPos, pos = 0;

  lastPos = str.find_first_not_of(delimiters, pos);
  pos = str.find_first_of(delimiters, lastPos);

  while(string::npos != pos || string::npos != lastPos) {
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, lastPos);
  }
}

//file to file (custom name)
bool Tokenizador::Tokenizar(const string& i, const string& f) const {
  return false;
}
//file to file (auto name)
bool Tokenizador::Tokenizar(const string& i) const {
  return false;
}

//files to files (from index)
bool Tokenizador::TokenizarListaFicheros(const string &i) const {
  return false;
}

//files to files (from folder)
bool Tokenizador::TokenizarDirectorio(const string& i) const {
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
