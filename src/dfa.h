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
friend class Digraph;
public:
  DFA(string re);
  DFA(NFA* nfa);
  DFA(TreeNode* root);
  ~DFA();
  bool match_from_head(string txt, string &result); //match from head, if ok return true, otherwise false;
  int match(string txt, string& result);
  bool simulate(string txt); // match exactly
  void minimize();
private:
  TreeNode* _root;
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
  enum MODE {
    from_re,
    from_re_parse_tree,
    from_nfa
  };
  MODE _mode;
  map< set<Digraph::DNode*>,MNode* > _Dstates;  // use for 
  map< set<TreeNode*>,MNode* > _Tstates;
  set< Group* > _group;
  map< Digraph::DNode*,Group* > _group_map;
  map< TreeNode*,bool > _nullable_map;
  map< TreeNode*,bool > _has_cal_nullable_map;
  map< TreeNode*,set<TreeNode*> > _firstpos_map;
  map< TreeNode*,bool > _has_cal_firstpos_map;
  map< TreeNode*,set<TreeNode*> > _lastpos_map;
  map< TreeNode*,bool > _has_cal_lastpos_map;
  map< TreeNode*,set<TreeNode*> > _followpos_map;
  bool _has_calculate_followpos;
  // in dfa we don't need the end state.
  // because we can check whether DNode is accept state;
  Digraph::DNode* _start; 
  set<Digraph::DNode*> _find_Dstates_unmarked_state();
  set<TreeNode*> _find_Tstates_unmarked_state();
  
  void _nfa_to_dfa();
  void _partition(); // for minimize
  void _connect();   // for minimize
  void _re_tree_to_dfa();
  bool _nullable(TreeNode* sub_root);
  set<TreeNode*> _firstpos(TreeNode* sub_root);
  set<TreeNode*> _lastpost(TreeNode* sub_root);
  set<TreeNode*> _followpos(TreeNode* sub_root);
  void _visit_calculate_followpos(TreeNode* sub_root);
};
#endif