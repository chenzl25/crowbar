#ifndef NFA_H
#define NFA_H
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <set>
#include "re_parse.h"
#include "Digraph.h"
using namespace std;

struct TreeNode;
class DFA;

class NFA {
public:
  friend DFA;
  NFA(TreeNode* root);
  bool simulate(string txt);
private:
  TreeNode* _root;
  Digraph _graph;
  Digraph::DNode* _start, *_end;
  void _re_tree_to_nfa();
  // notice the start and the end will be construct after call this function
  void _visit_construct_subnfa(TreeNode* tnode, Digraph::DNode* &start, Digraph::DNode* &end);
};
#endif
