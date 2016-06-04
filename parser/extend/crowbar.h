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
  Expression();
  Expression(CRB_TYPE::ExpressionType type, int n);
  Expression(CRB_TYPE::ExpressionType type, double d);
  Expression(CRB_TYPE::ExpressionType type, string* s);
  Expression(CRB_TYPE::ExpressionType type, bool value);
  Expression(CRB_TYPE::ExpressionType);
  ~Expression();
private:

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