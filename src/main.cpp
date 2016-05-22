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
  // visit_print(root); cout << endl;
  NFA nfa(root);
  DFA dfa(&nfa);
  int line = 0;
  // dfa.minimize();
  DFA dfa2(root);
  dfa2.minimize();
  while (getline(cin, text)) {
    line++;
    // if (dfa.simulate(text)) {
    //   cout << line << " : " << text << endl;
    // }
    if ((index = dfa2.match(text, result)) != -1) {
      if (result.empty()) {
        cout << "match [empty string]!!!" << endl;
        continue;
      }
      cout << line << " : " << text << endl;
      int tem = line;
      do {
        cout << " ";
      } while (tem /= 10);
      cout << "   ";
      for (int i = 0; i < text.length(); i++) {
        if (i >= index && i < result.size() + index) {
          if (text[i] == '\t') {
            cout << "^^^";
          } else {
            cout << "^";
          }
        } else {
          if (text[i] == '\t') {
            // cout << '\t';
          } else {
            cout << ' ';
          }
        }
      }
      cout << endl;
    }
  }
}