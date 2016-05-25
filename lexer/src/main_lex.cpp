#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "dfa.h"
#include "util.h"
// modify this to limit the max token size, we use this improve the performance of dfa match
const int max_token_size = 50; 
using namespace std;
struct LexRule {
  string lexeme;
  string pattern;
  DFA    *dfa;
};
string check_escape(string input) {
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

vector<LexRule> build_lex_rule (string conf_path) {
  ifstream conf_in(conf_path);
  string s;
  // build the lex rule
  vector<LexRule> vec;
  while (getline(conf_in, s)) {
    stringstream ss(s);
    char flag;
    ss >> flag;
    LexRule lex_rule;
    if (flag == '%') { 
      ss >> lex_rule.lexeme;
      lex_rule.pattern = "(";
      string raw_string;
      while (ss >> raw_string) {
        raw_string = check_escape(raw_string);
        lex_rule.pattern += raw_string + "|";
      }
      lex_rule.pattern[lex_rule.pattern.length()-1] = ')';
      lex_rule.dfa = new DFA(lex_rule.pattern);
    } else if (flag == '$') {
      ss >> lex_rule.lexeme;
      string re_string;
      getline(ss, re_string);
      // ss >> re_string;
      re_string = trim(re_string);
      cout << re_string << endl;
      lex_rule.pattern += re_string;
      lex_rule.dfa = new DFA(lex_rule.pattern);
    }
    vec.push_back(lex_rule);
  }
  conf_in.close();
  return vec; // vec 0 position has the heighest priority
}
string read_code(string code_path) {
  ifstream code_in(code_path);
  string text, s;
  while (getline(code_in, s)) {
    text += s + '\n';
  }
  code_in.close();
  return text;
}

int main(int argc, char **argv) {
  string code_path;
  if (argc == 2) {
    code_path = argv[1];
  } else {
    code_path = "data/code.data"; // default code path
  }
  vector<LexRule> vec;
  string text;
  vec = build_lex_rule("conf/conf.txt");
  text = read_code(code_path);

  int pos = 0;
  int line = 1;
  while (true) {
    string result;
    bool has_match = false;
    int not_empty_pos = pos;
    for (int i = pos; i < text.length(); i++) {
      if (text[i] == ' ' || text[i] == '\t') {
        not_empty_pos++;
      } else if (text[i] == '\n') {
        line++;
        not_empty_pos++;
      } else {
        break;
      }
    }
    pos = not_empty_pos;
    if (pos >= text.length()) {
      break;
    }
    string sub_string = text.substr(pos, (text.length()-pos < max_token_size?text.length()-pos:max_token_size) );
    for (int i = 0; i < vec.size(); i++) {
      if (vec[i].dfa->match_from_head(sub_string, result)) {
        cout << vec[i].lexeme << " " << result << endl;
        has_match = true;
        pos += result.length();
        break;
      }
    }
    if (!has_match) {
      error(line , "bad lexeme ");
    }
  }
}