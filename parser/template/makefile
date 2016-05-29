SRCS  = parser/src/main.cpp
SRCS += parser/src/yacc.cpp
SRCS += parser/src/slr.cpp
SRCS += parser/src/bnf_rule.cpp
SRCS += lexer/src/nfa.cpp
SRCS += lexer/src/util.cpp
SRCS += lexer/src/dfa.cpp
SRCS += lexer/src/digraph.cpp
SRCS += lexer/src/re_parse.cpp
SRCS += lexer/src/lex.cpp

OBJS=$(SRCS:.cpp=.o)

CXX?=g++
CPPFLAGS =-w -std=c++11
# CPPFLAGS +=-D DEBUG
all:	$(OBJS)
	$(CXX) -o dcompile $(OBJS)

yacc.o:     parser/src/yacc.h

bnf_rule.o: parser/src/bnf_rule.h

slr.o:      parser/src/slr.h

nfa.o:      lexer/src/nfa.h

util.o:     lexer/src/util.h

dfa.o:      lexer/src/dfa.h

digraph.o:  lexer/src/digraph.h

re_parse.o: lexer/src/re_parse.h

main.o:

clean:
	-rm -f $(OBJS) dcompile