#include "CRB.h"

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
Interpreter *create_interpreter(void) {

}
void compile(Interpreter *interpreter, string code_path) {

}
void interpret(Interpreter *interpreter) {

}
void dispose_interpreter(Interpreter *interpreter) {

}


} // CRB

