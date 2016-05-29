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
  struct LexRule {
    string type;
    string pattern;
    DFA    *dfa;
  };
  Lex();
  Lex(const Lex& another);
  Lex& operator = (const Lex& another);
  ~Lex();
  bool read_lex_rule(string lex_rule_path);
  void build_from_lex_rules(vector<LexRule> lex_rules);
  bool read_code(string code_path);
  void set_max_token_size(int n);
  void set_string_type(string string_type);
  void set_string_distincter(char string_distincter);
  string get_string_type();
  char get_string_distincter();
  vector<LexRule> get_lex_rules();
  Token get_token();
  void print();
private:
  string _check_escape(string input);
  vector<LexRule> _lex_rules;
  string _code;
  int _max_token_size; 
  int _line;
  int _pos;
  string _string_type;
  char   _string_distincter;
};

#endif