#ifndef YACC_H
#define YACC_H
#include <string>
#include <vector>
#include <set>
#include <map>
#include "bnf_rule.h"
#include "slr.h"
using namespace std;
class SLR;

class Yacc {
friend class SLR;
public:
  Yacc();
  ~Yacc();
  void parse();
  bool read_bnf_rule(string bnf_rule_path);
  void print();
  void set_lexer(Lex &lexer);
private:
  bool _check_terminal(string & input);
  bool _check_type(string& input);
  string _check_escape(char ch);
  string _find_type(string input);
  string _recover_pattern(string input);
  vector<BnfRule> _bnf_rules;
  set<string> _lex_declaration_set;
  string _union_type_string;
  string _include_string;
  map<string, string> _type_map;
  void _fill_template();
  void _fill_yystype();
  void _fill_action();
  void _fill_main();
  void _replace_action_string(int rule_pos, ofstream& out);
  void _print_specific_bnf_rule(int rule_pos);
  void _build_SLR();
  Lex _lexer;
  SLR _slr;
};

#endif