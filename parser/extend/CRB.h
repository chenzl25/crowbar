#ifndef H
#define H
#include <iostream>
#include <string>
#include "crowbar_type.h"
#include "crowbar.h"
using namespace std;
namespace CRB {

class Interpreter {
public:
  static Interpreter* getInstance();
  void chain_statement_list(Statement* statement);
private:

  Interpreter();
  ~Interpreter();
};

Interpreter *create_interpreter(void);
void compile(Interpreter *interpreter, string code_path);
void interpret(Interpreter *interpreter);
void dispose_interpreter(Interpreter *interpreter);
  
} // end CRB namespace

#endif /* CRB_H */
