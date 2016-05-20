#include <iostream>
#include "dfa.h"
#include "nfa.h"
#include "re_parse.h"
using namespace std;

int main(int argc, char**argv) {
  string pre = ".*";
  string text;
  if (argc == 0) {
    cout << "we need a regular expression as argument" << endl;
  }
  TreeNode* root = parse(pre + argv[1]+ pre);
  visit_print(root); cout << endl;
  NFA nfa(root);
  DFA dfa(&nfa);
  int line = 0;
  while (getline(cin, text)) {
    line++;
    if (dfa.simulate(text)) {
      cout << line << " : " << text << endl;
    }
    // if (nfa.simulate(text)) {
    //   cout << line << " : " << text << endl;
    // }
  }
}