#include "crowbar.h"
#include "CRB.h"
#include "crowbar_type.h"
#include "../lexer/src/util.h"
using namespace CRB;

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
Expression::Expression(CRB_TYPE::ExpressionType type_) {
  type = type_;
  line = Interpreter::getInstance()->get_line();
}
void Expression::eval() {

}
Expression::~Expression() {
  
}
IntExpression::IntExpression(int int_value_) : Expression(CRB_TYPE::INT_EXPRESSION) {
  int_value = int_value_;
}
void IntExpression::eval() {
  auto value = new CRB_TYPE::IntValue(int_value);
  Interpreter::getInstance()->get_stack()->push(value);
  cout << int_value << endl;
}
DoubleExpression::DoubleExpression(int double_value_) : Expression(CRB_TYPE::DOUBLE_EXPRESSION) {
  double_value = double_value_;
}
void DoubleExpression::eval() {
  auto value = new CRB_TYPE::DoubleValue(double_value);
  Interpreter::getInstance()->get_stack()->push(value);
  cout << double_value << endl;
}
BooleanExpression::BooleanExpression(bool boolean_value_) : Expression(CRB_TYPE::BOOLEAN_EXPRESSION) {
  boolean_value = boolean_value_;
}
void BooleanExpression::eval() {
  auto value = new CRB_TYPE::BooleanValue(boolean_value);
  Interpreter::getInstance()->get_stack()->push(value);
  cout << boolean_value << endl;
}
NullExpression::NullExpression() : Expression(CRB_TYPE::NULL_EXPRESSION) {
}
void NullExpression::eval() {
  auto value = new CRB_TYPE::Value();
  Interpreter::getInstance()->get_stack()->push(value);
  cout << "NULL" << endl;
}
StringExpression::StringExpression(string *string_value_) : Expression(CRB_TYPE::STRING_EXPRESSION){
  // string_value_ from the parser we need to delete them 
  string_value = string_value_; 
}
StringExpression::~StringExpression() {
  delete string_value;
  cout << "success delete the slr new string" << endl;
}
void StringExpression::eval() {
  auto value = Interpreter::getInstance()->get_heap()->alloc(string_value, true);
  Interpreter::getInstance()->get_stack()->push(value);
  cout << *string_value << endl;
}
IdentifierExpression::IdentifierExpression(string *identifier_): Expression(CRB_TYPE::IDENTIFIER_EXPRESSION) {
  // identifier_ from the parser we need to delete them 
  identifier = identifier_;
}
IdentifierExpression::~IdentifierExpression() {
  delete identifier;
  cout << "success delete the slr new string" << endl;
}
void IdentifierExpression::eval() {

}
MinusExpression::MinusExpression(Expression *operand_): Expression(CRB_TYPE::MINUS_EXPRESSION) {
  operand_ = operand_;
}
MinusExpression::~MinusExpression() {
  delete operand;
}
void MinusExpression::eval() {
  operand->eval();
  auto value = Interpreter::getInstance()->get_stack()->peek(0);
  if (value->type == CRB_TYPE::INT_VALUE) {
    CRB_TYPE::IntValue* value =  dynamic_cast<CRB_TYPE::IntValue*>(value);
    value->int_value = -value->int_value;
  } else if (value->type == CRB_TYPE::DOUBLE_VALUE) {
    CRB_TYPE::DoubleValue* value =  dynamic_cast<CRB_TYPE::DoubleValue*>(value);
    value->double_value = -value->double_value;
  } else {
    error(std::to_string(this->line) + " wrong type in minus expression");
  }
}
LogicalNotExpression::LogicalNotExpression(Expression *operand_): Expression(CRB_TYPE::LOGICAL_NOT_EXPRESSION) {
  operand = operand_;
}
LogicalNotExpression::~LogicalNotExpression() {
  delete operand;
}
void LogicalNotExpression::eval() {
  operand->eval();
  auto value = Interpreter::getInstance()->get_stack()->peek(0);
  if (value->type == CRB_TYPE::BOOLEAN_VALUE) {
    CRB_TYPE::BooleanValue* value =  dynamic_cast<CRB_TYPE::BooleanValue*>(value);
    value->boolean_value = !value->boolean_value;
  } else {
    error(std::to_string(this->line) + " wrong type in logical not expression");
  }
}
BinaryExpression::BinaryExpression(CRB_TYPE::ExpressionType operator_type_,
                                   Expression *left_,
                                   Expression *right_): Expression(operator_type_) {
  left = left_;
  right = right;
}
BinaryExpression::~BinaryExpression() {
  delete left;
  delete right;
}
void BinaryExpression::eval() {
  CRB_TYPE::Value *left_value, *right_value, *result_value;
  left->eval();
  right->eval();
  auto Istack = Interpreter::getInstance()->get_stack();
  left_value = Istack->peek(1);
  right_value = Istack->peek(0);
  // eval_binary_numeric
  // eval_binary_boolean
  // chain_string
  // eval_compare_string
  // eval_binary_null
  // ....
  // error
  Istack->pop();
  Istack->pop();
  Istack->push(result_value);
}
AssignExpression::AssignExpression( CRB_TYPE::ExpressionType assign_type_, 
                                    Expression *variable_,
                                    Expression *operand_): Expression(assign_type_) {
  variable = variable_;
  operand = operand_;
}
AssignExpression::~AssignExpression() {
  delete variable;
  delete operand;
}
void AssignExpression::eval() {

}
CommaExpression::CommaExpression(Expression *left_, 
                                 Expression *right_): Expression(CRB_TYPE::COMMA_EXPRESSION) {
  left = left_;
  right = right_;
}
CommaExpression::~CommaExpression() {
  delete left;
  delete right;
}
void CommaExpression::eval() {

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
