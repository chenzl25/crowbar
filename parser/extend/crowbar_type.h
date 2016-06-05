#ifndef CROWBAR_TYPE_H
#define CROWBAR_TYPE_H
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
  NULL_VALUE,
  ARRAY_VALUE,
  STRING_VALUE,
  ASSOC_VALUE,
  CLOSURE_VALUE,
  FAKE_METHOD_VALUE,
  SCOPE_CHAIN_VALUE
};
enum ObjectType {
    ARRAY_OBJECT = 1,
    STRING_OBJECT,
    ASSOC_OBJECT,
    SCOPE_CHAIN_OBJECT,
    OBJECT_TYPE_COUNT_PLUS_1
};
enum FunctionDefinitionType {
    CROWBAR_FUNCTION_DEFINITION = 1,
    NATIVE_FUNCTION_DEFINITION,
    FUNCTION_DEFINITION_TYPE_COUNT_PLUS_1
};

class Value {
public:
  Value();
  Value(CRB_TYPE::ValueType type_);
  virtual ~Value() = default;
  virtual void print();
  CRB_TYPE::ValueType  type;
};

class IntValue: public Value {
public:
  IntValue(int int_value_);
  virtual ~IntValue();
  virtual void print();
  int int_value;
};

class DoubleValue: public Value {
public:
  DoubleValue(double double_value_);
  virtual ~DoubleValue();
  virtual void print();
  double double_value;
};

class BooleanValue: public Value {
public:
  BooleanValue(bool boolean_value_);
  virtual ~BooleanValue();
  virtual void print();
  bool boolean_value;
};

class Object : public Value {
public:
  Object(CRB_TYPE::ValueType type_);
  virtual ~Object();
  virtual void print();
  ObjectType  type;
  bool        marked;
  // Array       array;
  // Assoc       assoc;
  // ScopeChain  scope_chain;
};

class Closure : public Value {
public:
  Closure(FunctionDefinition * f, Object *env);
  virtual ~Closure();
  virtual void print();
  FunctionDefinition *function;
  Object          *environment; /* CRB_ScopeChain */
};


class String : public Object {
public:
  String(string *string_value_, bool is_literal_);
  virtual ~String();
  virtual void print();
  bool is_literal;
  string   *string_value;
};
class Array : public Value {
public:
  Array(int size);
  virtual ~Array();
  virtual void print();
  int     size;
  int     alloc_size;  // like the vector memory management
  Value   *array;      // like  new Value[10]         
};

class Assoc : public Value {
public:
  Assoc(int size);
  virtual ~Assoc();
  virtual void print();
  int member_count;
  struct AssocMember {
    string *name;    // this come from expression name not heap
    Value   value;   // not pointer, because  a = 100; o1.one = a; o2.one = a; we need to distinct o1.one and o2.one
  };
  AssocMember *member;
};
class FakeMethod : public Value {
public:
  FakeMethod();
  virtual ~FakeMethod();
  virtual void print();
  String    *method_name;
  Object    *object;
};

class ScopeChain : public Value {
public:
  ScopeChain();
  virtual ~ScopeChain();
  virtual void print();
  Object  *frame; /* CRB_Assoc */
  Object  *next;  /* ScopeChain */
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