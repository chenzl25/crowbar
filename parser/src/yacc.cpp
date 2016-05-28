#include <iostream>
#include <fstream>
#include <sstream>
#include "yacc.h"
#include "bnf_rule.h"
#include "../../lexer/src/util.h"
using namespace std;
Yacc::Yacc() {

}
Yacc::~Yacc() {

}

bool Yacc::read_bnf_rule(string bnf_rule_path) {
  ifstream bnf_rule_in(bnf_rule_path);
  if (!bnf_rule_in.is_open()) {
    return false;
  }
  string s;
  // build the bnf rule
  while (getline(bnf_rule_in, s)) {
    if (s.length() == 0) continue; // empty line
    stringstream ss(s);
    string flag;
    ss >> flag;
    if (flag == "%token") {
      string raw_string;
      string type_string;
      ss >> raw_string;
      if (_check_type(raw_string)) {
        type_string = raw_string;
      } else {
        _lex_declaration_set.insert(raw_string);
      }
      while (ss >> raw_string) {
        _lex_declaration_set.insert(raw_string);
        if (!type_string.empty()) {
          _type_map[raw_string] = type_string;
        }
      }
      continue;
    } else if (flag == "%union") {
      ss >> flag;
      assert(flag == "{", "%union without {");
      while (getline(bnf_rule_in, s)) {
        stringstream ss(s);
        ss >> flag;
        if (flag == "}") {
          break;
        }
        _union_type_string += s + '\n';
      }
      continue;
    } else if (flag == "%type") {
      string raw_string;
      ss >> raw_string;
      if (_check_type(raw_string)) {
        string type_string = raw_string;
        while (ss >> raw_string) {
          _type_map[raw_string] = type_string;
        }
      } else {
        error("not type in %type <...>");
      }
      continue;
    } else if (flag == "$") {
      string head_value;
      ss >> head_value;

      string raw_string;
      // getline(ss, raw_string);
      ss >> raw_string;
      assert(raw_string == ":", " : should after the head");
      int or_count = 0;
      vector< BnfRule > bnf_rule_tem_set;
      BnfRule tmp;
      bnf_rule_tem_set.push_back(tmp);
      // first pass
      while (ss >> raw_string) {
        BnfRule::Symbol symbol(_check_terminal(raw_string), raw_string, _find_type(raw_string));
        bnf_rule_tem_set[or_count].body.push_back(symbol);
      }
      while (getline(bnf_rule_in, s)) {
        stringstream ss(s);
        ss >> flag;
        if (flag == ";") break;
        else if (flag == "{") {
          string action_string;
          while (getline(bnf_rule_in, s)) {
            stringstream ss(s);
            ss >> flag;
            if (flag == "}") break;
            action_string += s + '\n';
          }
          bnf_rule_tem_set[or_count].action_string = action_string; 
          continue; 
        }
        else assert(flag == "|", "use | to separate the production body");
        or_count++;
        BnfRule tmp;
        bnf_rule_tem_set.push_back(tmp);
        while (ss >> raw_string) {
          BnfRule::Symbol symbol(_check_terminal(raw_string), raw_string, _find_type(raw_string));
          bnf_rule_tem_set[or_count].body.push_back(symbol);
        }
      }

      for (auto bnf_rule : bnf_rule_tem_set) {
        bnf_rule.head.value = head_value;
        bnf_rule.head.is_terminal = false;
        bnf_rule.head.type = _find_type(head_value);
        _bnf_rules.push_back(bnf_rule);
      }
    } else if (flag == "#") {
      continue; // comment
    }
  }
  bnf_rule_in.close();
  // _bnf_rules 0 position has the heighest priority
  print();
  _build_SLR();
  return true; 
}
void Yacc::_build_SLR() {
  _slr.build_from_bnf_rules(_bnf_rules);
}
void Yacc::print() {
  cout << "--------------------------------------------------------------" << endl;
  cout << "_lex_declaration_set : ";
  for (auto it : _lex_declaration_set) {
    cout << it << " ";
  } cout << endl;
  cout << "_bnf_rules :" << endl;
  for (auto bnf_rule : _bnf_rules) {
    cout << bnf_rule.head.value << " -> ";
    if (bnf_rule.body.size() == 0) {
      cout << "ε";
    }
    for (auto symbol : bnf_rule.body) {
      cout << symbol.value << " ";
    }
    cout << endl;
  } 
  cout << "--------------------------------------------------------------" << endl;
}
bool Yacc::_check_terminal(string & input) {
  assert(input.length() > 0, "the input for _check_terminal size should > 0");
  if (input[0] == '\'' && input[input.length()-1] == '\'') {
    input = input.substr(1, input.length() -2);
    return true;
  }
  for (auto s: _lex_declaration_set) {
    if (s == input) {
      return true;
    }
  }
  return false;
}
bool Yacc::_check_type(string& input) {
  assert(input.length() > 0, "the input for _check_type size should > 0");
  if (input[0] == '<' && input[input.length()-1] == '>') {
    input = input.substr(1, input.length() -2);
    return true;
  }
  return false;
}
string Yacc::_find_type(string input) {
  if (_type_map.count(input)) {
    return _type_map[input];
  } else {
    return ""; // non-type
  }
}
void Yacc::parse(Lex &lexer) {
  _slr.parse(lexer);
}
