#include "bnf_rule.h"
BnfRule::Symbol::Symbol(bool is_t, string v) {
  is_terminal = is_t;
  value = v;
}

bool BnfRule::Symbol::operator =(Symbol &another) {
  if (is_terminal == another.is_terminal && value == another.value) return true;
  else return false;
}

bool operator == (const BnfRule::Symbol& one, const BnfRule::Symbol& another) {
  return (one.is_terminal == another.is_terminal) && (one.value == another.value);
}
bool operator < (const BnfRule::Symbol& one, const BnfRule::Symbol& another) {
  return one.value < another.value;
}