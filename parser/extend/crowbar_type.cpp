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
    CRB::statement_result_value_delete(value);
  }
}

Closure::Closure(FunctionDefinition *function_definition_,
                 ScopeChain *scope_chain_): Value(CRB_TYPE::CLOSURE_VALUE) {
  function_definition = function_definition_;
  scope_chain = scope_chain_;
}
Closure::~Closure(){
  // this function maybe called by environment, just delete itself without its member
  // don't delete function_definition.  remain for environment
  // don't delete scope_chain_;         remain for heap
}
void Closure::print() {
  cout << "value type : " << CRB::value_type_to_string(Value::type) << endl;
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
  // let Heap delete
  // if (frame) delete frame;
  // if (next)  delete next;
}

void ScopeChain::print() {
  cout << "value type : " << CRB::value_type_to_string(Value::type) << endl;
}

Assoc::Assoc() : Object(CRB_TYPE::ASSOC_VALUE) {
  int ref_cnt = 1;
}
Assoc::~Assoc() {
  // we can delete the Value, because Value only delete itself 
  // the Object in the Value will be delete by Heap
  CRB::assert(ref_cnt == 0, "when delete the Assoc the ref_cnt should equal 0");
  for (auto it = member_map.begin(); it != member_map.end(); it++) {
    CRB::assoc_value_delete(it->second);
  }
}
void Assoc::add_member(string name, Value* value) {
  CRB::assert(member_map.count(name) == 0, "add_member: the member exist before adding");
  member_map[name] = value;
}

Value* Assoc::search_member(string name) {
  if (member_map.count(name)) {
    return member_map[name];
  }
  return NULL;
}
void Assoc::assign_member(string name, Value* value) {
  CRB::assert(member_map.count(name), "the value be assigned should exist");
  CRB::non_object_delete(member_map[name]); // delete is ok the Object is independent from Value now
  member_map[name] = value;
}
void Assoc::print() {
  cout << "value type : " << CRB::value_type_to_string(Value::type) << endl;
}
Array::Array() : Object(CRB_TYPE::ARRAY_VALUE) {
  ref_cnt = 1;
}
Array::Array(int size) : Object(CRB_TYPE::ARRAY_VALUE) {
  ref_cnt = 1;
  vec.resize(size);
}
Array::~Array() {
  CRB::assert(ref_cnt == 0, "when delete the Array the ref_cnt should equal 0");
  for (int i = 0; i < vec.size(); i++) {
    CRB::array_value_delete(vec[i]);
  }
}
void Array::print() {
  cout << "value type : " << CRB::value_type_to_string(Value::type) << endl;
  for (int i = 0; i < vec.size(); i++) {
    vec[i]->print();
  }
}

FakeMethod::FakeMethod() : Value(CRB_TYPE::FAKE_METHOD_VALUE) {
  object = NULL;
  method_name = NULL;
}
FakeMethod::FakeMethod(Value *object_, string *method_name_) : Value(CRB_TYPE::FAKE_METHOD_VALUE) {
  object = object_;
  method_name = method_name_;
}
FakeMethod::~FakeMethod() {
  // do nothing 
  // object must be Array or String, ( maybe we can add Assoc later...)
  // object from others. we can't delete
  // method_name is from MemberExpression. we can't delete
}
void FakeMethod::print() {
  cout << "value type : " << CRB::value_type_to_string(Value::type) << endl;
  object->print();
  cout << "method name: " << *method_name << endl;
}
// Object    *object;
// String    *method_name;
