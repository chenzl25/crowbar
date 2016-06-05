#include "crowbar.h"
#include "CRB.h"
#include "crowbar_type.h"
#include "../lexer/src/util.h"
#include "eval.h"

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
  for (int i = 0; i < _parameter_vec.size(); i++) {
    delete _parameter_vec[i];
  }
}
void ParameterList::add_parameter(string* identifier) {
  _parameter_vec.push_back(identifier);
}
ArgumentList::ArgumentList() {
  
}
ArgumentList::~ArgumentList() {
  for (int i = 0; i < _argument_vec.size(); i++) {
    delete _argument_vec[i];
  }
}
void ArgumentList::add_argument(Expression* expression) {
  _argument_vec.push_back(expression);
}
Expression::Expression(CRB_TYPE::ExpressionType type_) {
  type = type_;
  line = Interpreter::getInstance()->get_line();
}
void Expression::eval() {

}

IntExpression::IntExpression(int int_value_) : Expression(CRB_TYPE::INT_EXPRESSION) {
  int_value = int_value_;
}
IntExpression::~IntExpression() {

}
void IntExpression::eval() {
  auto value = new CRB_TYPE::IntValue(int_value);
  Interpreter::getInstance()->get_stack()->push(value);
  // cout << int_value << endl;
}
DoubleExpression::DoubleExpression(double double_value_) : Expression(CRB_TYPE::DOUBLE_EXPRESSION) {
  double_value = double_value_;
}
void DoubleExpression::eval() {
  auto value = new CRB_TYPE::DoubleValue(double_value);
  Interpreter::getInstance()->get_stack()->push(value);
  // cout << "eval double :"<< double_value << endl;
}
DoubleExpression::~DoubleExpression() {
  
}
BooleanExpression::BooleanExpression(bool boolean_value_) : Expression(CRB_TYPE::BOOLEAN_EXPRESSION) {
  boolean_value = boolean_value_;
}
BooleanExpression::~BooleanExpression() {
  
}
void BooleanExpression::eval() {
  auto value = new CRB_TYPE::BooleanValue(boolean_value);
  Interpreter::getInstance()->get_stack()->push(value);
  // cout << "eval double :" << boolean_value << endl;
}
NullExpression::NullExpression() : Expression(CRB_TYPE::NULL_EXPRESSION) {
}
NullExpression::~NullExpression() {
  
}
void NullExpression::eval() {
  auto value = new CRB_TYPE::Value();
  Interpreter::getInstance()->get_stack()->push(value);
  // cout << "eval null :" << "NULL" << endl;
}
StringExpression::StringExpression(string *string_value_) : Expression(CRB_TYPE::STRING_EXPRESSION){
  // string_value_ from the parser we need to delete them 
  string_value = string_value_; 
}
StringExpression::~StringExpression() {
  delete string_value;
  // cout << "success delete the slr new string" << endl;
}
void StringExpression::eval() {
  auto value = Interpreter::getInstance()->get_heap()->alloc(string_value, true);
  Interpreter::getInstance()->get_stack()->push(value);
  // cout << "eval string :"<< *string_value << endl;
}
IdentifierExpression::IdentifierExpression(string *identifier_): Expression(CRB_TYPE::IDENTIFIER_EXPRESSION) {
  // identifier_ from the parser we need to delete them 
  identifier = identifier_;
}
IdentifierExpression::~IdentifierExpression() {
  delete identifier;
  // cout << "success delete the slr new string" << endl;
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
  right = right_;
}
BinaryExpression::~BinaryExpression() {
  delete left;
  delete right;
}
void  BinaryExpression::eval() {
  auto Istack = Interpreter::getInstance()->get_stack();
  Istack->push(this->constant_folding_eval());
}
CRB_TYPE::Value* BinaryExpression::constant_folding_eval() { 
  // return value used for constant folding
  CRB_TYPE::Value *left_value, *right_value;
  CRB_TYPE::Value *result_value;
  left->eval();
  right->eval();
  auto Istack = Interpreter::getInstance()->get_stack();
  left_value = Istack->peek(1);
  right_value = Istack->peek(0);
  if (is_numeric_type(left_value->type) && 
    is_numeric_type(right_value->type)) {
    eval_binary_numeric(this->type, left_value, right_value,
                        result_value, this->line);
  } else if (left_value->type == CRB_TYPE::BOOLEAN_VALUE && 
             right_value->type == CRB_TYPE::BOOLEAN_VALUE) {
    eval_binary_boolean(this->type, left_value, right_value,
                        result_value, this->line);
  } else if (left_value->type == CRB_TYPE::STRING_VALUE && 
             this->type == CRB_TYPE::ADD_EXPRESSION) {
    chain_string(left_value, right_value, result_value, this->line);
  } else if (left_value->type == CRB_TYPE::STRING_VALUE
             && right_value->type == CRB_TYPE::STRING_VALUE) {
      // result.type = CRB_TYPE::BOOLEAN_VALUE;
      // result.u.boolean_valueue
      //     = eval_compare_string(inter, env, operator, left_value, right_value,
      //                           left->line_number);
  } else if (left_value->type == CRB_TYPE::NULL_VALUE
             || right_value->type == CRB_TYPE::NULL_VALUE) {
      // result.type = CRB_TYPE::BOOLEAN_VALUE;
      // result.u.boolean_valueue
      //     = eval_binary_null(inter, env, operator, left_value, right_value,
      //                        left->line_number);
  } else if (is_object_value(left_value->type) &&
             is_object_value(right_value->type)) {
      // result.type = CRB_BOOLEAN_VALUE;
      // result.u.boolean_valueue
      //     = (left_value->u.object == right_value->u.object);
  } else {
      // char *op_str = crb_get_operator_string(operator);
      // crb_runtime_error(inter, env, left->line_number, BAD_OPERAND_TYPE_ERR,
      //                   CRB_STRING_MESSAGE_ARGUMENT, "operator", op_str,
      //                   CRB_MESSAGE_ARGUMENT_END);
  }
  // eval_binary_numeric
  // eval_binary_boolean
  // chain_string
  // eval_compare_string
  // eval_binary_null
  // ....
  // error
  delete Istack->pop(); // delete when pop
  delete Istack->pop(); // delete then pop
  // result_value->print();
  return result_value;
}
IncrementExpression::IncrementExpression(Expression *operand_): Expression(CRB_TYPE::INCREMENT_EXPRESSION) {
  operand = operand_;
}
IncrementExpression::~IncrementExpression() {
  delete operand;
}
void IncrementExpression::eval() {
  //TODO
}
DecrementExpression::DecrementExpression(Expression *operand_): Expression(CRB_TYPE::DECREMENT_EXPRESSION) {
  operand = operand_;
}
DecrementExpression::~DecrementExpression() {
  delete operand;
}
void DecrementExpression::eval() {
  //TODO
}
IndexExpression::IndexExpression(Expression *array_, Expression *index_): Expression(CRB_TYPE::INDEX_EXPRESSION) {
  array = array_;
  index = index_;
}
IndexExpression::~IndexExpression() {
  delete array;
  delete index;
}
void IndexExpression::eval() {
  //TODO
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

FunctionCallExpression::FunctionCallExpression(Expression *function_, 
                                               ArgumentList *argument_list_) : Expression(CRB_TYPE::FUNCTION_CALL_EXPRESSION) {
  function = function_;
  argument_list = argument_list_;
}
FunctionCallExpression::~FunctionCallExpression() {
  delete function;
  delete argument_list;
}
void FunctionCallExpression::eval() {

}

MemberExpression::MemberExpression(Expression *expression_, 
                                   string *member_name_) : Expression(CRB_TYPE::MEMBER_EXPRESSION) {
  expression = expression_;
  member_name = member_name_;
}
MemberExpression::~MemberExpression() {
  delete expression;
  delete member_name;
}
void MemberExpression::eval() {

}

ArrayExpression::ArrayExpression(ExpressionList *array_literal_):Expression(CRB_TYPE::ARRAY_EXPRESSION) {
  array_literal = array_literal_;
}
ArrayExpression::~ArrayExpression() {
  delete array_literal;
}
void ArrayExpression::eval() {

}

ClousreExpression::ClousreExpression(string *identifier_, ParameterList *parameter_list_,
                                     Block *block_):Expression(CRB_TYPE::CLOSURE_EXPRESSION) {
  function_definition = new FunctionDefinition(
        identifier_, parameter_list_, block_, true);
}
ClousreExpression::~ClousreExpression() {
  delete function_definition;
}
void ClousreExpression::eval() {

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


ElsifList::ElsifList() {
  
}
ElsifList::~ElsifList() {
  for (int i = 0; i < _elsif_vec.size(); i++) {
    delete _elsif_vec[i];
  }
}
void ElsifList::add_elsif(Elsif* elsif) {
  _elsif_vec.push_back(elsif);
}
Elsif::Elsif(Expression *condition_, Block *block_) {
  condition = condition_;
  block = block_;
}
Elsif::~Elsif() {
  delete condition;
  delete block;
}
IdentifierList::IdentifierList() {
  
}
IdentifierList::~IdentifierList() {
  for (int i = 0; i < _identifier_vec.size(); i++) {
    delete _identifier_vec[i];
  }
}
void IdentifierList::add_identifier(string *identifier) {
  _identifier_vec.push_back(identifier);
}

ExpressionList::ExpressionList() {

}
ExpressionList::~ExpressionList() {
  for (int i = 0; i < _expression_vec.size(); i++) {
    delete _expression_vec[i];
  }
}
void ExpressionList::add_expression(Expression* expression_) {
  _expression_vec.push_back(expression_);
}
Statement::Statement(CRB_TYPE::StatementType type_) {
  type = type_;
}
Statement::~Statement() {
  
}
StatementList::StatementList() {
  
}
StatementList::~StatementList() {
  for (int i = 0; i < _statement_vec.size(); i++) {
    delete _statement_vec[i];
  }
}
void StatementList::add_statement(Statement *statement) {
  _statement_vec.push_back(statement);
}

IfStatement::IfStatement(Expression *condition_, Block *then_block_, ElsifList *elsif_list_,
                         Block *else_block_) : Statement(CRB_TYPE::IF_STATEMENT) {
  condition = condition_;
  then_block = then_block_;
  elsif_list = elsif_list_;
  else_block = else_block_;
}
IfStatement::~IfStatement() {
  delete condition;
  delete then_block;
  delete elsif_list;
  delete else_block;
}
WhileStatement::WhileStatement(Expression *condition_,
                               Block *block_): Statement(CRB_TYPE::WHILE_STATEMENT) {
  condition = condition_;
  block = block_;
}
WhileStatement::~WhileStatement() {
  delete condition;
  delete block;
}
ForStatement::ForStatement(Expression *init_, Expression *cond_, Expression *post_, 
                           Block *block_): Statement(CRB_TYPE::FOR_STATEMENT) {
  init = init_;
  cond = cond_;
  post = post_;
  block = block_;
}
ForStatement::~ForStatement() {
  delete init;
  delete cond;
  delete post;
  delete block;
}
ExpressionStatement::ExpressionStatement(Expression *expression_): Statement(CRB_TYPE::EXPRESSION_STATEMENT) {
  expression = expression_;
}
ExpressionStatement::~ExpressionStatement() {
  delete expression;
}
ReturnStatement::ReturnStatement(Expression *expression_): Statement(CRB_TYPE::RETURN_STATEMENT) {
  expression = expression_;
}
ReturnStatement::~ReturnStatement() {
  delete expression;
}
BreakStatement::BreakStatement(): Statement(CRB_TYPE::BREAK_STATEMENT) {

}
BreakStatement::~BreakStatement() {

}
ContinueStatement::ContinueStatement(): Statement(CRB_TYPE::CONTINUE_STATEMENT) {

}
ContinueStatement::~ContinueStatement() {

}
GlobalStatement::GlobalStatement(IdentifierList* identifier_list_) :Statement(CRB_TYPE::GLOBAL_STATEMENT) {
  identifier_list = identifier_list_;
}
GlobalStatement::~GlobalStatement() {
  delete identifier_list;
}
Block::Block(StatementList* statement_list_) {
  statement_list = statement_list_;
}
Block::~Block() {
  delete statement_list;
}
