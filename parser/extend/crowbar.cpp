#include "crowbar.h"

FunctionDefinition::FunctionDefinition() {

}
FunctionDefinition::FunctionDefinition(string *name_,
                                       ParameterList *parameter_list_, 
                                       Block *block_, 
                                       bool is_closure_) {
  name = name_;
  parameter_list = parameter_list_;
  block = block_;
  is_closure = is_closure_;
  type = CRB_TYPE::CROWBAR_FUNCTION_DEFINITION; // maybe change
}

FunctionDefinition::~FunctionDefinition() {

}

ParameterList::ParameterList() {
  
}
ParameterList::~ParameterList() {
  
}
ArgumentList::ArgumentList() {
  
}
ArgumentList::~ArgumentList() {
  
}
Expression::Expression() {
  
}
Expression::Expression(CRB_TYPE::ExpressionType type, int n) {

}
Expression::Expression(CRB_TYPE::ExpressionType type, double d) {

}
Expression::Expression(CRB_TYPE::ExpressionType type, string* s) {

}
Expression::Expression(CRB_TYPE::ExpressionType type, bool value) {

}
Expression::Expression(CRB_TYPE::ExpressionType) {

}
Expression::~Expression() {
  
}
Statement::Statement() {
  
}
Statement::~Statement() {
  
}
StatementList::StatementList() {
  
}
StatementList::~StatementList() {
  
}
Block::Block() {
  
}
Block::~Block() {
  
}

ElsifList::ElsifList() {
  
}
ElsifList::~ElsifList() {
  
}
Elsif::Elsif() {
  
}
Elsif::~Elsif() {
  
}
IdentifierList::IdentifierList() {
  
}
IdentifierList::~IdentifierList() {
  
}
ExpressionList::ExpressionList() {

}
ExpressionList::~ExpressionList() {

}
