#include "crowbar_type.h"

using namespace CRB_TYPE;
Value::Value() {
  type = CRB_TYPE::NULL_VALUE;
}
Value::Value(CRB_TYPE::ValueType type_) {
  type = type_;
}
IntValue::IntValue(int int_value_):Value(CRB_TYPE::INT_VALUE) {
  int_value = int_value_;
}
DoubleValue::DoubleValue(double double_value_):Value(CRB_TYPE::DOUBLE_VALUE) {
  double_value = double_value_;
}
BooleanValue::BooleanValue(bool boolean_value_):Value(CRB_TYPE::BOOLEAN_VALUE) {
  boolean_value = boolean_value_;
}

Closure::Closure(FunctionDefinition * f, Object *env):Value(CRB_TYPE::CLOSURE_VALUE) {
  function = f;
  environment = env;
}

Object::Object(CRB_TYPE::ObjectType type_):Value(CRB_TYPE::OBJECT_VALUE) {
  type = type_;
  marked = false;
  // Array       array;
  // Assoc       assoc;
  // ScopeChain  scope_chain;
}

String::String(std::string *string_value_, bool is_literal_):Object(CRB_TYPE::STRING_OBJECT) {
  string_value = string_value_;
  is_literal = is_literal_;
}

