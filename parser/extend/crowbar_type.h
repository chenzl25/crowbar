#ifndef EXPRESSION_TYPE_H
#define EXPRESSION_TYPE_H
#include <string>
using namespace std;
namespace CRB_TYPE  {

enum Boolean {
  FALSE = 0,
  TRUE = 1
};
enum ValueType {
  BOOLEAN_VALUE = 1,
  INT_VALUE,
  DOUBLE_VALUE,
  STRING_VALUE,
  NATIVE_POINTER_VALUE,
  NULL_VALUE
};


struct NativePointer{
  struct NativePointerInfo {
      char        *name;
  };
  
  struct NativePointerInfo       *info;
  void                    *pointer;
};

struct Value {
  CRB_TYPE::ValueType  type;
  union {
    CRB_TYPE::Boolean      boolean_value;
    int               int_value;
    double            double_value;
    string            *string;
    CRB_TYPE::NativePointer     native_pointer;
  } u;
};

enum ExpressionType {
    BOOLEAN_EXPRESSION = 1,
    INT_EXPRESSION,
    DOUBLE_EXPRESSION,
    STRING_EXPRESSION,
    IDENTIFIER_EXPRESSION,
    ASSIGN_EXPRESSION,
    ADD_EXPRESSION,
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    EQ_EXPRESSION,
    NE_EXPRESSION,
    GT_EXPRESSION,
    GE_EXPRESSION,
    LT_EXPRESSION,
    LE_EXPRESSION,
    LOGICAL_AND_EXPRESSION,
    LOGICAL_OR_EXPRESSION,
    MINUS_EXPRESSION,
    FUNCTION_CALL_EXPRESSION,
    METHOD_CALL_EXPRESSION,
    NULL_EXPRESSION,
    ARRAY_EXPRESSION,
    INDEX_EXPRESSION,
    INCREMENT_EXPRESSION,
    DECREMENT_EXPRESSION,
    EXPRESSION_TYPE_COUNT_PLUS_1
} ;

} // CRB_TYPE

#endif