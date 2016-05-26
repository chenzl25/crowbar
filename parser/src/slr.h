#ifndef SLR_H
#define SLR_H
#include <set>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "bnf_rule.h"
using namespace std;


class SLR {

public:
  SLR();
  void build_from_bnf_rules(vector<BnfRule> bnf_rules);
  ~SLR();
private:
  struct Item {
    int rule_pos;
    int dot_pos;
    Item(int r, int d);
  };
  struct State {
    int no;
    set<Item> kernel;
    set<Item> item;
    State();
    State(int _no, set<Item> _kernel, set<Item> _item);
  };
  int _state_count;
  vector<BnfRule> _bnf_rules;
  set< set<Item> > _only_item_states;
  vector< State > _states;
  map< pair<set<Item>, BnfRule::Symbol>, set<Item> > _goto_map;
  map< set<Item>, int > _all_item_state_no_map;
  map< int, map<BnfRule::Symbol, int> > _states_trasition;
  set<Item> _goto(set<Item> I, BnfRule::Symbol X);
  set<Item> _closure(set<Item> I);
  void _construct_states();
  void _print_item(Item item);
  void _print_state(State state);
friend bool operator < (const Item& one, const Item& another);
friend bool operator== (const Item& one, const Item& another);
};

#endif