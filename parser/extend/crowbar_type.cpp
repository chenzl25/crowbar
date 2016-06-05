#include "crowbar_type.h"
#include "crowbar_util.h"
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
Closure::Closure(FunctionDefinition * f, Object *env):Value(CRB_TYPE::CLOSURE_VALUE) {
  function = f;
  environment = env;
}
Closure::~Closure(){
  // TODO
}
void Closure::print() {
  cout << "value type : " << CRB::value_type_to_string(this->type) << endl;
  // cout << "value value: "  << endl;
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
