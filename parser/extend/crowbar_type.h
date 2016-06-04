#ifndef EXPRESSION_TYPE_H
#define EXPRESSION_TYPE_H
#include <string>
using namespace std;
namespace CRB_TYPE  {

class ParameterList;
class Block;
class FunctionDefinition;

enum ValueType {
  BOOLEAN_VALUE = 1,
  INT_VALUE,
  DOUBLE_VALUE,
  STRING_VALUE,
  NULL_VALUE,
  ARRAY_VALUE,
  ASSOC_VALUE,
  CLOSURE_VALUE,
  FAKE_METHOD_VALUE,
  SCOPE_CHAIN_VALUE
};

enum FunctionDefinitionType {
    CROWBAR_FUNCTION_DEFINITION = 1,
    NATIVE_FUNCTION_DEFINITION,
    FUNCTION_DEFINITION_TYPE_COUNT_PLUS_1
};

// struct FakeMethod {
//     string    *method_name;
//     Object    *object;
// } ;

// struct CRB_Array_tag {
//     int         size;
//     int         alloc_size;
//     CRB_Value   *array;
// };


// typedef struct {
//     Object  *frame; /* Assoc */
//     Object  *next;  /* ScopeChain */
// } ScopeChain;


enum ObjectType {
    ARRAY_OBJECT = 1,
    STRING_OBJECT,
    ASSOC_OBJECT,
    SCOPE_CHAIN_OBJECT,
    OBJECT_TYPE_COUNT_PLUS_1
};

struct Object {
  ObjectType  type;
  bool        marked;
  string*     str;
  // Array       array;
  // Assoc       assoc;
  // ScopeChain  scope_chain;
};

// struct FakeMethod {
//     string  *method_name;
//     Object  *object;
// };

struct Closure {
  FunctionDefinition *function;
  Object          *environment; /* CRB_ScopeChain */
};

struct Value {
  CRB_TYPE::ValueType  type;
  union {
    bool              boolean_value;
    int               int_value;
    double            double_value;
    Object            *object;
    Closure           *closure;
    // FakeMethod        *fake_method;
  } u;
};


enum StatementType {
    EXPRESSION_STATEMENT = 1,
    GLOBAL_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    FOREACH_STATEMENT,
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    TRY_STATEMENT,
    THROW_STATEMENT,
    STATEMENT_TYPE_COUNT_PLUS_1
};

enum ExpressionType {
    BOOLEAN_EXPRESSION = 1,
    INT_EXPRESSION,
    DOUBLE_EXPRESSION,
    STRING_EXPRESSION,
    REGEXP_EXPRESSION,
    IDENTIFIER_EXPRESSION,
    COMMA_EXPRESSION,
    NORMAL_ASSIGN_EXPRESSION,
    ADD_ASSIGN_EXPRESSION,
    SUB_ASSIGN_EXPRESSION,
    MUL_ASSIGN_EXPRESSION,
    DIV_ASSIGN_EXPRESSION,
    MOD_ASSIGN_EXPRESSION,
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
    LOGICAL_NOT_EXPRESSION,
    FUNCTION_CALL_EXPRESSION,
    MEMBER_EXPRESSION,
    NULL_EXPRESSION,
    ARRAY_EXPRESSION,
    INDEX_EXPRESSION,
    INCREMENT_EXPRESSION,
    DECREMENT_EXPRESSION,
    CLOSURE_EXPRESSION,
    EXPRESSION_TYPE_COUNT_PLUS_1
} ;

} // CRB_TYPE

#endif