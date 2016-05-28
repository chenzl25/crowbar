#include "bnf_rule.h"
BnfRule::Symbol::Symbol() {
  is_terminal = false;
  value = "";
  type = "";
}
BnfRule::Symbol::Symbol(const Symbol &another) {
  is_terminal = another.is_terminal;
  value = another.value;
  type = another.type;
}
BnfRule::Symbol::Symbol(bool is_t, string v, string t) {
  is_terminal = is_t;
  value = v;
  type = t;
}

BnfRule::Symbol& BnfRule::Symbol::operator =(const Symbol &another) {
  if (this == &another) return *this;
  is_terminal = another.is_terminal;
  value = another.value;
  type = another.type;
  return *this;
}

bool operator == (const BnfRule::Symbol& one, const BnfRule::Symbol& another) {
  return (one.is_terminal == another.is_terminal) && 
         (one.value == another.value) &&
         (one.type  == another.type);
}
bool operator < (const BnfRule::Symbol& one, const BnfRule::Symbol& another) {
  return one.value < another.value;
}