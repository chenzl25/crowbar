#ifndef CRB_H
#define CRB_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include "crowbar_type.h"
#include "crowbar.h"
using namespace std;


namespace CRB {

class Interpreter {
 private:
  struct Heap;
  struct Stack;
 public:
  static Interpreter* getInstance();
  void chain_statement_list(Statement* statement);
  void add_function(FunctionDefinition* fd);
  void set_line(int line_);
  int get_line();
  Interpreter::Heap* get_heap();
  Interpreter::Stack* get_stack();
 private:
  class Heap {
   public:
    // the heap only calculate the current size, actually alloc is controlled by OS
    Heap();
    ~Heap();
    CRB_TYPE::Object* alloc(CRB_TYPE::ObjectType);
    CRB_TYPE::Object* alloc(string* string_value, bool is_literal_);
    int _threshold;
    int _current_size;
   private:
    list<CRB_TYPE::Object*> _heap_list;
  };
  class Stack {
   public:
    // for the Value reason we use pointer
    // actually, it should not alloc the stack value to the heap
    Stack();
    ~Stack();
    CRB_TYPE::Value* pop();
    CRB_TYPE::Value* peek(int pos);
    void push(CRB_TYPE::Value*);
    int size();
   private:
    vector<CRB_TYPE::Value*> _stack_vec; 
  };
  Heap *_heap;
  Stack * _stack;
  map<string*, FunctionDefinition*> _function_map;
  map<string*, CRB_TYPE::Value*> _variable_map;
  StatementList _statement_list;
  int _line;
  static Interpreter* _instance;
  Interpreter();
  ~Interpreter();
};


Interpreter *create_interpreter(void);
void compile(Interpreter *interpreter, string code_path);
void interpret(Interpreter *interpreter);
void dispose_interpreter(Interpreter *interpreter);
  
} // end CRB namespace

#endif /* CRB_H */
