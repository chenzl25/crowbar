#include <iostream>
#include <cstdio>
#include "slr.h"
#include "../../lexer/src/util.h"
#include <iomanip>
#include "yystype.h"
using namespace std;

/*%include*/

SLR::SLR() {
  _state_count = 0;
  _has_calculate_follow = false;
}
SLR::~SLR() {

}
void SLR::build_from_bnf_rules(vector<BnfRule> bnf_rules){
  _bnf_rules = bnf_rules;
  _construct_terminal_and_nonterminal_set();
  _construct_states(); // _goto_table  construct here implictly
  _construct_action_table();
  _print_to_yacc_output();
  // _print_terminal_and_nonterminal_set();
  // _print_first_follow();
  // cout << "final state " <<  _states.size() << endl;
  // for (auto ii : _states) _print_state(ii);
  // _print_state_transition();
  // _print_action_table();
  // _print_goto_table();
  // _print_bnf_rule();
}

void SLR::_construct_states() {
  assert(_bnf_rules.size() > 0, "_bnf_rules.size should > 0");
  // assume _bnf_rules[0] is the start bnf_rule
  BnfRule extend_bnf_rule;
  extend_bnf_rule.head.value = "S\'";
  extend_bnf_rule.body.push_back(_bnf_rules[0].head);
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
  
  // here we get the new state
  set<SLR::Item> next_closure_item_set = _closure(next_kernel_item_set);
  if (!_only_item_states.count(next_closure_item_set)) {
    SLR::State new_state(_state_count, next_kernel_item_set, next_closure_item_set);
    _all_item_state_no_map[next_closure_item_set] = _state_count;
    _states.push_back(new_state);
    _state_count++;
  }
  _goto_table[_all_item_state_no_map[I]][X] = _all_item_state_no_map[next_closure_item_set];

  _goto_map[IandX] = next_closure_item_set;
  
  { // ----------------------------------------------------
    // cout << "I : " << _all_item_state_no_map[I] << endl;
    // for (auto ii : I) _print_item(ii);
    // cout << "edge : "  << X.value << endl;
    // cout << "J : " << _all_item_state_no_map[next_closure_item_set] << endl;
    // for (auto ii : next_kernel_item_set) _print_item(ii);

    // cout << "clousre :" << endl;
    // for (auto ii : _goto_map[IandX]) _print_item(ii);
    // cout << endl;
  } // ----------------------------------------------------

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
        if (bnf_rule.head.value == B) {
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
  cout << _bnf_rules[item.rule_pos].head.value << " => ";
  int cnt = 0;
  if (_bnf_rules[item.rule_pos].body.size() == 0) {
    cout << "ε";
  }
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
void SLR::_print_bnf_rule() {
  cout << "--------------------------------------------------------------" << endl;
  cout << "_bnf_rules :" << endl;
  for (auto bnf_rule : _bnf_rules) {
    cout << bnf_rule.head.value << " -> ";
    if (bnf_rule.body.size() == 0) {
      cout << "ε";
    }
    for (auto symbol : bnf_rule.body) {
      cout << symbol.value << " ";
    }
    cout << endl;
  } 
  cout << "--------------------------------------------------------------" << endl;
}
void SLR::_print_specific_bnf_rule(int rule_pos) {
  auto bnf_rule = _bnf_rules[rule_pos];
  cout << bnf_rule.head.value << " -> ";
  if (bnf_rule.body.size() == 0) {
    cout << "ε";
  }
  for (auto symbol : bnf_rule.body) {
    cout << symbol.value << " ";
  }
  cout << endl;
}
void SLR::_print_state_transition() {
  for (auto ii : _goto_table) {
    for (auto jj: ii.second) {
      cout << ii.first << " => " << jj.first.value << " => " << jj.second << endl;
    }
  }
}
SLR::State::State() {

}
SLR::State::State(int _no, set<Item> _kernel, set<Item> _item) {
  no = _no;
  kernel = _kernel;
  item = _item;
}
bool SLR::_nullable(BnfRule::Symbol symbol) {
  if (_nullable_map.count(symbol)) {
    return _nullable_map[symbol];
  }
  if (symbol.is_terminal) {
    return false;
  }
  for (auto bnf_rule : _bnf_rules) {
    if (bnf_rule.head == symbol) {
      if (bnf_rule.body.size() == 0) {
        _nullable_map[symbol] = true;
      }
    }
  }
  _nullable_map[symbol] = false;
  return _nullable_map[symbol];
}
set<string> SLR::_first(BnfRule::Symbol symbol) {
  if (_first_map.count(symbol)) {
    return _first_map[symbol];
  }
  set<string> first_set;
  if (symbol.is_terminal) {
    first_set.insert(symbol.value);
    return first_set;
  }

  for (auto bnf_rule : _bnf_rules) {
    if (bnf_rule.head == symbol) {
      if (bnf_rule.body.size() == 0) {
        first_set.insert(EPS_STRING);
        continue;
      }
      for (int i =0; i < bnf_rule.body.size(); i++) {
        auto body_symbol = bnf_rule.body[i];
        // only work at the first order recursion
        // E -> Eβ1 | Eβ2 | ... | Eβm | α1 | α2 | ... | αn
        // calculate the First(E) will occur left recursion
        // if we assume the grammer is just first order recursion,
        // we can just use break to skip
        // we know First(E) should be {α1, α2, ... , αn}
        // PS : first order recursion means no this production occur
        //      E -> A
        //      A -> E
        if (body_symbol == symbol) break;
        /////////////////////////////////////////
        auto tmp_first_set = _first(body_symbol);
        for (auto it_first : tmp_first_set) {
          if (it_first != EPS_STRING) {
            first_set.insert(it_first);
          }
        }
        if (!_nullable(body_symbol)) {
          break;
        }
        if (i == bnf_rule.body.size() -1) {
          first_set.insert(EPS_STRING);
        }
      }
    }
  }
  _first_map[symbol] = first_set;
  return first_set;
}
set<string> SLR::_first_vec(vector<BnfRule::Symbol> symbol_vec) {
  if (_first_vec_map.count(symbol_vec)) {
    return _first_vec_map[symbol_vec];
  }
  set<string> first_set;
  if (symbol_vec.size() == 0) {
    return first_set;
  }
  for (int i =0; i < symbol_vec.size(); i++) {
    auto body_symbol = symbol_vec[i];
    for (auto it_first : _first(body_symbol)) {
      first_set.insert(it_first);
    }
    if (!_nullable(body_symbol)) {
      break;
    }
  }
  _first_vec_map[symbol_vec] = first_set;
  return first_set;
}

set<string> SLR::_follow(BnfRule::Symbol symbol) {
  if (!_has_calculate_follow) {
    _calculate_follow();
    _has_calculate_follow = true;
  }
  if (_follow_map.count(symbol)) {
    return _follow_map[symbol];
  }
  if (symbol.is_terminal) {
    error("calculate the follow for the terminal");
  }
  set<string> empty_set;
  return empty_set;
}
void SLR::_calculate_follow() {
  while (true) {
    bool follow_set_has_change = false;
    for (auto outter_bnf_rule : _bnf_rules) {
      BnfRule::Symbol symbol = outter_bnf_rule.head; // B
      set<string> follow_set, modified_set;
      modified_set = _follow_map[symbol];
      if (symbol == _bnf_rules[0].head) { // $ is in Follow(S)
        follow_set.insert(EOF_STRING);
      } 
      for (auto bnf_rule : _bnf_rules) {
        if (bnf_rule.body.size() == 0) continue;
        for (int i = 0; i < bnf_rule.body.size(); i++) {
          auto body_symbol = bnf_rule.body[i];
          if (body_symbol == symbol) { //A -> αBβ
            if (i == bnf_rule.body.size() -1) { //A -> αB then Follow(A) is in Follow(B)
              auto tmp_follow_set = _follow_map[bnf_rule.head];
              for (auto it_follow : tmp_follow_set) {
                follow_set.insert(it_follow);
              }
            } else {
              vector<BnfRule::Symbol> right_symbols;  // β
              for (int j = i + 1; j < bnf_rule.body.size(); j++) {
                right_symbols.push_back(bnf_rule.body[j]);
              }
              auto tmp_first_vec_set = _first_vec(right_symbols); //First(β)
              if (tmp_first_vec_set.count(EPS_STRING)) { // if First(β) contain ε then... 
                for (auto it_follow : tmp_first_vec_set) { // First(β) except ε is in Follow(B)
                  if (it_follow != EPS_STRING) {        
                    follow_set.insert(it_follow);
                  }
                }
                auto A = _follow_map[bnf_rule.head];
                for (auto it_follow : A) { // Follow(A) is in Follow(B)
                  follow_set.insert(it_follow);
                }
              } else {   // if First(β) doesn't contain ε , First(β) is in Follow(A)
                for (auto it_follow : tmp_first_vec_set) {
                  follow_set.insert(it_follow);
                }
              }
            }
          }
        }
      }
      int before_modified_size = modified_set.size();
      for (auto it_follow : follow_set) modified_set.insert(it_follow);
      int after_modified_size = modified_set.size();
      // we need to iterate all the follow sets don't change again
      if (before_modified_size != after_modified_size) { 
        follow_set_has_change = true;
      }
      _follow_map[symbol] = modified_set;
    } // end for
    if (!follow_set_has_change) {
      break;
    }
  } // end while
}
void SLR::_print_first_follow() {
  set<BnfRule::Symbol> symbol_set;
  cout << "=================================" << endl;
  for (auto bnf_rule : _bnf_rules) {
    if (!symbol_set.count(bnf_rule.head)) {
      symbol_set.insert(bnf_rule.head);
      cout << "First " << bnf_rule.head.value << " => ";
      for (auto s : _first(bnf_rule.head)) {
        cout << " " << s;
      } 
      cout << endl;
    }
  }
  symbol_set.clear();
  for (auto bnf_rule : _bnf_rules) {
    if (!symbol_set.count(bnf_rule.head)) {
      symbol_set.insert(bnf_rule.head);
      cout << "Follow " << bnf_rule.head.value << " => ";
      for (auto s : _follow(bnf_rule.head)) {
        cout << " " << s;
      } 
      cout << endl;
    }
  }
  cout << "=================================" << endl;
}
void SLR::_print_terminal_and_nonterminal_set() {
  cout << "terminal set : " << endl;
  for (auto terminal : _terminal_set) {
    cout << terminal << " ";
  }
  cout << endl << "nonterminal set : " << endl;
  for (auto nonterminal : _nonterminal_set) {
    cout << nonterminal.value << " ";
  }
  cout << endl;
}
void SLR::_construct_terminal_and_nonterminal_set() {
  _terminal_set.insert(EOF_STRING);
  for (auto bnf_rule : _bnf_rules) {
    _nonterminal_set.insert(bnf_rule.head);
    for (auto body_symbol : bnf_rule.body) {
      if (body_symbol.is_terminal) {
        _terminal_set.insert(body_symbol.value);
      }
    }
  }
}
SLR::Action::Action() {
  type = ENUM::ACTION_ERROR; // default type
}
SLR::Action::Action(ENUM::ActionType _type) {
  type = _type;
}
SLR::Action::Action(ENUM::ActionType _type, int _state_no, int _rule_pos) {
  type = _type;
  state_no = _state_no;
  rule_pos = _rule_pos;
}
void SLR::_construct_action_table() {
  for (auto state : _states) {
    for (auto item: state.item) {
      if (item.rule_pos == 0 && item.dot_pos == _bnf_rules[item.rule_pos].body.size()) {
        _action_table[state.no][EOF_STRING] = SLR::Action(ENUM::ACTION_ACCEPT);
      } else if (item.dot_pos == _bnf_rules[item.rule_pos].body.size()) {
        auto follow_set = _follow(_bnf_rules[item.rule_pos].head);
        for (auto it_follow : follow_set) {
          if (_action_table[state.no][it_follow].type == ENUM::ACTION_REDUCE &&
              _action_table[state.no][it_follow].rule_pos != item.rule_pos) {
            cout << "state.no " << state.no << " on input " << it_follow << " ";
            cout << "r" << _action_table[state.no][it_follow].rule_pos << " , ";
            cout << "r" << item.rule_pos << endl;
            cout << "we prefer the most first bnf_rule one" << endl;
            if (item.rule_pos < _action_table[state.no][it_follow].rule_pos) {
              _action_table[state.no][it_follow] = SLR::Action(ENUM::ACTION_REDUCE, NULL, item.rule_pos);
            }
            // error("reduce reduce conflit");
          } else if (_action_table[state.no][it_follow].type == ENUM::ACTION_SHIFT) {
            cout << "shift reduce conflit : "; 
            cout << "state.no " << state.no << " on input " << it_follow << " ";
            cout << "s" << _action_table[state.no][it_follow].state_no << " , ";
            cout << "r" << item.rule_pos << endl;
            cout << "we prefer shift" << endl;
            continue;
            // error("shift reduce conflit");
          } else if (_action_table[state.no][it_follow].type == ENUM::ACTION_ACCEPT) {
            error("accept error");
          }
          _action_table[state.no][it_follow] = SLR::Action(ENUM::ACTION_REDUCE, NULL, item.rule_pos);
        }
      } else if (item.dot_pos < _bnf_rules[item.rule_pos].body.size()) {
        BnfRule::Symbol dot_right = _bnf_rules[item.rule_pos].body[item.dot_pos];
        if (dot_right.is_terminal) {
          int shift_state_no =  _goto_table[state.no][dot_right];
          if (_action_table[state.no][dot_right.value].type == ENUM::ACTION_REDUCE) {
            cout << "shift reduce conflit : ";
            cout << "state.no " << state.no << " on input " << dot_right.value << " ";
            cout << "s" << _action_table[state.no][dot_right.value].state_no << " , ";
            cout << "r" << item.rule_pos << endl;
            cout << "we prefer shift" << endl;
            // error("shift reduce conflit");
          } else if (_action_table[state.no][dot_right.value].type == ENUM::ACTION_SHIFT &&
                     _action_table[state.no][dot_right.value].state_no != shift_state_no) {
            error("shift shift conflit");
          } else if (_action_table[state.no][dot_right.value].type == ENUM::ACTION_ACCEPT) {
            error("accept error");
          }
          _action_table[state.no][dot_right.value] = SLR::Action(ENUM::ACTION_SHIFT, shift_state_no, NULL);
        }
      } else {
        // nothing can go here
      }
    }
  }
}
template<typename T> void wprint(T t, const int& width = 8) {
  cout << std::left << setw(width) << setfill(' ') << t;
}
void SLR::_print_action_table() {
  cout << "--------------------------------------------------------------" << endl;
  wprint("state");
  for (auto terminal : _terminal_set) {
    wprint(terminal);
  } cout << endl;
  for (int i = 0; i < _states.size(); i++) {
    wprint(i);
    for (auto terminal : _terminal_set) {
      auto action = _action_table[i][terminal];
      switch(action.type) {
        case ENUM::ACTION_SHIFT:
          wprint("s" + std::to_string(action.state_no));
          break;
        case ENUM::ACTION_REDUCE:
          wprint("r" + std::to_string(action.rule_pos));
          break;
        case ENUM::ACTION_ACCEPT:
          wprint("acc");
          break;
        case ENUM::ACTION_ERROR:
          wprint(" ");
          break;
        default:
          break;
      }
    }
    cout << endl;
  }
  cout << "--------------------------------------------------------------" << endl;
}
void SLR::_print_goto_table() {
  cout << "--------------------------------------------------------------" << endl;
  wprint("state");
  for (auto nonterminal : _nonterminal_set) {
    wprint(nonterminal.value);
  } cout << endl;
  for (int i = 0; i < _states.size(); i++) {
    wprint(i);
    for (auto nonterminal : _nonterminal_set) {
      int to_state_no = _goto_table[i][nonterminal];
      if (to_state_no != 0) wprint(to_state_no);
      else wprint(" ");
    }
    cout << endl;
  }
  cout << "--------------------------------------------------------------" << endl;
}
void SLR::_print_to_yacc_output() {
  freopen("myyacc.output", "w", stdout);
  _print_terminal_and_nonterminal_set();
  cout << endl;
  _print_first_follow();
  cout << endl;
  cout << "final state " <<  _states.size() << endl;
  for (auto ii : _states) _print_state(ii);
  cout << endl;
  _print_state_transition();
  cout << endl;
  _print_action_table();
  cout << endl;
  _print_goto_table();
  cout << endl;
  _print_bnf_rule();
  cout << endl;
  freopen ("/dev/tty", "a", stdout);
}
void SLR::parse(Lex &lexer) {
  string action_string;
  stack<int> state_stack;
  stack<YYSTYPE> yystype_stack;
  state_stack.push(0);
  int body_length;
  // EOF Token  <"EOF", "$">
  Lex::Token token = lexer.get_token();
  while(true) {
    // cout << token.type << " -> " << token.lexeme << endl;
    //=========== default cooperate with lexer  ======================
    if (token.type == "number") {
      action_string = "number";
    } else if (token.type == "factor") {
      action_string = "factor";
    } else if (token.type == "identifier") {
      action_string = "identifier";
    } else if (token.type == "string") {
      action_string = "string";
    }else {
      action_string = token.lexeme;
    }
    //=============================================
    auto action = _action_table[state_stack.top()][action_string];
    switch(action.type) {
      case ENUM::ACTION_SHIFT:
        state_stack.push(action.state_no);
        // cout << "type: " << action_string << endl;
        //=========== default cooperate with lexer  ======================
        if (action_string == "number") {
          YYSTYPE u;
          u.int_value = token.vint;
          yystype_stack.push(u);
        } else if (action_string == "factor") {
          YYSTYPE u;
          u.double_value = token.vdouble;
          yystype_stack.push(u);
        } else if (action_string == "string") {
          YYSTYPE u;
          u.string_value = new string(token.vstring);
          yystype_stack.push(u);
        } else {
          YYSTYPE u;    // empty yystype use only to take place
          yystype_stack.push(u);
        }
        //=============================================
        // cout << "shift " << action.state_no << " <= " << action_string << endl;
        token = lexer.get_token();
        break;
      case ENUM::ACTION_REDUCE:
        body_length = _bnf_rules[action.rule_pos].body.size();
        if (body_length > 0) {
          while (body_length--) state_stack.pop();
        }
        state_stack.push(_goto_table[state_stack.top()][_bnf_rules[action.rule_pos].head]);
        // action... 
        // cout << "reduce ";
        // _print_specific_bnf_rule(action.rule_pos); 
        // cout << _bnf_rules[action.rule_pos].action_string;
        //-----------------------------------
        switch (action.rule_pos) {
          /*%action*/
          case 1:
            {
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = u1.double_value;
              yystype_stack.push(u);
            }
            break;
          case 2: 
            {
              YYSTYPE u3 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u2 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = u1.double_value + u3.double_value;
              yystype_stack.push(u);
              // cout << u.double_value << "!!!!!" << endl;
            }
            break;
          case 3:
            {
              YYSTYPE u3 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u2 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = u1.double_value - u3.double_value;
              yystype_stack.push(u);
              // cout << u.double_value << "!!!!!" << endl;
            }
            break;
          case 4: 
            {
              YYSTYPE u2 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              printf("%lf\n",u2.double_value);
              YYSTYPE u;
              yystype_stack.push(u);
            }
            break;
          case 5:
            {
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = u1.double_value;
              yystype_stack.push(u);
            }
            break;
          case 6:
            {
              YYSTYPE u3 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u2 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = u1.double_value * u3.double_value;
              yystype_stack.push(u);
              // cout << u.double_value << "!!!!!" << endl;
            }
            break;
          case 7:
            {
              YYSTYPE u3 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u2 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = u1.double_value / u3.double_value;
              yystype_stack.push(u);
              // cout << u.double_value << "!!!!!" << endl;
            }
            break;
          case 8:
            {
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = u1.int_value;
              yystype_stack.push(u);
              // cout << u.double_value << "!!!!!" << endl;
            }
            break;
          case 9:
            {
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = u1.double_value;
              yystype_stack.push(u);
              // cout << u.double_value << "!!!!!" << endl;
            }
            break;
          case 10:
            {
              YYSTYPE u3 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u2 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = u2.double_value;
              yystype_stack.push(u);
              // cout << u.double_value << "!!!!!" << endl;
            }
            break;
          case 11:
            {
              YYSTYPE u2 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u1 = yystype_stack.top();
              yystype_stack.pop();
              YYSTYPE u;
              u.double_value = -u1.double_value;
              yystype_stack.push(u);
              // cout << u.double_value << "!!!!!" << endl;
            }
            break;
          default:
            break;
        }// end switch
        //-----------------------------------
        
        break;
      case ENUM::ACTION_ACCEPT:
        cout << "accept : parse done!" << endl;
        return;
        break;
      case ENUM::ACTION_ERROR:
        cout << action_string << endl;
        cout << state_stack.top() << endl;
        error("line " + std::to_string(token.line) + ": parse error");
        break;
      default:
        error("wrong action type");
        break;
    }

  }
}
