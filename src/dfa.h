#ifndef DFA_H
#define DFA_H
#include "digraph.h"
#include "nfa.h"
#include <vector>
#include <set>
#include <map>
using namespace std;
class Digraph;
struct TreeNode;



class DFA {
public:
  DFA(NFA* nfa);
  DFA(TreeNode* root);
  int match(string txt, string& result);
  bool simulate(string txt);
  void minimize();
private:
  TreeNode* _root;
  Digraph   _graph;
  NFA*      _nfa;
  struct MNode {
    bool mark;
    Digraph::DNode* dnode;
    MNode(bool m, Digraph::DNode* d) {
      mark = m;
      dnode = d;
    }
  };
  map< set<Digraph::DNode*>,MNode* > _Dstates;
  // in dfa we don't need the end state.
  // because we can check whether DNode is accept state;
  Digraph::DNode* _start; 
  set<Digraph::DNode*> _find_unmarked_state();
  void _nfa_to_dfa();
  void _re_tree_to_dfa();
};
#endif