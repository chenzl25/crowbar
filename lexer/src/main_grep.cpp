#include <iostream>
#include "dfa.h"
#include "nfa.h"
#include "re_parse.h"
#include "grep.h"
#include "util.h"
using namespace std;


int main(int argc, char**argv) {
  if (argc == 1) {
    error("we need a regular expression as argument");
  }
  Grep mygrep(argv[1]);
  mygrep.match();
}