#include "CRB.h"
#include "../lexer/src/util.h"
namespace CRB {

Interpreter::Interpreter() {

}
Interpreter::~Interpreter() {

}

Interpreter* Interpreter::getInstance() {
  return new Interpreter();
}
void Interpreter::chain_statement_list(Statement* statement) {

}

void Interpreter::add_function(FunctionDefinition* fd) {
  if (_function_map.count(fd->name)) {
    error("multi definition of " + *(fd->name));
  }
  _function_map[fd->name] = fd;
  cout << "add function: " << *(fd->name) << endl;
}
Interpreter *create_interpreter(void) {

}
void compile(Interpreter *interpreter, string code_path) {

}
void interpret(Interpreter *interpreter) {

}
void dispose_interpreter(Interpreter *interpreter) {

}

Interpreter::Heap::Heap() {

}
CRB_TYPE::Object* Interpreter::Heap::alloc(CRB_TYPE::ObjectType) {
  
}

} // CRB

