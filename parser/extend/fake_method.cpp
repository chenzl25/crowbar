#include <vector>
#include <iostream>
#include <cstdio>
#include <string>
#include "fake_method.h"
#include "crowbar_util.h"
#include "CRB.h"
#include "execute.h"
using namespace std;

namespace CRB {




CRB_TYPE::Value* array_size_proc(int arg_cnt) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 0, "fake method size : need 0 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  // we assume the object is on the top of the stack
  assert(Istack->size() >= 1, "fake method size : stack should have 1 value at least");
  assert(Istack->peek(0)->type == CRB_TYPE::ARRAY_VALUE, "fake method size : should use only on Array");
  auto array_value = dynamic_cast<CRB_TYPE::Array*>(Istack->peek(0));
  int size = array_value->vec.size();
  return new CRB_TYPE::IntValue(size);
}

FunctionDefinition* create_array_size_method() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("size"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = array_size_proc;
  return fd;
}

CRB_TYPE::Value* array_resize_proc(int arg_cnt) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 1, "fake method resize : need 1 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  // we assume the object is on the top of the stack
  assert(Istack->size() >= 2, "fake method resize : stack should have 2 value at least");
  assert(Istack->peek(0)->type == CRB_TYPE::ARRAY_VALUE, "fake method resize : should use only on Array");
  assert(Istack->peek(1)->type == CRB_TYPE::INT_VALUE, "fake method resize : argument should be a integer");
  auto array_value = dynamic_cast<CRB_TYPE::Array*>(Istack->peek(0));
  auto resize_value = dynamic_cast<CRB_TYPE::IntValue*>(Istack->peek(1));
  array_value->vec.resize(resize_value->int_value);
  return new CRB_TYPE::Value(); // default NULL
}

FunctionDefinition* create_array_resize_method() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("resize"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = array_resize_proc;
  return fd;
}

CRB_TYPE::Value* array_insert_proc(int arg_cnt) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 2, "fake method insert :  need 2 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  // we assume the object is on the top of the stack
  assert(Istack->size() >= 3, "fake method insert : stack should have 3 value at least");
  assert(Istack->peek(0)->type == CRB_TYPE::ARRAY_VALUE, "fake method insert : should use only on Array");
  assert(Istack->peek(1)->type == CRB_TYPE::INT_VALUE, "fake method insert : first argument should be a integer");
  
  auto array_value = dynamic_cast<CRB_TYPE::Array*>(Istack->peek(0));
  auto insert_pos_value = dynamic_cast<CRB_TYPE::IntValue*>(Istack->peek(1));
  auto insert_item_value = value_copy(Istack->peek(2));
  assert(insert_pos_value->int_value >= 0 && 
         insert_pos_value->int_value <= array_value->vec.size(),
         "insert pos should only between 0 and array.size");
  array_value->vec.insert(array_value->vec.begin() + insert_pos_value->int_value, insert_item_value);
  return new CRB_TYPE::Value(); // default NULL
}

FunctionDefinition* create_array_insert_method() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("insert"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = array_insert_proc;
  return fd;
}

CRB_TYPE::Value* array_push_proc(int arg_cnt) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 1, "fake method push : need 1 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  // we assume the object is on the top of the stack
  assert(Istack->size() >= 2, "fake method push : stack should have 2 value at least");
  assert(Istack->peek(0)->type == CRB_TYPE::ARRAY_VALUE, "fake method push : should use only on Array");
  auto array_value = dynamic_cast<CRB_TYPE::Array*>(Istack->peek(0));
  auto push_item_value = value_copy(Istack->peek(1));
  array_value->vec.push_back(push_item_value);
  return new CRB_TYPE::Value(); // default NULL
}

FunctionDefinition* create_array_push_method() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("push"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = array_push_proc;
  return fd;
}

CRB_TYPE::Value* array_pop_proc(int arg_cnt) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 0, "fake method pop : need 1 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  // we assume the object is on the top of the stack
  assert(Istack->size() >= 1, "fake method pop : stack should have 1 value at least");
  assert(Istack->peek(0)->type == CRB_TYPE::ARRAY_VALUE, "fake method pop : should use only on Array");
  auto array_value = dynamic_cast<CRB_TYPE::Array*>(Istack->peek(0));
  auto pop_value =  array_value->vec[array_value->vec.size()-1]; // we don't delete, so can use to return
  array_value->vec.erase(array_value->vec.end()-1);
  return pop_value; // default NULL
}

FunctionDefinition* create_array_pop_method() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("pop"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = array_pop_proc;
  return fd;
}

CRB_TYPE::Value* array_erase_proc(int arg_cnt) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 1, "fake method erase : need 1 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  // we assume the object is on the top of the stack
  assert(Istack->size() >= 2, "fake method erase : stack should have 2 value at least");
  assert(Istack->peek(0)->type == CRB_TYPE::ARRAY_VALUE, "fake method erase : should use only on Array");
  assert(Istack->peek(1)->type == CRB_TYPE::INT_VALUE, "fake method erase : argument should be a integer");
  auto array_value = dynamic_cast<CRB_TYPE::Array*>(Istack->peek(0));
  auto erase_pos_value = dynamic_cast<CRB_TYPE::IntValue*>(Istack->peek(1));
  assert(erase_pos_value->int_value >= 0 &&
         erase_pos_value->int_value < array_value->vec.size(), "erase pos should only between 0 and array.size-1");
  auto erase_value =  array_value->vec[erase_pos_value->int_value]; // we don't delete, so can use to return
  array_value->vec.erase(array_value->vec.begin() + erase_pos_value->int_value);
  return erase_value; // default NULL
}

FunctionDefinition* create_array_erase_method() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("erase"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = array_erase_proc;
  return fd;
}

CRB_TYPE::Value* string_length_proc(int arg_cnt) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 0, "fake method length : need 0 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  // we assume the object is on the top of the stack
  assert(Istack->size() >= 1, "fake method length : stack should have 1 value at least");
  assert(Istack->peek(0)->type == CRB_TYPE::STRING_VALUE, "fake method length : should use only on String");
  auto string_value = dynamic_cast<CRB_TYPE::String*>(Istack->peek(0));
  int length = string_value->string_value->length();
  return new CRB_TYPE::IntValue(length);
}

FunctionDefinition* create_string_length_method() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("length"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = string_length_proc;
  return fd;
}

CRB_TYPE::Value* string_slice_proc(int arg_cnt) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  CRB::assert(arg_cnt == 0 || arg_cnt == 1 || arg_cnt == 2, "fake method slice : need 0, 1 or 2 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  // we assume the object is on the top of the stack
  assert(Istack->size() >= 1, "fake method slice : stack should have 1 value at least");
  assert(Istack->peek(0)->type == CRB_TYPE::STRING_VALUE, "fake method slice : should use only on String");
  auto string_value = dynamic_cast<CRB_TYPE::String*>(Istack->peek(0));
  string result_string;
  int string_value_length = string_value->string_value->length();
  if (arg_cnt == 0) {
    result_string = *string_value->string_value;
  } else if (arg_cnt == 1) {
    assert(Istack->peek(1)->type == CRB_TYPE::INT_VALUE, "fake method slice : argument should be a integer");
    auto pos = dynamic_cast<CRB_TYPE::IntValue*>(Istack->peek(1))->int_value;
    if (pos >= string_value_length) {
      result_string = "";
    } else if (pos >= 0 && pos < string_value_length) {
      result_string = string_value->string_value->substr(pos, string_value_length - pos);
    } else if (pos < 0 && pos > -string_value_length) {
      pos += string_value_length;
      result_string = string_value->string_value->substr(pos, string_value_length - pos);
    } else {
      result_string = *string_value->string_value;
    }
  } else if (arg_cnt == 2) {
    assert(Istack->peek(1)->type == CRB_TYPE::INT_VALUE, "fake method slice : first argument should be a integer");
    assert(Istack->peek(2)->type == CRB_TYPE::INT_VALUE, "fake method slice : second argument should be a integer");
    auto first_pos = dynamic_cast<CRB_TYPE::IntValue*>(Istack->peek(1))->int_value;
    auto second_pos = dynamic_cast<CRB_TYPE::IntValue*>(Istack->peek(2))->int_value;
    if (first_pos >= string_value_length) {
      first_pos = string_value_length;
    } else if (first_pos >= 0 && first_pos < string_value_length) {
      // do nothing
    } else if (first_pos < 0 && first_pos > -string_value_length) {
      first_pos += string_value_length;
    } else {
      first_pos = 0;
    }
    if (second_pos >= string_value_length) {
      second_pos = string_value_length;
    } else if (second_pos >= 0 && second_pos < string_value_length) {
      // do nothing
    } else if (second_pos < 0 && second_pos > -string_value_length) {
      second_pos += string_value_length;
    } else {
      second_pos = 0;
    }
    if (second_pos > first_pos) {
      result_string = string_value->string_value->substr(first_pos, second_pos - first_pos);
    } else {
      result_string = "";
    }
  }
  string* result_string_ptr = new string(result_string);
  return Iheap->alloc(result_string_ptr, false);
}

FunctionDefinition* create_string_slice_method() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("slice"); // we name it Array
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = string_slice_proc;
  return fd;
}

void add_fake_method() {
  auto Ienv = CRB::Interpreter::getInstance()->get_environment();
  Ienv->add_fake_method(create_array_size_method());
  Ienv->add_fake_method(create_array_resize_method());
  Ienv->add_fake_method(create_array_insert_method());
  Ienv->add_fake_method(create_array_push_method());
  Ienv->add_fake_method(create_array_pop_method());
  Ienv->add_fake_method(create_array_erase_method());
  Ienv->add_fake_method(create_string_length_method());
  Ienv->add_fake_method(create_string_slice_method());
}



} // CRB

