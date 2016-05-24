#include <iostream>
#include "dfa.h"
#include "nfa.h"
#include "re_parse.h"
#include "grep.h"
using namespace std;


int main(int argc, char**argv) {
  if (argc == 0) {
    cout << "we need a regular expression as argument" << endl;
  }
  Grep mygrep(argv[1]);
  mygrep.match();
}