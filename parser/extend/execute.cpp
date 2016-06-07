#include "execute.h"
#include "string"
#include "CRB.h"
using namespace std;
CRB_TYPE::Value* value_copy(CRB_TYPE::Value* value) {
  CRB_TYPE::Value* result;
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  switch(value->type) {
    case CRB_TYPE::BOOLEAN_VALUE: {
      return new CRB_TYPE::BooleanValue(dynamic_cast<CRB_TYPE::BooleanValue*>(value)->boolean_value);
    } 
    case CRB_TYPE::INT_VALUE: {
      return new CRB_TYPE::IntValue(dynamic_cast<CRB_TYPE::IntValue*>(value)->int_value);
    } 
    case CRB_TYPE::DOUBLE_VALUE: {
      return new CRB_TYPE::DoubleValue(dynamic_cast<CRB_TYPE::DoubleValue*>(value)->double_value);
    } 
    case CRB_TYPE::NULL_VALUE: {
      return new CRB_TYPE::Value(); // default NULL
    } 
    case CRB_TYPE::ARRAY_VALUE: {
      // return new 
    } 
    case CRB_TYPE::STRING_VALUE: {
      string* tem_string_ptr = dynamic_cast<CRB_TYPE::String*>(value)->string_value;
      result = Iheap->alloc(tem_string_ptr, false);
      return result;
    } 
    case CRB_TYPE::ASSOC_VALUE: {
      // return new 
    } 
    case CRB_TYPE::CLOSURE_VALUE: {
      // return new 
    } 
    case CRB_TYPE::FAKE_METHOD_VALUE: {
      // return new 
    } 
    case CRB_TYPE::SCOPE_CHAIN_VALUE: {
      // return new 
    } 
  }
}

