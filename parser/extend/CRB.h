#ifndef CRB_H
#define CRB_H
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <set>
#include "crowbar_type.h"
#include "crowbar.h"
using namespace std;
namespace CRB {

class Interpreter {
public:
  static Interpreter* getInstance();
  void chain_statement_list(Statement* statement);
  CRB_TYPE::Value eval_constant(CRB_TYPE::ExpressionType operator_type,
                                         Expression *left,
                                         Expression *right);
  void eval();
  void add_function(FunctionDefinition* fd);
private:
  struct Heap {
    Heap();
    CRB_TYPE::Object* alloc(CRB_TYPE::ObjectType);

    set<CRB_TYPE::Object*> heap_set;
  };
  Heap _heap;
  map<string*, FunctionDefinition*> _function_map;
  map<string*, CRB_TYPE::Value> _variable_map;
  stack<CRB_TYPE::Value> _stack;
  StatementList _statement_list;
  Interpreter();
  ~Interpreter();
};

Interpreter *create_interpreter(void);
void compile(Interpreter *interpreter, string code_path);
void interpret(Interpreter *interpreter);
void dispose_interpreter(Interpreter *interpreter);
  
} // end CRB namespace

#endif /* CRB_H */
