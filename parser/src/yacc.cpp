#include <iostream>
#include <fstream>
#include <sstream>
#include "yacc.h"
#include "../../lexer/src/util.h"
using namespace std;
Yacc::Yacc() {

}
Yacc::~Yacc() {

}
bool Yacc::build_bnf_rule(string bnf_rule_path) {
  ifstream bnf_rule_in(bnf_rule_path);
  if (!bnf_rule_in.is_open()) {
    return false;
  }
  string s;
  // build the bnf rule
  while (getline(bnf_rule_in, s)) {
    if (s.length() == 0) continue; // empty line
    stringstream ss(s);
    char flag;
    ss >> flag;
    Yacc::BnfRule bnf_rule;
    if (flag == '%') {
      string raw_string;
      while (ss >> raw_string) {
        _lex_declaration_set.insert(raw_string);
      }
    } else if (flag == '$') {
      ss >> bnf_rule.head;
      string raw_string;
      // getline(ss, raw_string);
      ss >> raw_string;
      assert(raw_string == "->", " -> should after the head");
      int or_count = 0;
      vector<BnfRule::Symbol> tmp;
      bnf_rule.body.push_back(tmp);
      while (ss >> raw_string) {
        if (raw_string == "|") {
          or_count++;
          vector<BnfRule::Symbol> tmp;
          bnf_rule.body.push_back(tmp);
        } else {
          Yacc::BnfRule::Symbol symbol(_check_terminal(raw_string), raw_string);
          bnf_rule.body[or_count].push_back(symbol);
        }
      }
      _bnf_rules.push_back(bnf_rule);
    } else if (flag == '#') {
      continue; // comment
    }
  }
  bnf_rule_in.close();;
  // _bnf_rules 0 position has the heighest priority
  return true; 

}
bool Yacc::_check_terminal(string & input) {
  if (input.length() < 2) {
    return false;
  }
  if (input[0] == '\'' && input[0] == '\'') {
    input = input.substr(1, input.length() -2);
    return true;
  } else {
    return false;
  }
}
void Yacc::print() {
  cout << "--------------------------------------------------------------" << endl;
  cout << "_lex_declaration_set : ";
  for (auto it : _lex_declaration_set) {
    cout << it << " ";
  } cout << endl;
  cout << "_bnf_rules :" << endl;
  for (auto bnf_rule : _bnf_rules) {
    cout << bnf_rule.head << " -> ";
    int count = 0;
    for (auto partial_body : bnf_rule.body) {
      count++;
      for (auto symbol : partial_body) {
        cout << symbol.s << " ";
      }
      if (count < bnf_rule.body.size()) {
        cout << "| ";
      }
    }
    cout << endl;
  } 
  cout << "--------------------------------------------------------------" << endl;
}