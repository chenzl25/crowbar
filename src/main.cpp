#include <iostream>
#include "dfa.h"
#include "nfa.h"
#include "re_parse.h"
using namespace std;

int main(int argc, char**argv) {
  string pre = ""; // we can use ".*"
  string text, result;
  int index;
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
    // if (dfa.simulate(text)) {
    //   cout << line << " : " << text << endl;
    // }
    if ((index = dfa.match(text, result)) != -1) {
      cout << line << " : " << text << endl;
      int tem = line;
      do {
        cout << " ";
      } while (tem /= 10);
      cout << "   ";
      for (int i = 0; i < text.length(); i++) {
        if (i >= index && i < result.size() + index) {
          cout << "^";
        } else {
          cout << ' ';
        }
      }
      cout << endl;
    }
  }
}