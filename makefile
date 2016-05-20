SRCS  = src/nfa.cpp
SRCS += src/main.cpp
SRCS += src/util.cpp
SRCS += src/dfa.cpp
SRCS += src/digraph.cpp
SRCS += src/re_parse.cpp

OBJS=$(SRCS:.cpp=.o)

CXX?=g++
CPPFLAGS=-w

all: $(OBJS)
	$(CXX) -o dfa $(OBJS)

nfa.o:      src/nfa.h

util.o: 		src/util.h

dfa.o: 			src/dfa.h

digraph.o:  src/digraph.h

re_parse.o: src/re_parse.h

main.o: 

clean:
	-rm $(OBJS) dfa 