#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "dfa.h"
#include "util.h"
#include "lex.h"

int main(int argc, char **argv) {
  string code_path;
  if (argc == 2) {
    code_path = argv[1];
  } else {
    code_path = "data/code.data"; // default code path
  }
  Lex mylex;
  if (!mylex.read_lex_rule("conf/conf.txt")) {
    error("read lex rule error");
  }
  if (!mylex.read_code(code_path)) {
    error("read code_file error");
  }
  // modify this to limit the max token size, we use this improve the performance of dfa match
  mylex.set_max_token_size(50);
  while(true) {
    Lex::Token token = mylex.get_token();
    if (token.type != "EOF") {
      cout << token.type << " -> " << token.lexeme << endl;
    } else {
      break;
    }
  }
  return 0;
}