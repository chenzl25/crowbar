#include "dfa.h"
#include "util.h"
using namespace std;
// class DFA {
// public:
//   DFA(NFA* nfa);
//   DFA(TreeNode* root);
//   bool simulate(string txt);
//   void minimize();
// private:
//   TreeNode* _root;
//   Digraph   _graph;
//   NFA*      _nfa;
//   // in dfa we don't need the end state.
//   // because we can check whether DNode is accept state;
//   Digraph::DNode* _start; 
//   void _nfa_to_dfa();
//   void _re_tree_to_dfa();
// };
class Digraph;


DFA::DFA(NFA* nfa) {
  _nfa = nfa;
  _nfa_to_dfa();
}
DFA::DFA(TreeNode* root) {
  _root = root;
  _re_tree_to_dfa();
}
bool DFA::simulate(string txt) {
  Digraph::DNode *cur_dnode = _start;
  for (int i = 0; i < txt.length(); i++) {
    if (cur_dnode->accept) {
      return true;
    } else {
      set<Digraph::DNode*> next_set = cur_dnode->jump(txt[i]);
      if (next_set.size() == 0) {
        return false;
      }
      assert(next_set.size() == 1, "the size of next_set must be 1");
      for (auto it = next_set.begin(); it != next_set.end(); it++) {
        cur_dnode = *it;
        // we omit the break, for only one item in the next_set
      }
    }
  }
  return false;
}
void DFA::minimize() {

}
void DFA::_nfa_to_dfa() {
  set<Digraph::DNode*> nfa_start_set;
  nfa_start_set.insert(_nfa->_start);
  set<Digraph::DNode*> e_closure_s0 = Digraph::e_closure(nfa_start_set);
  _start = new Digraph::DNode();
  _Dstates[e_closure_s0] = new MNode(false, _start);
  set<Digraph::DNode*> unmarked_state;
  unmarked_state = _find_unmarked_state();
  while (unmarked_state.size() != 0) {
    _Dstates[unmarked_state]->mark = true;
    set<char> input_symbol_set;
    // get all the input symbol
    for (auto it = unmarked_state.begin(); it != unmarked_state.end(); it++) {
      for (int i = 0; i < (*it)->out.size(); i++) {
        if ((*it)->out[i]->symbol != ANY) {
          input_symbol_set.insert((*it)->out[i]->symbol);
        }
      }
    }
    // jump and e-closure
    for (auto it1 = input_symbol_set.begin(); it1 != input_symbol_set.end(); it1++) {
      set<Digraph::DNode*> total_jump_set;
      for (auto it2 = unmarked_state.begin(); it2 != unmarked_state.end(); it2++) {
        set<Digraph::DNode*> jump_set = (*it2)->jump(*it1);
        for (auto it3 = jump_set.begin(); it3 != jump_set.end(); it3++) {
          total_jump_set.insert(*it3);
        }
      }
      set<Digraph::DNode*> new_group = Digraph::e_closure(total_jump_set);
      if (_Dstates.count(new_group) == 0) {
        bool accept = false;
        for (auto it = new_group.begin(); it != new_group.end(); it++) {
          if ((*it)->accept) {
            accept = true;
            break;
          }
        }
        _Dstates[new_group] = new MNode(false, new Digraph::DNode());
        _Dstates[new_group]->dnode->accept = accept;
      }
      Digraph::addEdge(_Dstates[unmarked_state]->dnode, *it1, _Dstates[new_group]->dnode);
    } // end jump and e-closure
    unmarked_state = _find_unmarked_state();
  } // end while
}
void DFA::_re_tree_to_dfa() {

}
set<Digraph::DNode*> DFA::_find_unmarked_state() {
  for (map< set<Digraph::DNode*>,MNode* >::iterator it = _Dstates.begin(); it != _Dstates.end(); it++) {
    if (it->second->mark == false) {
      return it->first;
    }
  }
  set<Digraph::DNode*> s;
  return s;
}
int main(int argc, char**argv) {
  string pre = ".*";
  string text;
  TreeNode* root = parse(pre + argv[1]+ pre);
  visit_print(root); cout << endl;
  NFA nfa(root);
  DFA dfa(&nfa);
  int line = 0;
  while (getline(cin, text)) {
    line++;
    if (dfa.simulate(text)) {
      cout << line << " : " << text << endl;
    }
  }
}