#include <iostream>
#include "slr.h"
#include "../../lexer/src/util.h"
using namespace std;
SLR::SLR() {
  _state_count = 0;
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
  set<Item> init_kernel_item_set;
  init_kernel_item_set.insert(SLR::Item(0, 0));
  set<SLR::Item> init_closure_item_set = _closure(init_kernel_item_set);
  _only_item_states.insert(init_closure_item_set);
  SLR::State init_state(_state_count, init_kernel_item_set, init_closure_item_set);
  _all_item_state_no_map[init_closure_item_set] = _state_count;
  _state_count++;
  _states.push_back(init_state);

  // something like bfs here, the _states will grow in _goto
  for (int i = 0; i < _states.size(); i++) {
    auto only_item_state = _states[i].item;
    for (auto item : only_item_state) {
      set<Item> J;
      if (item.dot_pos != _bnf_rules[item.rule_pos].body.size()) {
        J = _goto(only_item_state, _bnf_rules[item.rule_pos].body[item.dot_pos]);
      }
      if (!J.empty() && !_only_item_states.count(J)) {
        _only_item_states.insert(J);
      }
    }
  }
  for (int i = 0; i < _states.size(); i++) assert(_states[i].no == i, "state doesn't match");
  cout << "final state " <<  _states.size() << endl;
  // for (auto ii : _states) _print_state(ii);
  for (auto ii : _states_trasition) {
    for (auto jj: ii.second) {
      cout << ii.first << " => " << jj.first.value << " => " << jj.second << endl;
    }
  }
}
set<SLR::Item> SLR::_goto(set<SLR::Item> I, BnfRule::Symbol X) {
  auto IandX = make_pair(I, X);
  if (_goto_map.count(IandX)) {
    return _goto_map[IandX];
  }
  set<SLR::Item> next_kernel_item_set;
  for (auto item : I) { // A -> α.Xβ
    assert(item.dot_pos <= _bnf_rules[item.rule_pos].body.size(), "item dot should between 0 and body.size");
    if (item.dot_pos == _bnf_rules[item.rule_pos].body.size()) continue;
    if (X == _bnf_rules[item.rule_pos].body[item.dot_pos]) {
      next_kernel_item_set.insert(SLR::Item(item.rule_pos, item.dot_pos+1));
    }
  }
  // cout << "I : " << endl;
  // for (auto ii : I) _print_item(ii);
  // cout << "edge : "  << X.value << endl;
  // cout << "J : " << endl;
  // for (auto ii : next_kernel_item_set) _print_item(ii);
  
  // here we get the new state
  set<SLR::Item> next_closure_item_set = _closure(next_kernel_item_set);
  if (!_only_item_states.count(next_closure_item_set)) {
    SLR::State new_state(_state_count, next_kernel_item_set, next_closure_item_set);
    _all_item_state_no_map[next_closure_item_set] = _state_count;
    _states.push_back(new_state);
    _states_trasition[_all_item_state_no_map[I]][X] = _state_count;
    _state_count++;
  }

  _goto_map[IandX] = next_closure_item_set;

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
void SLR::_print_state(State state) {
  cout << "--------------------------------------------------" << endl;
  cout << "NO: " << state.no << endl;
  cout << "kernel :" << endl;
  for (auto ii : state.kernel) _print_item(ii);
  cout << "all item:" << endl;
  for (auto ii : state.item) _print_item(ii);
  cout << "--------------------------------------------------" << endl;
}


SLR::State::State() {

}
SLR::State::State(int _no, set<Item> _kernel, set<Item> _item) {
  no = _no;
  kernel = _kernel;
  item = _item;
}