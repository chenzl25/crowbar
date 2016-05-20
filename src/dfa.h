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
friend Digraph;
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
  struct Group {
    set<Digraph::DNode*> dfa_node_set;
    Digraph::DNode*      representative;
    set<Group*> split(set<char> input_set, map< Digraph::DNode*,Group* > &group_map);
    static bool group_set_deep_equal(set< Group* > & gs1, set< Group* > &gs2);
  };
  map< set<Digraph::DNode*>,MNode* > _Dstates;
  set< Group* > _group;
  map< Digraph::DNode*,Group* > _group_map;
  // in dfa we don't need the end state.
  // because we can check whether DNode is accept state;
  Digraph::DNode* _start; 
  set<Digraph::DNode*> _find_unmarked_state();
  void _nfa_to_dfa();
  void _re_tree_to_dfa();
  void _partition(); // for minimize
  void _connect();   // for minimize
};
#endif