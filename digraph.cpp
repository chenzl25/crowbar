#include "digraph.h"
using namespace std;

Digraph::DEdge::DEdge(Digraph::DNode* f, char s, Digraph::DNode* t) {
  from = f;
  symbol = s;
  to = t;
}

void Digraph::addEdge(Digraph::DNode* from, char symbol, Digraph::DNode* to) {
  DEdge*  edge = new DEdge(from ,symbol, to);
  from->out.push_back(edge);
  to->in.push_back(edge);
}
void Digraph::dfs(Digraph::DNode* from_node, set<Digraph::DNode*>& result_set) {
  set<Digraph::DNode*> e_set = from_node->jump(EPS);
  for (set<Digraph::DNode*>::iterator it = e_set.begin(); it != e_set.end(); it++) {
    if (!result_set.count(*it)) {
      result_set.insert(*it);
      Digraph::dfs(*it, result_set);
    }
  }
}
set<Digraph::DNode*> Digraph::e_closure(set<Digraph::DNode*>& from_set) {
  set<Digraph::DNode*> result_set;
  for (set<Digraph::DNode*>::iterator it = from_set.begin(); it != from_set.end(); it++) {
    Digraph::dfs(*it, result_set);
  }
  return result_set;
}
Digraph::DNode::DNode() {
  accept = false;
}
set<Digraph::DNode*> Digraph::DNode::jump(char s) {
  set<Digraph::DNode*> result;
  for (int i = 0; i < out.size(); i++) {
    if (out[i]->symbol == s | out[i]->symbol == ANY) {
      result.insert(out[i]->to);
    }
  }
  return result;
}
