#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
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
private:
	struct DNode {
		vector<DEdge*> in, out;
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
	void simulate(string txt) {

	}
private:
	TreeNode* _root;
	Digraph _graph;
	Digraph::DNode* _start, *_end;
	void _re_tree_to_nfa() {
		_visit_construct_subnfa(_root, _start, _end);
	}
	// notice the start and the end will be construct after call this function
	void _visit_construct_subnfa(TreeNode* tnode, Digraph::DNode* start, Digraph::DNode* end) {
		cout << "_visit_construct_subnfa" << endl;
		if (tnode) {
			Digraph::DNode* left_start, *left_end, *right_start, *right_end;
	    switch (tnode->type) {
	      case ENUM::TYPE_CAT:
	      	_visit_construct_subnfa(tnode->left, left_start, left_end);
	    		_visit_construct_subnfa(tnode->right, right_start, right_end);
	        // start = left_start;
	        // end = right_end;
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
	TreeNode* root = parse("ab");
  visit_print(root);
  cout << endl;
  // TreeNode* root = parse("((ab|cd|e)*(hg)*)*)");
  NFA nfa(root);
  nfa.simulate("abhg");
  nfa.simulate("zzzz");
}