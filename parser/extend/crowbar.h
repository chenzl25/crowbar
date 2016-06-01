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

#endif