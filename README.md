# lexical-analysis
1. implement of grep by NFA. (regular expression).
2. convert regular expression to NFA.
3. convert NFA to DFA.
4. convert regular expression to DFA.
5. minimize the number of states of DFA.

##how to use

 1. go to the root directory. run `make` to produce executable file `mygrep`.
 2. we can use like `grep` in linux.  for example: `cat src/dfa.cpp | ./mygrep \[a-zA-Z_\]\*\\\(\\\)`

PS: 

 1. you can look at the `src/main.cpp` to configure the finding policy. 
 2. you can improve the performance in the way you like~~



##file structure

 1. `src/main.cpp` : use to make like grep.
 2. `src/re_parse.cpp` : preprocess the regular expression string and convert it to a parse tree.
 3. `src/digraph.(cpp/h)` : use to store the NFA and DFA as graph.
 4. `src/nfa.(cpp/h)` :  convert a regular expression parse tree to NFA and provide method to match the string.
 5. `src/dfa.(cpp/h)` : convert a NFA to DFA and provide method to match the string.  minimize the number of states of DFA.
 6. `src/util.(cpp/h)` : provide auxiliary function.


##Sample Code
```
#include <iostream>
#include "dfa.h"
#include "nfa.h"
#include "re_parse.h"
using namespace std;

int main(int argc, char**argv) {
  string text, result;
  text = "!!!dsadasasd(int)longasd";
  int index;
  TreeNode* root = parse("[a-zA-Z_]*\\(int\\)(long|float){1,3}");
  NFA nfa(root);
  DFA dfa(&nfa);
  dfa.minimize();
  index = dfa.match(text, result);
  index = nfa.match(text, result);
  cout << index << endl; // -1
  cout << result << endl; // dsadasasd(int)long
}
```