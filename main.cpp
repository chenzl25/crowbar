#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <set>
using namespace std;

class Solution {
public:
    bool isMatch(string s, string p) {
        NFA nfa(p);
        return nfa.recognizes(s);
    }
private:
	class Digraph {
	 private:
	 	vector<int>* _vec;
	 	int _size;
	 	set<int> state;
	 public:
	 	Digraph() {
	 		_size = 20;
	 		_vec = new vector<int> [_size];
	 	}
	 	Digraph(int size):_size(size) {
	 		_size = size;
	 		_vec = new vector<int> [_size];
	 	}
	 	void addEdge(int from, int to) {
	 		if (from > _size || to > _size || from < 0 || to < 0) {
	 			throw false; // bad ways
	 		} else {
	 			_vec[from].push_back(to);
	 		}
	 	}
	 	int size() {
	 		return _size;
	 	}
	 	void dfs(int u) {
	 		if (_vec[u].size() == 0) {
	 			state.insert(u);
	 		}
	 		for (int i = 0; i < _vec[u].size(); i++) {
	 			if (!state.count(_vec[u][i])) {
	 				state.insert(_vec[u][i]);
	 				dfs(_vec[u][i]);
	 			}
	 		}
	 	}
	 	set<int> reachable(set<int>& start) {
	 		state = set<int>();
	 		for (set<int>::iterator it = start.begin(); it != start.end(); it++) {
	 			dfs(*it);
	 		}
	 		return state;
	 	}
	};
	class NFA {
	 private:
	 	string re;
	 	Digraph G;
	 	int _size;
	 public:
	 	NFA (string regexp):G(regexp.size()+1),re(regexp),_size(regexp.size()+1) {
	 		stack<int> ops;
	 		for (int i= 0; i < _size; i++) {
	 			int lp = i;
	 			if (re[i] == '(' || re[i] == '|')
	 				ops.push(i);
	 			else if (re[i] == ')') {
	 				int tem = ops.top();
	 				ops.pop();
	 				if (re[tem] == '|') {
	 					lp = ops.top();
	 					ops.pop();
	 					G.addEdge(lp, tem+1);
	 					G.addEdge(tem, i);
	 				} else {
	 					lp = tem;
	 				}
	 			}
	 			if (i < _size && re[i+1] == '*' ) {
	 				G.addEdge(lp, i+1);
	 				G.addEdge(i+1, lp);
	 			}
	 			if (re[i] == '(' || re[i] == '*' || re[i] == ')')
	 				G.addEdge(i, i+1);
	 		}
	 	}
	 	bool recognizes(string txt) {
	 		set<int> now;
	 		now.insert(0);
	 		now = G.reachable(now);
	 		for (int i = 0; i < txt.size(); i++) {
	 			set<int> match;
	 			for (set<int>::iterator it = now.begin(); it != now.end(); it++)
	 				if (*it < _size)
	 					if (re[*it] == txt[i] || re[*it] == '.')
	 						match.insert(*it + 1);

	 			now = G.reachable(match);
	 		}
	 		for (set<int>::iterator it = now.begin(); it != now.end(); it++)
	 			if (*it == _size || *it == _size - 1) return true;

	 		return false;
	 	}
	};
};
int main(int argc, char* argv[]) {
	string pre = ".*";
	Solution s;
	string text;
	int line = 0;
	while (getline(cin, text)) {
		line++;
		if (s.isMatch(text, pre + argv[1]+ pre)) {
			cout << line << " : " << text << endl;
		}
	}
}