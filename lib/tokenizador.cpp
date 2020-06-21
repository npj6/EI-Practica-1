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
  PasarAminuscSinAcentos(minuscSinAcentos);
}

Tokenizador::Tokenizador(const Tokenizador& tokenizador) {
  delimiters = tokenizador.delimiters;
  casosEspeciales = tokenizador.casosEspeciales;
  PasarAminuscSinAcentos(tokenizador.pasarAminuscSinAcentos);
}

Tokenizador::Tokenizador() {
  delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
  casosEspeciales = true;
  PasarAminuscSinAcentos(false);
}

Tokenizador::~Tokenizador() {
  delimiters = "";
}

Tokenizador& Tokenizador::operator=(const Tokenizador& tokenizador) {
  delimiters = tokenizador.delimiters;
  casosEspeciales = tokenizador.casosEspeciales;
  PasarAminuscSinAcentos(tokenizador.pasarAminuscSinAcentos);
  return *this;
}


/*MAIN FUNCTIONS*/

void Tokenizador::addCharToWordBasic(string &word, const char &c) const {
  word.push_back(c);
}

void Tokenizador::addCharToWordAccentsLower(string &word, const char &c) const {
  char newC;
  switch(c) {
    case (char) 192: case (char) 193: case (char) 224: case (char) 225:
      newC = 'a';
    case (char) 200: case (char) 201: case (char) 232: case (char) 233:
      newC = 'e';
    case (char) 204: case (char) 205: case (char) 236: case (char) 237:
      newC = 'i';
    case (char) 210: case (char) 211: case (char) 242: case (char) 243:
      newC = 'o';
    case (char) 217: case (char) 218: case (char) 249: case (char) 250:
      newC = 'u';
    default:
      if ('A' <= c && c <= 'Z' || (char) 192 <= c && c <= (char) 222) {
        newC = c + ('a' - 'A');
      } else {
        newC = c;
      }
    }
    word.push_back(newC);
}

//string to list
void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const {
  tokens.erase(tokens.begin(), tokens.end());
  //IDEA: usar idx para reordenar los separadores segun aparecen
  string word;
  const char *pointerToTheEnd = &*--str.end();
  for(const char &c : str) {
    bool esDelim = false;
    for(const char &d : delimiters) {
      //delimitador encontrado
      if (c==d) {
        esDelim = true;
        break;
      }
    }
    if(!esDelim) {
      //caracter encontrado
      (this->*addCharToWord)(word, c);
    }
    if(esDelim || &c == pointerToTheEnd) {
      if(0 < word.size()) {
        //guarda palabra
        tokens.push_back(word);
        //borra string
        word.clear();
      }
    }
  }
}

//file to file (custom name)
bool Tokenizador::Tokenizar(const string& i, const string& f) const {
  ifstream input; ofstream output;
  string cadena;
  list<string> tokens;

  struct stat dir;
  int err=stat(i.c_str(), &dir);
  if(S_ISDIR(dir.st_mode)) {
    cerr << "ERROR: " << i << " es una carpeta" << endl;
    return false;
  }
  input.open(i.c_str());
  if(!input) {
    cerr << "ERROR: No existe el archivo " << i << endl;
    return false;
  } else {
    while(!input.eof()) {
      cadena="";
      getline(input, cadena);
      if(cadena.length() != 0) {
        list<string> tokensTEMP;
        Tokenizar(cadena, tokensTEMP);
        tokens.splice(tokens.end(), tokensTEMP);
      }
    }
  }
  input.close();

  output.open(f.c_str());
  for(auto itS=tokens.begin(); itS!=tokens.end(); itS++) {
    output << (*itS) << endl;
  }
  output.close();
  return true;
}
//file to file (auto name)
bool Tokenizador::Tokenizar(const string& i) const {
  return Tokenizar(i, i+".tk");
}

//files to files (from index)
bool Tokenizador::TokenizarListaFicheros(const string &i) const {
  ifstream input;
  string cadena;
  bool output = true;

  struct stat dir;
  int err=stat(i.c_str(), &dir);
  if(S_ISDIR(dir.st_mode)) {
    cerr << "ERROR: " << i << " es una carpeta" << endl;
    return false;
  }
  input.open(i.c_str());
  if(!input) {
    cerr << "ERROR: No existe el archivo: " << i << endl;
    return false;
  } else {
    while(!input.eof()) {
      cadena="";
      getline(input, cadena);
      if(cadena.length() != 0) {
        output = Tokenizar(cadena) || output;
      }
    }
  }
  input.close();

  return output;
}

//files to files (from folder)
bool Tokenizador::TokenizarDirectorio(const string& i) const {
  struct stat dir;
  int err=stat(i.c_str(), &dir);
  if(err==-1 || !S_ISDIR(dir.st_mode)) {
    return false;
  } else {
    //-follow is deprecated according to SS64
    string cmd = "find -L "+i+" -type f |sort > .lista_fich";
    system(cmd.c_str());
    bool output = TokenizarListaFicheros(".lista_fich");
    system("rm .lista_fich");
    return output;
  }
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
  if(pasarAminuscSinAcentos) {
    addCharToWord = &Tokenizador::addCharToWordAccentsLower;
  } else {
    addCharToWord = &Tokenizador::addCharToWordBasic;
  }
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
