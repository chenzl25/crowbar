#include "bnf_rule.h"
BnfRule::Symbol::Symbol() {
  is_terminal = false;
  value = "";
}
BnfRule::Symbol::Symbol(const Symbol &another) {
  is_terminal = another.is_terminal;
  value = another.value;
}
BnfRule::Symbol::Symbol(bool is_t, string v) {
  is_terminal = is_t;
  value = v;
}

BnfRule::Symbol& BnfRule::Symbol::operator =(const Symbol &another) {
  if (this == &another) return *this;
  is_terminal = another.is_terminal;
  value = another.value;
  return *this;
}

bool operator == (const BnfRule::Symbol& one, const BnfRule::Symbol& another) {
  return (one.is_terminal == another.is_terminal) && (one.value == another.value);
}
bool operator < (const BnfRule::Symbol& one, const BnfRule::Symbol& another) {
  return one.value < another.value;
}