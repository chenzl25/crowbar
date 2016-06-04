#ifndef CROWBAR_H
#define CROWBAR_H
#include <string>
#include "crowbar_type.h"

using namespace std;

class ParameterList {
public:  
  ParameterList();
  ~ParameterList();
private:

};       
class ArgumentList {
public:  
  ArgumentList();
  ~ArgumentList();
private:

};

class Expression {
public:  
  Expression(CRB_TYPE::ExpressionType type_);
  ~Expression();
  virtual void eval();
  int line;
  CRB_TYPE::ExpressionType type;
private:
};
class IntExpression : public Expression {
public:  
  IntExpression(int int_value_);
  ~IntExpression();
  virtual void eval();
  int int_value;
};
class DoubleExpression : public Expression {
public:  
  DoubleExpression(int double_value_);
  ~DoubleExpression();
  virtual void eval();
  double double_value;
};
class StringExpression : public Expression {
public:  
  StringExpression(string *string_value_);
  ~StringExpression();
  virtual void eval();
  string* string_value;
};
class BooleanExpression : public Expression {
public:  
  BooleanExpression(bool boolean_value_);
  ~BooleanExpression();
  virtual void eval();
  bool boolean_value;
};
class NullExpression : public Expression {
public:  
  NullExpression();
  ~NullExpression();
  virtual void eval();
};
class IdentifierExpression : public Expression {
public:  
  IdentifierExpression(string *identifier_);
  ~IdentifierExpression();
  virtual void eval();
  string *identifier;
};
class MinusExpression : public Expression {
public:  
  MinusExpression(Expression *operand_);
  ~MinusExpression();
  virtual void eval();
  Expression *operand;
};
class LogicalNotExpression : public Expression {
public:  
  LogicalNotExpression(Expression *operand_);
  ~LogicalNotExpression();
  virtual void eval();
  Expression *operand;
};
class BinaryExpression : public Expression {
public:  
  BinaryExpression(CRB_TYPE::ExpressionType operator_type_,
                   Expression *left_,
                   Expression *right_);
  ~BinaryExpression();
  virtual void eval();
  Expression *left, *right;
};
class AssignExpression : public Expression {
public:  
  AssignExpression( CRB_TYPE::ExpressionType assign_type_, 
                    Expression *variable_,
                    Expression *operand_);
  ~AssignExpression();
  virtual void eval();
  Expression *variable, *operand;
};
class CommaExpression : public Expression {
public:  
  CommaExpression(Expression *left_, Expression *right_);
  ~CommaExpression();
  virtual void eval();
  Expression *left, *right;
};
  
class Statement {
public:  
  Statement();
  ~Statement();
private:

};          
class StatementList {
public:  
  StatementList();
  ~StatementList();
private:

};      
class Block {
public:  
  Block();
  ~Block();
private:

};              
class ElsifList {
public:  
  ElsifList();
  ~ElsifList();
private:

};            
class Elsif {
public:  
  Elsif();
  ~Elsif();
private:

};
class IdentifierList {
public:  
  IdentifierList();
  ~IdentifierList();
private:

};     
class ExpressionList {
public:
  ExpressionList();
  ~ExpressionList();
private:

};
class FunctionDefinition {
public:
  FunctionDefinition();
  FunctionDefinition(string *name_, ParameterList *parameter_list_, Block *block_, bool is_closure_);
  ~FunctionDefinition();
  string * name;
  CRB_TYPE::FunctionDefinitionType  type;
  bool            is_closure;
  ParameterList   *parameter_list;
  Block           *block;
};


#endif