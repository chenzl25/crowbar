#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include "../../lexer/src/lex.h"
#include "yacc.h"
#include "slr.h"
using namespace std;
int main(int argc, char** argv) {
  string lex_rule_path = "data/crowbar.lex";
  string code_path     = "data/crowbar.code";
  string bnf_rule_path = "data/crowbar.bnf";
  for (int i = 1; i < argc;) {
    if (strcmp(argv[i],"-lex") == 0) {
      lex_rule_path = argv[i+1];
      i+=2;
    } else if (strcmp(argv[i], "-bnf") == 0) {
      bnf_rule_path = argv[i+1];
      i+=2;
    } else if (strcmp(argv[i], "-code") == 0) {
      code_path = argv[i+1];
      i+=2;
    } else {
      i++;
    }
  } 
  Lex mylex;
  if (!mylex.read_lex_rule(lex_rule_path)) {
    error("read lex rule error");
  }
  if (!mylex.read_code(code_path)) {
    error("read code_file error");
  }
  mylex.set_max_token_size(50);
  // mylex.print();
  Yacc myyacc;
  myyacc.set_lexer(mylex);
  if (!myyacc.read_bnf_rule(bnf_rule_path)) {
    error("read bnf rule error");
  }
  // myyacc.parse();
  // myyacc.print();

}