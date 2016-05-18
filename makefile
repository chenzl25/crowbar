all:
	g++ nfa_build.cpp re_parse.cpp -w -o nfa
clean:
	rm nfa