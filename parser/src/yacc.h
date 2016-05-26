#ifndef YACC_H
#define YACC_H
#include <string>
#include <vector>
#include <set>
using namespace std;

class Yacc {
public:
  Yacc();
  ~Yacc();
  bool build_bnf_rule(string bnf_rule_path);
  void print();
private:
  struct BnfRule {
    struct Symbol {
      bool is_terminal;
      string s;
      Symbol(bool is_t, string tem) {
        is_terminal = is_t;
        s = tem;
      }
    };
    string head;
    vector< vector<BnfRule::Symbol> > body;
  };
  vector<BnfRule> _bnf_rules;
  set<string> _lex_declaration_set;

  bool _check_terminal(string & input);
};

#endif