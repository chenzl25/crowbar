#include "crowbar_type.h"
#include "crowbar_util.h"
#include "crowbar.h"
#include "CRB.h"
#include <iostream>
using namespace std;
using namespace CRB_TYPE;
Value::Value() {
  type = CRB_TYPE::NULL_VALUE;
}
Value::Value(CRB_TYPE::ValueType type_) {
  type = type_;
}
void Value::print() {
  cout << "value type : " << CRB::value_type_to_string(this->type) << endl;
  cout << "value value: " << "NULL" << endl; // default Value is NULL;
}
IntValue::IntValue(int int_value_):Value(CRB_TYPE::INT_VALUE) {
  int_value = int_value_;
}
IntValue::~IntValue(){

}
void IntValue::print() {
  cout << "value type : " << CRB::value_type_to_string(this->type) << endl;
  cout << "value value: " << this->int_value << endl;
}
DoubleValue::DoubleValue(double double_value_):Value(CRB_TYPE::DOUBLE_VALUE) {
  double_value = double_value_;
}
DoubleValue::~DoubleValue(){
  
}
void DoubleValue::print() {
  cout << "value type : " << CRB::value_type_to_string(this->type) << endl;
  cout << "value value: " << this->double_value << endl;
}
BooleanValue::BooleanValue(bool boolean_value_):Value(CRB_TYPE::BOOLEAN_VALUE) {
  boolean_value = boolean_value_;
}
BooleanValue::~BooleanValue(){
  
}
void BooleanValue::print() {
  cout << "value type : " << CRB::value_type_to_string(this->type) << endl;
  cout << "value value: " << this->boolean_value << endl;
}

Object::Object(CRB_TYPE::ValueType type_):Value(type_) {
  switch (type_) {
    case CRB_TYPE::STRING_VALUE:
      type = CRB_TYPE::STRING_OBJECT;
      break;
    case CRB_TYPE::ARRAY_VALUE:
      type = CRB_TYPE::ARRAY_OBJECT;
      break;
    case CRB_TYPE::ASSOC_VALUE:
      type = CRB_TYPE::ASSOC_OBJECT;
      break;
    case CRB_TYPE::SCOPE_CHAIN_VALUE:
      type = CRB_TYPE::SCOPE_CHAIN_OBJECT;
      break;
    default:
      CRB::error("type for Object constructor should be ValueType");
  }
  marked = false;
  // Array       array;
  // Assoc       assoc;
  // ScopeChain  scope_chain;
}
Object::~Object(){
  
}
void Object::print() {
  cout << "value type : " << CRB::value_type_to_string(Value::type) << endl;
  cout << "value value: " << "Object only" << endl;
}
String::String(std::string *string_value_, bool is_literal_):Object(CRB_TYPE::STRING_VALUE) {
  string_value = string_value_;
  is_literal = is_literal_;
}
String::~String() {
  if (!is_literal) {
    delete string_value;
  }
}
void String::print() {
  cout << "value type : " << CRB::value_type_to_string(Value::type) << endl;
  cout << "value value: " << *(this->string_value) << endl;
}
StatementResult::StatementResult(StatementResultType type_) {
  CRB::assert(type != CRB_TYPE::RETURN_STATEMENT_RESULT,
         "the statement result without value should not return ");
  type = type_;
  value = NULL;
}
StatementResult::StatementResult(Value* value_) {
  type = CRB_TYPE::RETURN_STATEMENT_RESULT;
  CRB::assert(value_ != NULL, "the statement result _value should not NULL");
  value = value_;
}
StatementResult::~StatementResult() {
  if (value) {
    delete value; // maybe need to change when consider other value from heap
  }
}

Closure::Closure(FunctionDefinition * function_definition_,
                 ScopeChain *scope_chain_):Value(CRB_TYPE::CLOSURE_VALUE) {
  function_definition = function_definition_;
  scope_chain = scope_chain_;
}
Closure::~Closure(){
  if (function_definition) delete function_definition;
  if (scope_chain) delete scope_chain;
}
void Closure::print() {
  cout << "value type : " << CRB::value_type_to_string(this->type) << endl;
  // cout << "value value: "  << endl;
}
ScopeChain::ScopeChain() : Object(CRB_TYPE::SCOPE_CHAIN_VALUE) {
  frame = NULL;
  next = NULL; 
}
ScopeChain::ScopeChain(ScopeChain  *next_) : Object(CRB_TYPE::SCOPE_CHAIN_VALUE) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  frame = dynamic_cast<Assoc*>(Iheap->alloc(CRB_TYPE::ASSOC_OBJECT));
  next = next_;
}
ScopeChain::ScopeChain(Assoc  *frame_, ScopeChain  *next_) : Object(CRB_TYPE::SCOPE_CHAIN_VALUE) {
  frame = frame_;
  next = next_;
}
ScopeChain::~ScopeChain() {
  if (frame) delete frame;
  if (next)  delete next;
}

void ScopeChain::print() {
}

Assoc::Assoc() : Object(CRB_TYPE::ASSOC_VALUE) {

}
Assoc::~Assoc() {
  for (auto it : member_map) {
    delete it.second;
  }
}
void Assoc::add_member(string name, Value* value) {
  member_map[name] = value;
}

Value* Assoc::search_member(string name) {
  return member_map[name];
}
void Assoc::assign_member(string name, Value* value) {
  CRB::assert(member_map.count(name), "the value be assigned should exist");
  if (is_object_value(member_map[name]->type)) {
    // remain for the Heap to delete
  } else {
    delete member_map[name];
  }
  member_map[name] = value;
}
void Assoc::print() {

}

