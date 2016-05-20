#include "nfa.h"
#include "re_parse.h"
using namespace std;

struct TreeNode;
class NFA;

NFA::NFA(TreeNode* root) {
	_root = root;
	_re_tree_to_nfa();
}
// match the longer one before the short one
int NFA::match(string txt, string& result) {
  for (int i = 0; i < txt.length(); i++) {
    for (int j = txt.length(); j >=i+1 ; j--) {
      if (simulate(txt.substr(i,j-i))) {
        result = txt.substr(i,j-i);
        return i;
      }
    }
  }
  return -1;
}
bool NFA::simulate(string txt) {
  // only match the txt exactly
	set<Digraph::DNode*> now;
	now.insert(_start);
	now = Digraph::e_closure(now);
	for (int i = 0; i < txt.length(); i++) {
		set<Digraph::DNode*> match_set;
		for (set<Digraph::DNode*>::iterator it = now.begin(); it != now.end(); it++) {
			set<Digraph::DNode*> jump_set = (*it)->jump(txt[i]);
			for (set<Digraph::DNode*>::iterator it2 = jump_set.begin(); it2 != jump_set.end(); it2++) {
				match_set.insert(*it2);
			}
		}
		now = Digraph::e_closure(match_set);
  }
	// find the final state at last, in order to know whether exactly match
	for (set<Digraph::DNode*>::iterator it = now.begin(); it != now.end(); it++) {
		if (*it == _end) {
			return true;
		}
	}
	return false;
}
void NFA::_re_tree_to_nfa() {
	_visit_construct_subnfa(_root, _start, _end);
	_end->accept = true;
}
// notice the start and the end will be construct after call this function
void NFA::_visit_construct_subnfa(TreeNode* tnode, Digraph::DNode* &start, Digraph::DNode* &end) {
	if (tnode) {
		Digraph::DNode* left_start, *left_end, *right_start, *right_end;
    switch (tnode->type) {
      case ENUM::TYPE_CAT:
      	_visit_construct_subnfa(tnode->left, left_start, left_end);
    		_visit_construct_subnfa(tnode->right, right_start, right_end);
        start = left_start;
        end = right_end;
        Digraph::addEdge(left_end, EPS, right_start);
        break;
      case ENUM::TYPE_OR:
      	_visit_construct_subnfa(tnode->left, left_start, left_end);
    		_visit_construct_subnfa(tnode->right, right_start, right_end);
        start = new Digraph::DNode();
        end = new Digraph::DNode();
        Digraph::addEdge(start, EPS, left_start);
        Digraph::addEdge(start, EPS, right_start);
        Digraph::addEdge(left_end, EPS, end);
        Digraph::addEdge(right_end, EPS, end);
        break;
      case ENUM::TYPE_STAR:
      	_visit_construct_subnfa(tnode->left, left_start, left_end);
        start = new Digraph::DNode();
        end = new Digraph::DNode();
        Digraph::addEdge(start, EPS, left_start);
        Digraph::addEdge(left_end, EPS, end);
        Digraph::addEdge(start, EPS, end);
        Digraph::addEdge(left_end, EPS, left_start);
        break;
      case ENUM::TYPE_CHAR:
        start = new Digraph::DNode();
        end = new Digraph::DNode();
        Digraph::addEdge(start, tnode->ch, end);
        break;
      case ENUM::TYPE_ANY:
      	start = new Digraph::DNode();
        end = new Digraph::DNode();
        Digraph::addEdge(start, ANY, end);
        break;
      default:
        break;
    }
  }
}
