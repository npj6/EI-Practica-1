
template<>
  void Output<string>::addToken(string token) {
    output += "\n" + token;
  }


template<>
  void Output<list<string> >::addToken(string token) {
    output.push_back(token);
  }
