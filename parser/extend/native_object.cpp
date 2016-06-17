#include "native_object.h"
#include "CRB.h"
#include "crowbar_type.h"
#include "crowbar_util.h"


namespace CRB {

CRB_TYPE::Value* random_method(int arg_cnt, int line_number) {
  string line_string = std::to_string(line_number);
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 0, line_string + ": Math.random : need 0 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  auto Ienv = Interpreter::getInstance()->get_environment();
  // we assume the object is on the top of the stack
  CRB::assert(Istack->size() >= 1, line_string  + ": Math.random : stack should have 1 value at least");
  return new CRB_TYPE::DoubleValue(CRB::uniform_rand());
}

FunctionDefinition* create_random_method() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("random"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = random_method;
  return fd;
}


void add_math_object() {
  auto Ienv = Interpreter::getInstance()->get_environment();
  auto Iheap = Interpreter::getInstance()->get_heap();
  auto math_assoc_value = dynamic_cast<CRB_TYPE::Assoc*>(Iheap->alloc(CRB_TYPE::ASSOC_OBJECT));
  math_assoc_value->add_member("PI", new CRB_TYPE::DoubleValue(3.141592653589793));
  math_assoc_value->add_member("E", new CRB_TYPE::DoubleValue(2.718281828459045));
  auto random_closure_value = new CRB_TYPE::Closure(create_random_method(), NULL);
  math_assoc_value->add_member("random", random_closure_value);
  Ienv->add_native_variable("Math", math_assoc_value);
  return;
}

void add_native_object() {
  add_math_object();
}

  
}