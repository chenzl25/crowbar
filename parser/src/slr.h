#ifndef SLR_H
#define SLR_H
#include <set>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <utility>
#include "bnf_rule.h"
#include "../../lexer/src/lex.h"
#include "../../lexer/src/util.h"

using namespace std;

namespace ENUM {
  enum ActionType {
    ACTION_SHIFT,
    ACTION_REDUCE,
    ACTION_ACCEPT,
    ACTION_ERROR
  };
}

class SLR {

public:
  SLR();
  ~SLR();
  void build_from_bnf_rules(vector<BnfRule> bnf_rules);
  void parse(Lex &lexer);
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
  struct Action {
    ENUM::ActionType type;
    int  state_no;
    int  rule_pos;
    Action();
    Action(ENUM::ActionType _type);
    Action(ENUM::ActionType _type, int _state_no, int _rule_pos);
  };
  int _state_count;
  vector<BnfRule> _bnf_rules;
  set< set<Item> > _only_item_states;
  vector< State > _states;
  map< pair<set<Item>, BnfRule::Symbol>, set<Item> > _goto_map;
  map< set<Item>, int > _all_item_state_no_map;
  set< string > _terminal_set;
  set< BnfRule::Symbol > _nonterminal_set;
  map< int, map<BnfRule::Symbol, int> > _goto_table;
  map< int, map<string, Action> > _action_table;
  set<Item> _goto(set<Item> I, BnfRule::Symbol X);
  set<Item> _closure(set<Item> I);
  void _construct_terminal_and_nonterminal_set();
  void _construct_states();
  void _construct_action_table();
  void _print_bnf_rule();
  void _print_specific_bnf_rule(int rule_pos);
  void _print_item(Item item);
  void _print_state(State state);
  void _print_state_transition();
  void _print_first_follow();
  void _print_action_table();
  void _print_goto_table();
  void _print_terminal_and_nonterminal_set();
  void _print_to_yacc_output();
  map<BnfRule::Symbol, bool> _nullable_map;
  map<BnfRule::Symbol, set<string>> _first_map;
  map< vector<BnfRule::Symbol>, set<string>> _first_vec_map;
  map<BnfRule::Symbol, set<string>> _follow_map;
  bool _nullable(BnfRule::Symbol symbol);
  set<string> _first_vec(vector<BnfRule::Symbol> symbol_vec);
  set<string> _first(BnfRule::Symbol symbol);
  set<string> _follow(BnfRule::Symbol symbol);
  void _calculate_follow();
  bool _has_calculate_follow;
friend bool operator < (const Item& one, const Item& another);
friend bool operator== (const Item& one, const Item& another);
};

#endif