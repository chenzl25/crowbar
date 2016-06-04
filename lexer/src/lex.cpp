#include "lex.h"
Lex::Lex() {
  _max_token_size = 50;
  _line = 1;
  _pos = 0;
  _string_type = "string";
  _string_distincter = '\"';
}
Lex::Lex(const Lex& another) {
  _max_token_size = another._max_token_size;
  _line = another._line;
  _pos = another._line;
  _string_type = another._string_type;
  _string_distincter = another._string_distincter;
  for (auto lex_rule : another._lex_rules) {
    _lex_rules.push_back(lex_rule);
    _lex_rules[_lex_rules.size()-1].dfa = new DFA(lex_rule.pattern);
  }
}
Lex& Lex::operator = (const Lex& another) {
  if (this == &another) {
    return *this;
  }
  _max_token_size = another._max_token_size;
  _line = another._line;
  _pos = another._line;
  _string_type = another._string_type;
  _string_distincter = another._string_distincter;
  for (auto lex_rule : another._lex_rules) {
    _lex_rules.push_back(lex_rule);
    _lex_rules[_lex_rules.size()-1].dfa = new DFA(lex_rule.pattern);
  }
  return *this;
}
Lex::~Lex() {
  for (auto it : _lex_rules) {
    if (it.dfa != NULL) {
      delete it.dfa;
    }
  }
}
void Lex::set_max_token_size(int n) {
  _max_token_size = n;
}
void Lex::set_string_type(string string_type) {
  _string_type = string_type;
}
void Lex::set_string_distincter(char string_distincter) {
  _string_distincter = string_distincter;
}
string Lex::get_string_type() {
  return _string_type;
}
char Lex::get_string_distincter() {
  return _string_distincter;
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
void Lex::build_from_lex_rules(vector<LexRule> lex_rules) {
  _lex_rules = lex_rules;
  for (int i = 0; i < _lex_rules.size(); i++) {
    assert(_lex_rules[i].dfa == NULL, "build_from_lex_rules: the dfa should be NULL");
    _lex_rules[i].dfa = new DFA(_lex_rules[i].pattern);
  }
}
bool Lex::read_code(string code_path) {
  ifstream code_in(code_path);
  if (!code_in.is_open()) {
    return false;
  }
  _code.clear();
  _pos = 0;
  _line = 1;
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
      // do nothing
    } else if (_code[i] == '\n') {
      _line++;
    } else if (_code[i] == '#') {
      while(i < _code.length() && _code[i] != '\n') {
        i++;
      } // now i point to \n
      _line++;
    } else {
      break;
    }
    not_empty_pos = i+1;
  }
  _pos = not_empty_pos;
  if (_pos >= _code.length()) {
    Lex::Token token("EOF", "$", _line);
    return token;
  }
  if(_code[_pos] == _string_distincter) { // enter string mode
    int old_pos = _pos;
    _pos++; // skip the first _string_distincter
    while (!(_code[_pos] == _string_distincter && _code[_pos-1] != '\\')) {
      _pos++;
    }
    _pos++; // skip the last _string_distincter
    Lex::Token token(_string_type, _string_distincter + _code.substr(old_pos, _pos - old_pos) + _string_distincter, _line);
    token.vstring = _code.substr(old_pos, _pos - old_pos);
    return token;
  }
  string sub_string = _code.substr(_pos, (_code.length()-_pos < _max_token_size?_code.length()-_pos:_max_token_size) );
  // need to find the longest one to avoid , like: "return_func"  return "return"
  bool found = false;
  int longest_length = 0;
  string longest_result;
  string longest_type;
  for (int i = 0; i < _lex_rules.size(); i++) {
    if (_lex_rules[i].dfa->match_from_head(sub_string, result)) {
      if (result.length() > longest_length) {
        longest_type = _lex_rules[i].type;
        longest_result = result;
        found = true;
        longest_length = result.length();
      }
    }
  }
  if (!found) {
    error(_line , "bad lexeme ");
  }
  _pos += longest_result.length();
  Lex::Token token(longest_type, longest_result, _line);
  return token;
}
Lex::Token::Token(string t, string l, int line_) {
  type = t;
  lexeme = l;
  line = line_;
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
vector<Lex::LexRule> Lex::get_lex_rules() {
  vector<Lex::LexRule> safe_lex_rules = _lex_rules;
  for (int i = 0; i < safe_lex_rules.size(); i++) {
    safe_lex_rules[i].dfa = NULL;
  }
  return safe_lex_rules;
}
