all:
	g++ nfa.cpp re_parse.cpp digraph.cpp dfa.cpp util.cpp -w -o dfa
clean:
	rm dfa