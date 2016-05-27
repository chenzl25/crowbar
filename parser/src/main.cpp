#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../../lexer/src/lex.h"
#include "yacc.h"
#include "slr.h"
using namespace std;
int main(int argc, char** argv) {
  ifstream bnf_in("data/bnf.data");
  string lex_rule_path = "data/lex_rule.data";
  string code_path     = "data/code.data";
  string bnf_rule_path = "data/bnf.data";
  Lex mylex;
  if (!mylex.read_lex_rule(lex_rule_path)) {
    error("read lex rule error");
  }
  if (!mylex.read_code(code_path)) {
    error("read code_file error");
  }
  mylex.set_max_token_size(50);
  // while(true) {
  //   Lex::Token token = mylex.get_token();
  //   if (token.type != "EOF") {
  //     cout << token.type << " -> " << token.lexeme << endl;
  //   } else {
  //     break;
  //   }
  // }
  string test_follow_path = "data/test_follow.data";
  string no_recursion_path ="data/no_recursion.data";
  Yacc myyacc;
  if (!myyacc.read_bnf_rule(bnf_rule_path)) {
    error("read bnf rule error");
  }
  // mylex.print();
  myyacc.print();

}