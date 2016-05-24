# lexical-analysis
1. implement of grep by NFA. (regular expression).
2. convert regular expression to NFA.
3. convert NFA to DFA.
4. convert regular expression to DFA.
5. minimize the number of states of DFA.

##how to use

###mygrep
 - go to the root directory. run `make grep` to produce executable file `mygrep`.
 - we can use like `grep` in linux.  
for example:
 $: `cat src/dfa.cpp | ./mygrep \[a-zA-Z_\]\*\\\(\\\)`
 $: `cat test/mail.txt | ./mygrep \\w+@\[\\w\\d\\\.\]\+\[\\w\]\+`

 ###mylex
 
 - go to the root directory. run `make lex` to produce executable file `mylex`.
 - write the configuration file  in `conf/conf.txt`.
 - give the code path to `./mylex` then it can output the token<name,value>
 
for example:
 $: `./mylex data/code.data`

PS: 

 1. you can look at the `src/main_grep.cpp`  or `src/main_lex.cpp`to configure the finding policy. 
 2. you can improve the performance in the way you like~~


##file structure

 1. `src/main_grep.cpp` : use to make like grep.
 2. `src/main_lex.cpp`   : mylex file output the token.
 3. `src/re_parse.cpp` : preprocess the regular expression string and convert it to a parse tree.
 4. `src/digraph.(cpp/h)` : use to store the NFA and DFA as graph.
 5. `src/nfa.(cpp/h)` :  convert a regular expression parse tree to NFA and provide method to match the string.
 6. `src/dfa.(cpp/h)` : convert a NFA to DFA and provide method to match the string.  minimize the number of states of DFA.
 7. `src/util.(cpp/h)` : provide auxiliary function.
 8. `src/grep.(cpp/h)` : mygrep file, make the output human readable.


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
###mylex conf.txt
```
% reserve_words if else for while switch do case return
% operators >= <= == != > < = + - * / ( ) ; { } [ ] ,
$ type (int|char|long long|long|float|double|void)
$ number [-]?[0-9]+(\.[0-9]+)?
$ identifier [a-zA-Z_][a-zA-Z0-9]*
```
after the % simple string match
after the $ regular expression match

###mylex code.data
```
int main() {
  int size = 1.2;
  for (int i = 0; i < size; i++) {
    print(i);
  }
  long long j = 0.333;
}
int add (int a, int b) {
  return a + b;
}
```

###mylex output
```
type int
identifier main
operators (
operators )
operators {
type int
identifier size
operators =
number 1.2
operators ;
reserve_words for
operators (
type int
identifier i
operators =
number 0
operators ;
identifier i
operators <
identifier size
operators ;
identifier i
operators +
operators +
operators )
operators {
identifier print
operators (
identifier i
operators )
operators ;
operators }
type long long
identifier j
operators =
number 0.333
operators ;
operators }
type int
identifier add
operators (
type int
identifier a
operators ,
type int
identifier b
operators )
operators {
reserve_words return
identifier a
operators +
identifier b
operators ;
operators }
```