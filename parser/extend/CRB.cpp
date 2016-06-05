#include "CRB.h"
#include "../lexer/src/util.h"

namespace CRB {

Interpreter* Interpreter::_instance = NULL;

Interpreter::Interpreter() {
  _heap = new Heap();
  _stack = new Stack();
}
Interpreter::~Interpreter() {
  delete _heap;
  delete _stack;
}


Interpreter* Interpreter::getInstance() {
  if (_instance == NULL) {
    _instance = new Interpreter();
  } 
  return _instance;
}

void Interpreter::chain_statement_list(Statement* statement) {

}
void Interpreter::set_line(int line_) {
  _line = line_;
}
int Interpreter::get_line() {
  return _line;
}
Interpreter::Heap* Interpreter::get_heap() {
  return _heap;
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
Interpreter::Stack* Interpreter::get_stack() {
  return _stack;
}
Interpreter::Stack::Stack() {

}
Interpreter::Stack::~Stack() {
  // TODO
}
int Interpreter::Stack::size() {
  return _stack_vec.size();
}
CRB_TYPE::Value* Interpreter::Stack::pop() {
  auto result = _stack_vec[_stack_vec.size()-1];
  _stack_vec.erase(_stack_vec.end()-1);
  return result;
}
CRB_TYPE::Value* Interpreter::Stack::peek(int pos) {
  assert(_stack_vec.size() > pos, "can't peek the stack over the size");
  return _stack_vec[_stack_vec.size() - pos - 1];
}
void Interpreter::Stack::push(CRB_TYPE::Value* v) {
  _stack_vec.push_back(v);
}
Interpreter::Heap::Heap() {
  _threshold = (1024 * 256); // 256K
  _current_size = 0;
}
Interpreter::Heap::~Heap() {
  // TODO

}
CRB_TYPE::Object* Interpreter::Heap::alloc(CRB_TYPE::ObjectType type_) {
  switch (type_) {
    case CRB_TYPE::STRING_OBJECT:
      error("should use another string alloc");
      break;
    case CRB_TYPE::ARRAY_OBJECT:

      break;
    case CRB_TYPE::SCOPE_CHAIN_OBJECT:
      break;
    case CRB_TYPE::ASSOC_OBJECT:
      break;
    default:
      error("heap alloc type error");
  }
}
CRB_TYPE::Object* Interpreter::Heap::alloc(string* string_value, bool is_literal_) {
  if (is_literal_) { // when gc we do not sweep literal
    return new CRB_TYPE::String(string_value, is_literal_);
  } else {
    _current_size += sizeof(CRB_TYPE::String) + string_value->length();
    return new CRB_TYPE::String(new string(*string_value), is_literal_);
  }
}

} // CRB

