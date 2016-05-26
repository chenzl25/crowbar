#include <iostream>
#include "slr.h"
#include "../../lexer/src/util.h"
using namespace std;
SLR::SLR() {

}
SLR::~SLR() {

}
void SLR::build_from_bnf_rules(vector<BnfRule> bnf_rules){
  _bnf_rules = bnf_rules;
  _construct_states();
}

void SLR::_construct_states() {
  assert(_bnf_rules.size() > 0, "_bnf_rules.size should > 0");
  // assume _bnf_rules[0] is the start bnf_rule
  BnfRule extend_bnf_rule;
  extend_bnf_rule.head = "S\'";
  extend_bnf_rule.body.push_back(BnfRule::Symbol(false, _bnf_rules[0].head));
  _bnf_rules.insert(_bnf_rules.begin(), extend_bnf_rule);
  set<Item> init_set;
  init_set.insert(SLR::Item(0, 0));
  _states.insert(_closure(init_set));

  while (true) {
    int old_states_size = _states.size();
    for (auto state : _states) {
      for (auto item : state) {
        set<Item> J;
        if (item.dot_pos != _bnf_rules[item.rule_pos].body.size()) {
          J = _goto(state, _bnf_rules[item.rule_pos].body[item.dot_pos]);
        }
        if (!J.empty() && !_states.count(J)) {
          _states.insert(J);
        }
      }
    }
    if (old_states_size == _states.size()) break;
  }
  // cout << "final state " <<  _states.size() << endl;
}
set<SLR::Item> SLR::_goto(set<SLR::Item> I, BnfRule::Symbol X) {
  auto IandX = make_pair(I, X);
  if (_goto_map.count(IandX)) {
    return _goto_map[IandX];
  }
  set<SLR::Item> next_kernel_item_set;
  for (auto item : I) { // A -> α.Xβ
    assert(item.dot_pos <= _bnf_rules[item.rule_pos].body.size(), "item dot should between 0 and body.size");
    if (X == _bnf_rules[item.rule_pos].body[item.dot_pos]) {
      next_kernel_item_set.insert(SLR::Item(item.rule_pos, item.dot_pos+1));
    }
  }
  // cout << "I : " << endl;
  // for (auto ii : I) _print_item(ii);
  // cout << "edge : "  << X.value << endl;
  // cout << "J : " << endl;
  // for (auto ii : next_kernel_item_set) _print_item(ii);

  _goto_map[IandX] = _closure(next_kernel_item_set);

  // cout << "clousre :" << endl;
  // for (auto ii : _goto_map[IandX]) _print_item(ii);
  // cout << endl;

  return _goto_map[IandX];
}
set<SLR::Item> SLR::_closure(set<SLR::Item> I) {
  set<SLR::Item> J = I;
  while (true) {
    set<SLR::Item> next_J = J;
    for (auto item : J) { //  A -> α.Bβ
      assert(item.dot_pos <= _bnf_rules[item.rule_pos].body.size(), "item dot should between 0 and body.size");
      if (item.dot_pos == _bnf_rules[item.rule_pos].body.size()) continue;
      if (_bnf_rules[item.rule_pos].body[item.dot_pos].is_terminal) continue;
      string B = _bnf_rules[item.rule_pos].body[item.dot_pos].value;
      int rule_count = 0;
      for (auto bnf_rule : _bnf_rules) { // B -> .γ
        if (bnf_rule.head == B) {
          SLR::Item item(rule_count, 0);
          next_J.insert(item);
        }
        rule_count++;
      }
    }
    if (J == next_J) {
      break;
    } else {
      J = next_J;
    }
  }
  return J;
}
SLR::Item::Item(int r, int d) {
  rule_pos = r;
  dot_pos = d;
}
bool operator < (const SLR::Item& one, const SLR::Item& another) {
  if (one.rule_pos < another.rule_pos) return true;
  else if (one.rule_pos == another.rule_pos) {
    if (one.dot_pos < another.dot_pos) return true;
    else return false;
  } else return false;
}
bool operator== (const SLR::Item& one, const SLR::Item& another) {
  return (one.rule_pos == another.rule_pos) && (one.dot_pos == another.dot_pos);
}
void SLR::_print_item(Item item) {
  cout << _bnf_rules[item.rule_pos].head << " => ";
  int cnt = 0;
  for (auto s : _bnf_rules[item.rule_pos].body) {
    if (cnt == item.dot_pos) {
      cout << ".";
    }
    cout << s.value << " ";
    cnt++;
  } 
  if (cnt == item.dot_pos) {
    cout << ".";
  }
  cout << endl;
}

