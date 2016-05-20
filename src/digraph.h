#ifndef DIGRAPH_H
#define DIGRAPH_H
#include <set>
#include <vector>
#include <iostream>
#define EPS ((char)0)
#define ANY ((char)254)
#define OTHER ((char)255)

using namespace std;
struct TreeNode;
class NFA;
class DFA;

class Digraph {
friend NFA;
friend DFA;
private:
  struct DNode;
  struct DEdge;
public:
  static void addEdge(DNode* from, char symbol, DNode* to);
  static void dfs(DNode* from_node, set<DNode*>& result_set);
  static set<DNode*> e_closure(set<DNode*>& from_set);
private:
  struct DNode {
    DNode();
    bool accept;
    vector<DEdge*> in, out;
    set<DNode*> jump(char s);
  };
  struct DEdge {
    char symbol;
    DNode* to;
    DNode* from;
    DEdge(DNode* f, char s, DNode* t);
  };
};
#endif