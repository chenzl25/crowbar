#include "grep.h"

Grep::Grep(string re) {
  TreeNode* root = parse(re);
  // visit_print(root); cout << endl;
  _dfa = new DFA(root);
  _dfa->minimize();
}
void Grep::match() {
  string result, text;
  int index;
  int line = 0;
  while (getline(cin, text)) {
    line++;
    if ((index = _dfa->match(text, result)) != -1) {
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
