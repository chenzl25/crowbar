#ifndef CROWBAR_TYPE_H
#define CROWBAR_TYPE_H
#include <string>
#include <vector>
#include <map>
using namespace std;

class ParameterList;
class Block;
class FunctionDefinition;

namespace CRB_TYPE  {


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



class String : public Object {
public:
  String(string *string_value_, bool is_literal_);
  virtual ~String();
  virtual void print();
  string   *string_value;
  bool is_literal;
};
class Array : public Object {
public:
  Array();
  Array(int size);
  virtual ~Array();
  virtual void print();
  vector<CRB_TYPE::Value*> vec;
  int ref_cnt;
};

class Assoc : public Object {
public:
  Assoc();
  virtual ~Assoc();
  virtual void print();
  void add_member(string name, Value* value);
  Value* search_member(string name);
  void assign_member(string name, Value* value);
  map<string, Value*> member_map;
};
class FakeMethod : public Value {
public:
  FakeMethod();
  virtual ~FakeMethod();
  virtual void print();
  String    *method_name;
  Object    *object;
};

class ScopeChain : public Object {
public:
  ScopeChain();
  ScopeChain(ScopeChain  *next_);
  ScopeChain(Assoc  *frame_, ScopeChain  *next_);
  virtual ~ScopeChain();
  virtual void print();
  Assoc  *frame; /* CRB_Assoc */
  ScopeChain  *next;  /* ScopeChain */
};

class Closure : public Value {
public:
  Closure(FunctionDefinition *function_definition_, ScopeChain *scope_chain_);
  virtual ~Closure();
  virtual void print();
  FunctionDefinition  *function_definition;
  ScopeChain          *scope_chain; /* CRB_ScopeChain */
};


enum StatementResultType{
  NORMAL_STATEMENT_RESULT = 1,
  RETURN_STATEMENT_RESULT,
  BREAK_STATEMENT_RESULT,
  CONTINUE_STATEMENT_RESULT,
  STATEMENT_RESULT_TYPE_COUNT_PLUS_1
};

class StatementResult {
public:
  StatementResult(StatementResultType type_);
  StatementResult(Value* value_);
  ~StatementResult();
  StatementResultType type;
  Value*       value;
} ;

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