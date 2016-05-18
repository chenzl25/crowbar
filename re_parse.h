#ifndef RE_PARSE_H
#define RE_PARSE_H
#include <string>
using namespace std;

// struct TreeNode;
struct TreeNode { // the Node of the re parse tree
  int type;
  char ch;
  TreeNode* left; // the TYPE_STAR use the left node only
  TreeNode* right;
  TreeNode(int t, char c, TreeNode* l, TreeNode* r) {
    type = t;
    ch = c;
    left = l;
    right = r;
  }
};
namespace ENUM {
	enum TYPE {
	  TYPE_CAT = 1,
	  TYPE_STAR,
	  TYPE_OR,
	  TYPE_CHAR,
    TYPE_ANY
	};
}

TreeNode* parse(string s);
void visit_print(TreeNode* root);

#endif
// end RE_PARSE_H