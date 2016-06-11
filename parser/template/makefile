SRCS  = main.cpp

SRCS += parser/src/yacc.cpp
SRCS += parser/src/slr.cpp
SRCS += parser/src/bnf_rule.cpp
SRCS += parser/src/hardcode.cpp

SRCS += lexer/src/nfa.cpp
SRCS += lexer/src/util.cpp
SRCS += lexer/src/dfa.cpp
SRCS += lexer/src/digraph.cpp
SRCS += lexer/src/re_parse.cpp
SRCS += lexer/src/lex.cpp

SRCS += extend/crowbar.cpp
SRCS += extend/CRB.cpp
SRCS += extend/create.cpp
SRCS += extend/crowbar_type.cpp
SRCS += extend/crowbar_util.cpp
SRCS += extend/eval.cpp
SRCS += extend/execute.cpp
SRCS += extend/native_func.cpp
SRCS += extend/fake_method.cpp


OBJS=$(SRCS:.cpp=.o)

CXX?=g++
CPPFLAGS =-w -std=c++11
# CPPFLAGS +=-D DEBUG
all:  $(OBJS)
	$(CXX) -o dcompile $(OBJS)

yacc.o:     parser/src/yacc.h

bnf_rule.o: parser/src/bnf_rule.h

hardcode.o: parser/src/hardcode.h

slr.o:      parser/src/slr.h

nfa.o:      lexer/src/nfa.h

util.o:     lexer/src/util.h

dfa.o:      lexer/src/dfa.h

digraph.o:  lexer/src/digraph.h

re_parse.o: lexer/src/re_parse.h

main.o:

crowbar.o:  extend/crowbar.h

create.o:   extend/create.h

CRB.o:      extend/CRB.h

crowbar_type.o: extend/crowbar_type.h
  
crowbar_util.o: extend/crowbar_util.h

eval.o: 				extend/eval.h

execute.o:			extend/execute.h

native_func.o:  extend/native_func.h

fake_method.o: extend/fake_method.h

clean:
	-rm -f $(OBJS) dcompile