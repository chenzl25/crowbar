#ifndef CRB_H
#define CRB_H
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <list>
#include "crowbar_type.h"
#include "crowbar.h"
using namespace std;


namespace CRB {

class LocalEnv {
 public:
  LocalEnv(CRB_TYPE::ScopeChain *next_);
  ~LocalEnv();
  CRB_TYPE::ScopeChain * get_scope_chain();
  CRB_TYPE::ScopeChain *_scope_chain; // local_variable here
  map<string, CRB_TYPE::Value*> _global_declare_map;
};

class Interpreter {
 private:
  class Heap;
  class Stack;
  class  Environment;
 public:
  static Interpreter* getInstance();
  void before_hook();
  void dealloc();
  void chain_statement_list(Statement* statement);
  void set_line(int line_);
  int get_line();
  void execute();
  Interpreter::Heap* get_heap();
  Interpreter::Stack* get_stack();
  Interpreter::Environment* get_environment();
 private:
  class Environment {
   public:
    Environment();
    ~Environment();
    LocalEnv* get_top_env();
    LocalEnv* get_use_env();
    void use_env(LocalEnv* use_env);
    void alloc_env(CRB_TYPE::ScopeChain* next_);
    void dealloc_env();
    void add_function(FunctionDefinition* fd);
    void add_fake_method(FunctionDefinition* fd);
    void add_variable(string name, CRB_TYPE::Value*);
    void add_global_declare(string name);
    void assign_variable(string name, CRB_TYPE::Value*);
    CRB_TYPE::Value* search_variable(string name);
    void assign(string name, CRB_TYPE::Value*);
    FunctionDefinition* search_function(string name);
    FunctionDefinition* search_fake_method(string name);
   private:
    friend class Interpreter::Heap;
    LocalEnv* _use_env;
    bool _use_caller_env;
    vector<LocalEnv*> _local_env_vec;
    CRB_TYPE::ScopeChain *_scope_chain; // local_variable here
    map<string, CRB_TYPE::Value*> _global_declare_map;
    map<string, FunctionDefinition*> _global_function_map;
    map<string, FunctionDefinition*> _global_fake_method_map;
    map<string, CRB_TYPE::Value*> _global_variable_map;
  };
  class Heap {
   public:
    // the heap only calculate the current size, actually alloc is controlled by OS
    Heap();
    ~Heap();
    CRB_TYPE::Object* alloc(CRB_TYPE::ObjectType);
    CRB_TYPE::Object* alloc(string* string_value, bool is_literal_);
    int _current_threshold;
    int _threshold_size;
    int _current_size;
    int _gc_times;
   private:
    void garbage_collect();
    void mark_from_root_set();
    void mark_value(CRB_TYPE::Value* value);
    void sweep();
    void print();
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
    friend class Interpreter::Heap;
    vector<CRB_TYPE::Value*> _stack_vec; 
  };
  Heap *_heap;
  Stack * _stack;
  Environment *_environment;
  StatementList *_statement_list;
  int _line;
  static Interpreter* _instance;
  Interpreter();
  ~Interpreter();
};

  
} // end CRB namespace

#endif /* CRB_H */
