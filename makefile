all:
	g++ src/nfa.cpp src/re_parse.cpp src/digraph.cpp src/dfa.cpp src/util.cpp src/main.cpp -w -o dfa
clean:
	rm dfa