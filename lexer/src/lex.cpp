#include "lex.h"
Lex::Lex() {
  _max_token_size = 50;
  _line = 1;
  _pos = 0;
  _string_type = "string";
  _string_distincter = '\"';
}
Lex::~Lex() {
  for (auto it : _lex_rules) {
    delete it.dfa;
  }
}
void Lex::set_max_token_size(int n) {
  _max_token_size = n;
}
string Lex::_check_escape(string input) {
  string result;
  for (int i = 0; i < input.size(); i++) {
    switch(input[i]) {
      case '(':
      case ')':
      case '{':
      case '}':
      case '\\':
      case '+':
      case '?':
      case '[':
      case ']':
      case '*':
      case '|':
      case '.':
      case '\"':
      case '\'':
      case '-':
      case '^':
      case '$':
        result += "\\";
        break;
      default:
        break;
    }
    result += input[i];
  }
  return result;
}

bool Lex::read_lex_rule (string lex_rule_path) {
  ifstream lex_rule_in(lex_rule_path);
  if (!lex_rule_in.is_open()) {
    return false;
  }
  string s;
  // build the lex rule
  while (getline(lex_rule_in, s)) {
    if (s.length() == 0) continue;
    stringstream ss(s);
    char flag;
    ss >> flag;
    Lex::LexRule lex_rule;
    if (flag == '%') {
      ss >> lex_rule.type;
      lex_rule.pattern = "(";
      string raw_string;
      while (ss >> raw_string) {
        raw_string = _check_escape(raw_string);
        lex_rule.pattern += raw_string + "|";
      }
      lex_rule.pattern[lex_rule.pattern.length()-1] = ')';
      lex_rule.dfa = new DFA(lex_rule.pattern);
    } else if (flag == '$') {
      ss >> lex_rule.type;
      string re_string;
      getline(ss, re_string);
      // ss >> re_string;
      re_string = trim(re_string);
      lex_rule.pattern += re_string;
      lex_rule.dfa = new DFA(lex_rule.pattern);
    } else if (flag == '^') {
      ss >> _string_type >> _string_distincter;
      continue;
    } else if (flag == '#') {
      continue;
    }
    _lex_rules.push_back(lex_rule);
  }
  lex_rule_in.close();;
  // _lex_rules 0 position has the heighest priority
  return true; 
}

bool Lex::read_code(string code_path) {
  ifstream code_in(code_path);
  if (!code_in.is_open()) {
    return false;
  }
  string s;
  while (getline(code_in, s)) {
    _code += s + '\n';
  }
  code_in.close();
  return true;
}

Lex::Token Lex::get_token() {
  string result;
  int not_empty_pos = _pos;
  for (int i = _pos; i < _code.length(); i++) {
    if (_code[i] == ' ' || _code[i] == '\t') {
      not_empty_pos++;
    } else if (_code[i] == '\n') {
      _line++;
      not_empty_pos++;
    } else {
      break;
    }
  }
  _pos = not_empty_pos;
  if (_pos >= _code.length()) {
    Lex::Token token("EOF", "$");
    return token;
  }
  if(_code[_pos] == _string_distincter) { // enter string mode
    int old_pos = _pos;
    _pos++; // skip the first _string_distincter
    while (!(_code[_pos] == _string_distincter && _code[_pos-1] != '\\')) {
      _pos++;
    }
    _pos++; // skip the last _string_distincter
    Lex::Token token(_string_type, _string_distincter + _code.substr(old_pos, _pos - old_pos) + _string_distincter);
    token.vstring = _code.substr(old_pos, _pos - old_pos);
    return token;
  }
  string sub_string = _code.substr(_pos, (_code.length()-_pos < _max_token_size?_code.length()-_pos:_max_token_size) );
  for (int i = 0; i < _lex_rules.size(); i++) {
    if (_lex_rules[i].dfa->match_from_head(sub_string, result)) {
      _pos += result.length();
      Lex::Token token(_lex_rules[i].type, result);
      return token;
    }
  }
  error(_line , "bad lexeme ");
}
Lex::Token::Token(string t, string l) {
  type = t;
  lexeme = l;
  if (type == "identifier") {

  } else if (type == "number") {
    stringstream ss(lexeme);
    ss >> vint;
  } else if (type == "factor") {
    stringstream ss(lexeme);
    ss >> vdouble;
  } else if (type == "type") {

  } else if (type == "operators") {

  } else if (type == "reserve_words") {

  }
}
void Lex::print() {
  cout << "--------------------------------------------------------------" << endl;
  for (auto lex_rule: _lex_rules) {
    cout << lex_rule.type << " : " << lex_rule.pattern << endl;
  }
  cout << "--------------------------------------------------------------" << endl;
}
