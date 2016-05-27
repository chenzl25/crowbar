#ifndef YACC_H
#define YACC_H
#include <string>
#include <vector>
#include <set>
#include "bnf_rule.h"
#include "slr.h"
using namespace std;
class SLR;

class Yacc {
friend class SLR;
public:
  Yacc();
  ~Yacc();
  void parse(Lex &lexer);
  bool read_bnf_rule(string bnf_rule_path);
  void print();
private:
  bool _check_terminal(string & input);
  vector<BnfRule> _bnf_rules;
  set<string> _lex_declaration_set;
  void _build_SLR();
  SLR _slr;
};

#endif