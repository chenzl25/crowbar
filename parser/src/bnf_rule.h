#ifndef BNF_RULE_H
#define BNF_RULE_H
#include <string>
#include <vector>
#include <set>
using namespace std;
struct BnfRule {
  struct Symbol {
    bool is_terminal;
    string value;
    Symbol(bool is_t, string v);
    bool operator =(Symbol &another);
    friend bool operator < (const BnfRule::Symbol& one, const BnfRule::Symbol& another);
    friend bool operator == (const BnfRule::Symbol& one, const BnfRule::Symbol& another);
  };
  
  string head;
  vector<BnfRule::Symbol> body;
};

#endif