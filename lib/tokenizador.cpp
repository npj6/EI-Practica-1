#include "tokenizador.h"

using namespace std;

ostream& operator<<(ostream& os, const Tokenizador& tokenizador) {
  os << "DELIMITADORES: " << tokenizador.delimiters
     << " TRATA CASOS ESPECIALES: " << tokenizador.casosEspeciales
     << " PASAR A MINUSCULAS Y SIN ACENTOS: " << tokenizador.pasarAminuscSinAcentos;
  return os;
}

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos) {
  DelimitadoresPalabra(delimitadoresPalabra);
  CasosEspeciales(kcasosEspeciales);
  PasarAminuscSinAcentos(minuscSinAcentos);
  rellenarConversion();
}

Tokenizador::Tokenizador(const Tokenizador& tokenizador) {
  DelimitadoresPalabra(tokenizador.delimiters);
  CasosEspeciales(tokenizador.casosEspeciales);
  PasarAminuscSinAcentos(tokenizador.pasarAminuscSinAcentos);
  rellenarConversion();
}

Tokenizador::Tokenizador() {
  DelimitadoresPalabra(",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@");
  CasosEspeciales(true);
  PasarAminuscSinAcentos(false);
  rellenarConversion();
}

Tokenizador::~Tokenizador() {
  delimiters = "";
}

Tokenizador& Tokenizador::operator=(const Tokenizador& tokenizador) {
  DelimitadoresPalabra(tokenizador.delimiters);
  CasosEspeciales(tokenizador.casosEspeciales);
  PasarAminuscSinAcentos(tokenizador.pasarAminuscSinAcentos);
  rellenarConversion();
  return *this;
}


OutputList::OutputList(list<string> &o) : output(o) { }
void OutputList::add(const string &word) { output.push_back(word); }

OutputString::OutputString(string &o) : output(o) { }
void OutputString::add(const string &word) { output.append(word); output.push_back('\n'); }

/*MAIN FUNCTIONS*/

/*
void Tokenizador::addCharToWordBasic(string &word, const char &c) const {
  word.push_back(c);
}

void Tokenizador::addCharToWordAccentsLower(string &word, const char &c) const {
    word.push_back(conversion[128 + (int) c]);
}
*/

void Tokenizador::rellenarConversion(void) {
  char newC;
  for(int c=-128; c<128; c++) {
    switch(c) {
      case (char) 192: case (char) 193: case (char) 224: case (char) 225:
        newC = 'a'; break;
      case (char) 200: case (char) 201: case (char) 232: case (char) 233:
        newC = 'e'; break;
      case (char) 204: case (char) 205: case (char) 236: case (char) 237:
        newC = 'i'; break;
      case (char) 210: case (char) 211: case (char) 242: case (char) 243:
        newC = 'o'; break;
      case (char) 217: case (char) 218: case (char) 249: case (char) 250:
        newC = 'u'; break;
      default:
        if ('A' <= c && c <= 'Z' || (char) 192 <= c && c <= (char) 222) {
          newC = c + ('a' - 'A');
        } else {
          newC = c;
        }
    }
    conversion[c+128] = newC;
    normal[c+128] = c;
  }
}



//string to output
void Tokenizador::Tokenizar(const string& str, OutputIF& output) const {
  //IDEA: usar idx para reordenar los separadores segun aparecen
  string word; bool esDelim;

  for(const char &c : str) {
    esDelim = false;
    for(const char &d : idxDelims) { if (c==d) { esDelim = true; break; } }
    if(esDelim) {
      if(0 < word.size()) { output.add(word); word.clear(); }
    } else {
      //(this->*addCharToWord)(word, c);
      word.push_back(addChar[128 + (int) c]);
    }
  }

  //añade la ultima palabra
  if(0 < word.size()) {
    output.add(word);
  }
}

//string to list
void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const {
  tokens.erase(tokens.begin(), tokens.end());
  OutputList output(tokens);
  Tokenizar(str, output);
}

//file to file (custom name)
bool Tokenizador::Tokenizar(const string& i, const string& f) const {
  ifstream input; ofstream output;
  string cadena;

  string salida;
  OutputString o(salida);

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
        Tokenizar(cadena, o);
      }
    }
  }
  input.close();

  output.open(f.c_str());
  output << o.output;
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

  idxDelims.clear();
  idx.clear();
  idxCount.clear();

  for(unsigned i=0; i<delimiters.length(); i++) {
    idxDelims.push_back(delimiters[i]);
    idx.push_back(i);
    idxCount.push_back(0);
  }

  comprobarDelimitadoresCasosEspeciales();
}

void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters) {
  DelimitadoresPalabra(delimiters + nuevoDelimiters);
}

string Tokenizador::DelimitadoresPalabra() const {
  return delimiters;
}

void Tokenizador::CasosEspeciales(const bool& nuevoCasosEspeciales) {
  casosEspeciales = nuevoCasosEspeciales;
  comprobarDelimitadoresCasosEspeciales();
}


void Tokenizador::comprobarDelimitadoresCasosEspeciales(void) {
    //si espacio no es un delimitador
    if(delimiters.find(" ") == string::npos) {
      //al activar casos especiales lo añade, al desactivarlos lo quita
      if(casosEspeciales) {
        if (find(idxDelims.begin(), idxDelims.end(), ' ')==idxDelims.end()) {
          idxDelims.push_back(' ');
          idx.push_back(idx.size());
          idxCount.push_back(0);
        }
      } else {
        int place = -1;
        for(unsigned i=0; i<idx.size(); i++) {
          //si ha encontrado el espacio, mueve los siguientes hacia atras
          if(place != -1) {
            idx[i]--;
          }

          //si hay espacio lo encuentra
          if(idxDelims[idx[i]] == ' ') {
            place = i;
          }
        }

        //si lo ha encontrado, lo borra
        if(place != -1) {
          idxDelims.erase(idxDelims.begin()+idx[place]);
          idxCount.erase(idxCount.begin()+idx[place]);
          idx.erase(idx.begin()+place);
        }
      }
    }
}

bool Tokenizador::CasosEspeciales() const {
  return casosEspeciales;
}

void Tokenizador::PasarAminuscSinAcentos(const bool& nuevoPasarAminuscSinAcentos) {
  pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;
  if(pasarAminuscSinAcentos) {
    //addCharToWord = &Tokenizador::addCharToWordAccentsLower;
    addChar = conversion;
  } else {
    //addCharToWord = &Tokenizador::addCharToWordBasic;
    addChar = normal;
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
