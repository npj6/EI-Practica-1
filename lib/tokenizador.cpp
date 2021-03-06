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

void Tokenizador::encontradoDelimitador(unsigned pos) {
  idxCount[idx[pos]]++;
  while(pos != 0 && idxCount[idx[pos-1]] < idxCount[idx[pos]]) {
    unsigned i = idx[pos];
    idx[pos] = idx[pos-1];
    pos--;
    idx[pos] = i;
  }
}

//string to output
void Tokenizador::Tokenizar(const string& str, OutputIF& output) {
  string word; bool esDelim; unsigned estado = 0;

  for(const char &c : str) {
    esDelim = false;
    for(unsigned d=0; d<idx.size(); d++) { if (idxDelims[idx[d]]==c) { esDelim = true; encontradoDelimitador(d); break; } }

    if(esDelim) {
      estado = (this->*funcionesDelim[estado])(c, word, output);
    } else {
      estado = (this->*funcionesNoDelim[estado])(c, word, output);
    }
  }

  //añade la ultima palabra
  if(!especulativoSimb.empty()) {
    char c2 = ' ';
    (this->*funcionesDelim[estadoSimb])(c2, word, output);
    output.add(especulativoSimb);
    especulativoSimb.clear();
  }
  if(!word.empty() || !especulativoNum.empty()) {
    if (!especulativoNum.empty()) {
      if(!word.empty()) { word.push_back(','); }
      word += especulativoNum;
      especulativoNum.clear();
    }
    output.add(prefijoNum+word);
    prefijoNum.clear();
  }
}

//string to list
void Tokenizador::Tokenizar(const string& str, list<string>& tokens) {
  tokens.erase(tokens.begin(), tokens.end());
  OutputList output(tokens);
  Tokenizar(str, output);
}

//file to file (custom name)
bool Tokenizador::Tokenizar(const string& i, const string& f) {
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
bool Tokenizador::Tokenizar(const string& i) {
  return Tokenizar(i, i+".tk");
}

//files to files (from index)
bool Tokenizador::TokenizarListaFicheros(const string &i) {
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
bool Tokenizador::TokenizarDirectorio(const string& i) {
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
  if (casosEspeciales) {
    funcionesDelim [0] = &Tokenizador::estado0_delim;
    funcionesNoDelim[0] = &Tokenizador::estado0_noDelim;
  } else {
    funcionesDelim [0] = &Tokenizador::estado0_delimNoCasosEspeciales;
    funcionesNoDelim[0] = &Tokenizador::estado0_noDelimNoCasosEspeciales;
  }
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
          //si hay espacio lo encuentra
          if(idxDelims[idx[i]] == ' ') {
            place = i;
          }
        }

        //si lo ha encontrado, lo borra
        if(place != -1) {
          for(unsigned i=0; i<idx.size(); i++) {
            if(idx[place] < idx[i]) {
              idx[i]--;
            }
          }

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
    addChar = conversion;
  } else {
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


unsigned Tokenizador::estado0_delimNoCasosEspeciales(const char& c, string& word, OutputIF& output) {
    if(!word.empty()) { output.add(word); word.clear(); }
    return 0;
}
unsigned Tokenizador::estado0_noDelimNoCasosEspeciales(const char& c, string& word, OutputIF& output) {
  word.push_back(addChar[128 + (int) c]);
  return 0;
}

unsigned Tokenizador::estado0_delim(const char& c, string& word, OutputIF& output) {
  //hack para hacer funcionar $%
  if(!especulativoSimb.empty()) {
    char c2 = ' ';
    (this->*funcionesDelim[estadoSimb])(c2, word, output);
    output.add(especulativoSimb);
    especulativoSimb.clear();
  }
  if (c==':' && (word == "http" || word == "https" || word == "ftp")) {
    return 3;
  }
  if (c=='.') {
    if(word.empty()) { prefijoNum = "0."; return 6; } else { return 4; }
  }
  if (c==',' && word.empty()) { prefijoNum = "0,"; return 7; }
  if (c=='-' && !word.empty()) { return 1; }
  if(!word.empty()) { output.add(word); word.clear(); return 0; }
  return 0;
}
unsigned Tokenizador::estado0_noDelim(const char& c, string& word, OutputIF& output) {
  //hack para hacer funcionar $%
  if(!especulativoSimb.empty()) {
    (this->*funcionesNoDelim[estadoSimb])(especulativoSimb[0], word, output);
    especulativoSimb.clear();
  }
  bool num = '0' <= c && c <= '9';
  if(num && word.empty()) { activoNum = true; }
  word.push_back(addChar[128 + (int) c]);
  if(!num) { activoNum = false; }
  if (c==':' && (word == "http:" || word == "https:" || word == "ftp:")) {
    return 3;
  }
  if (num && activoNum) {
    return 5;
  }
  return 0;
}

unsigned Tokenizador::estado1_delim(const char& c, string& word, OutputIF& output) {
  if(!word.empty()) { output.add(word); word.clear(); }
  return 0;
}
unsigned Tokenizador::estado1_noDelim(const char& c, string& word, OutputIF& output) {
  word.push_back('-'); word.push_back(addChar[128 + (int) c]);
  return 0;
}

unsigned Tokenizador::estado2_delim(const char& c, string& word, OutputIF& output) {
  string URLignoredDelims = "_:/.?&-=#@";
  if (URLignoredDelims.find(c) != -1) {
    word.push_back(addChar[128 + (int) c]);
    return 2;
  }
  if(!word.empty()) { output.add(word); word.clear(); }
  return 0;
}
unsigned Tokenizador::estado2_noDelim(const char& c, string& word, OutputIF& output) {
  word.push_back(addChar[128 + (int) c]);
  return 2;
}

unsigned Tokenizador::estado3_delim(const char& c, string& word, OutputIF& output) {
  string URLignoredDelims = "_:/.?&-=#@";
  if (URLignoredDelims.find(c) != -1) {
    if(word[word.size()-1] != ':') word.push_back(':'); //el if para no poner doble si no es delim
    word.push_back(addChar[128 + (int) c]);
    return 2;
  }
  if(!word.empty()) { output.add(word); word.clear(); }
  return 0;
}
unsigned Tokenizador::estado3_noDelim(const char& c, string& word, OutputIF& output) {
  if(word[word.size()-1] != ':') word.push_back(':'); //el if para no poner doble si no es delim
  word.push_back(addChar[128 + (int) c]);
  return 2;
}

unsigned Tokenizador::estado4_delim(const char& c, string& word, OutputIF& output) {
  if(!word.empty()) { output.add(word); word.clear(); }
  return 0;
}
unsigned Tokenizador::estado4_noDelim(const char& c, string& word, OutputIF& output) {
  word.push_back('.'); word.push_back(addChar[128 + (int) c]);
  return 0;
}

unsigned Tokenizador::estado5_delim(const char& c, string& word, OutputIF& output) {
  if(c=='.') { return 6; }
  if(c==',') { return 7; }
  activoNum = false;
  if(!especulativoSimb.empty()) {
    if(!word.empty()) { output.add(word); word.clear(); return 0; }
    return 0; //evita bucle infinito
  }
  return estado0_delim(c, word, output);
}
unsigned Tokenizador::estado5_noDelim(const char& c, string& word, OutputIF& output) {
    if((c=='$' || c=='%') && especulativoSimb.empty()) {
      especulativoSimb.push_back(c);
      estadoSimb = 5;
      return 0;
    }
    if(!especulativoSimb.empty()) {
      word.push_back(c);
      return 0; //evita bucle infinito
    }
    return estado0_noDelim(c, word, output);
    /*
      reutiliza el codigo de 0
      si hay un numero, el estado vuelve a 5
      si no, se hace lo que haría el estado 0
    */
}

unsigned Tokenizador::estado6_delim(const char& c, string& word, OutputIF& output) {
  //numero
  if(!word.empty()) { output.add(prefijoNum+word); word.clear(); }
  prefijoNum.clear();
  activoNum = false;
  return estado0_delim(c, word, output); //2..3 tiene que leer desde 0 el segundo .
}
unsigned Tokenizador::estado6_noDelim(const char& c, string& word, OutputIF& output) {
  if ('0' <= c && c <= '9') {
    if (!word.empty()) { word.push_back('.'); }
    word.push_back(addChar[128 + (int) c]);
    return 8;
  } else if (word.empty()) {
    //acronimo empieza por .
    word.push_back(addChar[128 + (int) c]);
    prefijoNum.clear();
    activoNum = false;
    return 0;
  } else {
    //acronimo
    word.push_back('.');
    word.push_back(addChar[128 + (int) c]);
    prefijoNum.clear();
    activoNum = false;
    return 0;
  }
}

unsigned Tokenizador::estado7_delim(const char& c, string& word, OutputIF& output) {
  //acepta especulacion
  if(!especulativoNum.empty()) {
      if(!word.empty()) { word.push_back(','); }
      word += especulativoNum;
      especulativoNum.clear();
    }
  //numero
  if(!word.empty()) { output.add(prefijoNum+word); word.clear(); }
  prefijoNum.clear();
  activoNum = false;
  return estado0_delim(c, word, output); //,2..3 tiene que leer desde 0 el segundo .
}
unsigned Tokenizador::estado7_noDelim(const char& c, string& word, OutputIF& output) {
  if ('0' <= c && c <= '9') {
    //acepta especulacion
    if(!especulativoNum.empty()) {
      if(!word.empty()) { word.push_back(','); }
      word += especulativoNum;
      especulativoNum.clear();
    }
    //inicia especulacion
    especulativoNum.push_back(addChar[128 + (int) c]);
    return 9;
  } else {
    //numero
    if(!word.empty()) { output.add(prefijoNum+word); word.clear(); }
    prefijoNum.clear();
    //acronimo
    word = especulativoNum;
    word.push_back(addChar[128 + (int) c]);
    especulativoNum.clear();
    activoNum = false;
    return 0;
  }
}

unsigned Tokenizador::estado8_delim(const char& c, string& word, OutputIF& output) {
  if(c=='.') { return 6; }
  if(c==',') { return 7; }
  //numero
  if(!word.empty()) { output.add(prefijoNum+word); word.clear(); }
  prefijoNum.clear();
  activoNum = false;
  return 0; //8@2? no usa estado0
}
unsigned Tokenizador::estado8_noDelim(const char& c, string& word, OutputIF& output) {
  if('0' <= c && c <= '9') {
    word.push_back(addChar[128 + (int) c]);
    return 8;
  } else if((c=='$' || c=='%') && especulativoSimb.empty()) {
    especulativoSimb.push_back(c);
    estadoSimb = 8;
    return 0;
  } else {
    //acronimo
    word.push_back(addChar[128 + (int) c]);
    prefijoNum.clear();
    activoNum = false;
    return 0;
  }
}

unsigned Tokenizador::estado9_delim(const char& c, string& word, OutputIF& output) {
  if(c=='.') { return 10; }
  if(c==',') { return 7; }
  //acepta especulacion
  if(!especulativoNum.empty()) {
    if(!word.empty()) { word.push_back(','); }
    word += especulativoNum;
    especulativoNum.clear();
  }
  //numero
  if(!word.empty()) { output.add(prefijoNum+word); word.clear(); }
  prefijoNum.clear();
  activoNum = false;
  return 0; //,2@3? no usa estado0
}
unsigned Tokenizador::estado9_noDelim(const char& c, string& word, OutputIF& output) {
  if('0' <= c && c <= '9') {
    especulativoNum.push_back(addChar[128 + (int) c]);
    return 9;
  } else if((c=='$' || c=='%') && especulativoSimb.empty()) {
    especulativoSimb.push_back(c);
    estadoSimb = 9;
    return 0;
  } else {
    //numero
    if(!word.empty()) { output.add(prefijoNum+word); word.clear(); }
    prefijoNum.clear();
    //acronimo
    word = especulativoNum;
    word.push_back(addChar[128 + (int) c]);
    especulativoNum.clear();
    activoNum = false;
    return 0;
  }
}

unsigned Tokenizador::estado10_delim(const char& c, string& word, OutputIF& output) {
  return estado7_delim(c, word, output);
}
unsigned Tokenizador::estado10_noDelim(const char& c, string& word, OutputIF& output) {
  if ('0' <= c && c <= '9') {
    especulativoNum.push_back('.');
    especulativoNum.push_back(addChar[128 + (int) c]);
    return 9;
  } else {
    especulativoNum.push_back('.');
    return estado7_noDelim(c, word, output);
  }
}
