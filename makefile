SRCS  = src/nfa.cpp
SRCS += src/util.cpp
SRCS += src/dfa.cpp
SRCS += src/digraph.cpp
SRCS += src/re_parse.cpp
SRCS += src/grep.cpp

OBJS=$(SRCS:.cpp=.o)

CXX?=g++
CPPFLAGS =-w -std=c++11
# CPPFLAGS +=-D DEBUG
lex: $(OBJS)
	$(CXX) -o mylex $(OBJS)  src/main_lex.cpp

grep: $(OBJS)
	$(CXX) -o mygrep $(OBJS) src/main_grep.cpp

nfa.o:      src/nfa.h

util.o: 		src/util.h

dfa.o: 			src/dfa.h

digraph.o:  src/digraph.h

re_parse.o: src/re_parse.h

grep.o:     src/grep.h


clean:
	-rm -f $(OBJS) mygrep mylex 