#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <set>
#include "re_parse.h"
using namespace std;
#define EPS 0

struct TreeNode;
class NFA;
class Digraph {
friend NFA;
private:
	struct DNode;
	struct DEdge;
public:
 	static void addEdge(DNode* from, char symbol, DNode* to) {
 		DEdge*  edge = new DEdge(from ,symbol, to);
 		from->out.push_back(edge);
 		to->in.push_back(edge);
 	}
 	static void dfs(DNode* from_node, set<DNode*>& result_set) {
 		set<DNode*> e_set = from_node->jump(EPS);
 		for (set<DNode*>::iterator it = e_set.begin(); it != e_set.end(); it++) {
 			if (!result_set.count(*it)) {
 				result_set.insert(*it);
 				Digraph::dfs(*it, result_set);
 			}
 		}
 	}
 	static set<DNode*> e_closure(set<DNode*>& from_set) {
 		set<DNode*> result_set;
 		for (set<DNode*>::iterator it = from_set.begin(); it != from_set.end(); it++) {
 			Digraph::dfs(*it, result_set);
 		}
 		return result_set;
 	}
private:
	struct DNode {
		vector<DEdge*> in, out;
		set<DNode*> jump(char s) {
			set<DNode*> result;
			for (int i = 0; i < out.size(); i++) {
				if (out[i]->symbol == s | out[i]->symbol == '.') {
					result.insert(out[i]->to);
				}
			}
			return result;
		}
	};
	struct DEdge {
		char symbol;
		DNode* to;
		DNode* from;
		DEdge(DNode* f, char s, DNode* t) {
			from = f;
			symbol = s;
			to = t;
		}
	};
 	
};
class NFA {
public:
	NFA(TreeNode* root) {
		_root = root;
		_re_tree_to_nfa();
	}
	bool simulate(string txt) {
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
			// find the final state
			for (set<Digraph::DNode*>::iterator it = now.begin(); it != now.end(); it++) {
				if (*it == _end) {
					return true;
				}
			}
		}
		return false;
	}
private:
	TreeNode* _root;
	Digraph _graph;
	Digraph::DNode* _start, *_end;
	void _re_tree_to_nfa() {
		_visit_construct_subnfa(_root, _start, _end);
	}
	// notice the start and the end will be construct after call this function
	void _visit_construct_subnfa(TreeNode* tnode, Digraph::DNode* &start, Digraph::DNode* &end) {
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
	      default:
	        break;
	    }
	  }
	}
};

int main(int argc, char**argv) {
  string pre = ".*";
	string text;
  TreeNode* root = parse(pre + argv[1]+ pre);
  visit_print(root); cout << endl;
  NFA nfa(root);
	int line = 0;
	while (getline(cin, text)) {
		line++;
		if (nfa.simulate(text)) {
			cout << line << " : " << text << endl;
		}
	}
}