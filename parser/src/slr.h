#ifndef SLR_H
#define SLR_H
#include <set>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "bnf_rule.h"
using namespace std;

class Yacc;

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
  vector<BnfRule> _bnf_rules;
  set< set<Item> > _states;
  map< pair<set<Item>, BnfRule::Symbol>, set<Item> > _goto_map;
  set<Item> _goto(set<Item> I, BnfRule::Symbol X);
  set<Item> _closure(set<Item> I);
  void _construct_states();
  void _print_item(Item item);
friend bool operator < (const Item& one, const Item& another);
friend bool operator== (const Item& one, const Item& another);
};

#endif