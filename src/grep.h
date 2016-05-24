#ifndef GREP_H
#define GREP_H
#include "dfa.h"
#include "nfa.h"
#include "re_parse.h"
#include <string>
using namespace std;

class Grep {
public:
  Grep(string re);
  void match();
private:
  DFA* _dfa;
};

#endif