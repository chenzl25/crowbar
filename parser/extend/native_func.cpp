#include <vector>
#include <iostream>
#include <cstdio>
#include <string>
#include "native_func.h"
#include "crowbar_util.h"
#include "CRB.h"

using namespace std;

namespace CRB {

CRB_TYPE::Value* print_proc(int arg_cnt, int line_number) {
  string line_string = std::to_string(line_number);
  CRB::assert(arg_cnt == 1, line_string + ": print: need 1 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  string s = CRB::value_to_string(Istack->peek(0), line_number);
  char *buf = new char[s.length()];
  int j = 0;
  for (int i = 0; i < s.length(); i++, j++) {
    if (s[i] == '\\') {
      if (i+1 >= s.length()) break;
      switch(s[i+1]) {
        case 'n' : 
          buf[j] = '\n';
          break;
        case 't' :  
          buf[j] = '\t';

        case 'b' :
          buf[j] = '\b';
          break;
        case '\\':
          buf[j] = '\\';
          break;
        case '\"':
          buf[j] = '\"';
          break;
        case '\'':
          buf[j] = '\'';
          break;
        default:
          CRB::error(line_string + ": can't not escape char :" + s[i]);
      }
      i++;
    } else {
      buf[j] = s[i];
    }
  }
  buf[j] = '\0';
  printf("%s", buf);
  delete []buf;
  return new CRB_TYPE::Value();
}
FunctionDefinition* create_print_native_function() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("print");
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = print_proc;
  return fd;
}

CRB_TYPE::Value* new_array_proc(int arg_cnt, int line_number) {
  string line_string = std::to_string(line_number);
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  int array_size;
  if (arg_cnt == 0) {
    array_size = 0;
  } else {
    CRB::assert(arg_cnt == 1, line_string + ": Array: need 1 argument");
    CRB::assert(Istack->size() >= 1, line_string + ": Array: the eval stack should have 1 value at least ");
    CRB::assert(Istack->peek(0)->type == CRB_TYPE::INT_VALUE, line_string + ": Array: argument should be interger");
    array_size = dynamic_cast<CRB_TYPE::IntValue*>(Istack->peek(0))->int_value;
  }
  auto array_value = dynamic_cast<CRB_TYPE::Array*>(Iheap->alloc(CRB_TYPE::ARRAY_OBJECT));
  array_value->vec.resize(array_size);
  for (int i = 0; i < array_size; i++) {
    array_value->vec[i] = new CRB_TYPE::Value();
  }
  return array_value;
}

FunctionDefinition* create_new_array_native_function() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("Array"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = new_array_proc;
  return fd;
}

CRB_TYPE::Value* new_object_proc(int arg_cnt, int line_number) {
  string line_string = std::to_string(line_number);
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 0, line_string + ": Object: need 0 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  return Iheap->alloc(CRB_TYPE::ASSOC_OBJECT);
}

FunctionDefinition* create_new_object_native_function() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("Object"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = new_object_proc;
  return fd;
}

CRB_TYPE::Value* from_char_code_proc(int arg_cnt, int line_number) {
  string line_string = std::to_string(line_number);
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt >= 1, line_string + ": fromCharCode: need 1 argument at least");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  CRB::assert(Istack->size() >= arg_cnt, line_string + ": fromCharCode: the eval stack should have " + std::to_string(arg_cnt) +" value at least ");
  string tem_string;
  for (int i = 0; i < arg_cnt; i++) {
    CRB::assert(Istack->peek(i)->type == CRB_TYPE::INT_VALUE, line_string + ": fromCharCode: argument should be a interger");
    int char_code = dynamic_cast<CRB_TYPE::IntValue*>(Istack->peek(i))->int_value;
    tem_string += static_cast<char>(char_code);
  }
  return Iheap->alloc(&tem_string, false);
}

FunctionDefinition* create_from_char_code_function() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("fromCharCode"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = from_char_code_proc;
  return fd;
}

  




CRB_TYPE::Value* require_proc(int arg_cnt, int line_number) {
  string line_string = std::to_string(line_number);
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt >= 1, line_string + ": require: need 1 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  CRB::assert(Istack->size() >= 1, line_string + ": require: the eval stack should have 1 value");
  CRB::assert(Istack->peek(0)->type == CRB_TYPE::STRING_VALUE, line_string + ": require: the argument should be String");
  string require_file_path = *dynamic_cast<CRB_TYPE::String*>(Istack->peek(0))->string_value;
  string old_run_code_path = CRB::Interpreter::getInstance()->get_current_run_code_path();
  string result_path = CRB::calculate_path(old_run_code_path, require_file_path);
  if (!CRB::file_exists(result_path)) {
    CRB::error(line_string + ": " + require_file_path + " doesn't exists");
  }
  CRB::Interpreter::getInstance()->parse(result_path);
  CRB::Interpreter::getInstance()->set_current_run_code_path(old_run_code_path);
  return new CRB_TYPE::Value(); // NULL, maybe we will improve the require function to return a variable in the future.
}

FunctionDefinition* create_require_function() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("require"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = require_proc;
  return fd;
}


void add_native_function() {
  auto Ienv = CRB::Interpreter::getInstance()->get_environment();
  Ienv->add_function(create_print_native_function());
  Ienv->add_function(create_new_array_native_function());
  Ienv->add_function(create_new_object_native_function());
  Ienv->add_function(create_from_char_code_function());
  Ienv->add_function(create_require_function());
}



} // CRB

