#ifndef LEX_H
#define LEX_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "dfa.h"
#include "util.h"
using namespace std;
class Lex {
public:
  struct Token {
    string type;
    string lexeme;
    int vint;
    string vstring;
    double vdouble;
    Token(string t, string l);
  };
  Lex();
  ~Lex();
  bool build_lex_rule(string lex_rule_path);
  bool read_code(string code_path);
  void set_max_token_size(int n);
  Token get_token();
private:
  struct LexRule {
    string type;
    string pattern;
    DFA    *dfa;
  };
  string _check_escape(string input);
  vector<LexRule> _vec;
  string _code;
  int _max_token_size; 
  int _line;
  int _pos;
};

#endif