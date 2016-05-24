#include "digraph.h"
#include "util.h"
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
  result_set.insert(from_node);
  for (auto it_edge : from_node->out) {
    if (!result_set.count(it_edge->to)) {
      Digraph::dfs(it_edge->to, result_set);
    }
  }
}
void Digraph::delete_from (DNode* from_node) {
  set<Digraph::DNode*> delete_set;
  Digraph::dfs(from_node, delete_set);
  for (auto it_node : delete_set) {
    delete it_node;
  }
}
void Digraph::e_dfs(Digraph::DNode* from_node, set<Digraph::DNode*>& result_set) {
  result_set.insert(from_node);
  set<Digraph::DNode*> e_set = from_node->jump(EPS);
  for (set<Digraph::DNode*>::iterator it = e_set.begin(); it != e_set.end(); it++) {
    if (!result_set.count(*it)) {
      Digraph::e_dfs(*it, result_set);
    }
  }
}
set<Digraph::DNode*> Digraph::e_closure(set<Digraph::DNode*>& from_set) {
  set<Digraph::DNode*> result_set;
  for (set<Digraph::DNode*>::iterator it = from_set.begin(); it != from_set.end(); it++) {
    Digraph::e_dfs(*it, result_set);
  }
  return result_set;
}
Digraph::DNode::DNode() {
  accept = false;
}
set<Digraph::DNode*> Digraph::DNode::jump(char s) {
  set<Digraph::DNode*> result;
  int  other_edge_pos = -1;
  int  other_edge_count = 0;
  set<char> input_explicit_set;
  for (int i = 0; i < out.size(); i++) {
    if (out[i]->symbol == OTHER) {
      other_edge_pos = i;
      other_edge_count++;
    } else {
      input_explicit_set.insert(out[i]->symbol);
    }
  }
  assert(other_edge_count <= 1, "the other edge should 1 at most.");
  if (other_edge_count == 1) {
    if (input_explicit_set.count(s)) {
      for (int i = 0; i < out.size(); i++) {
        if (out[i]->symbol == s) {
          result.insert(out[i]->to);
        }
      }
    } else { // the other input
      result.insert(out[other_edge_pos]->to);
    }
  } else {
    for (int i = 0; i < out.size(); i++) {
      if (out[i]->symbol == s) {
        result.insert(out[i]->to);
      } else if (s != EPS && out[i]->symbol == ANY) {
        result.insert(out[i]->to); 
      }
    }
  }
  return result;
}
