#include "create.h"
#include "crowbar.h"
#include "CRB.h"
#include "crowbar_util.h"

void crb_function_define(string *identifier, ParameterList *parameter_list,
                         Block *block) {
  auto fd = new FunctionDefinition(identifier, parameter_list, block, false);
  CRB::Interpreter::getInstance()->add_function(fd);
}
ParameterList *crb_create_parameter_list(string *identifier) {
  auto list = new ParameterList();
  list->add_parameter(identifier);
  return list;
}
ParameterList *crb_chain_parameter_list(ParameterList *list,
                                   string *identifier) {
  list->add_parameter(identifier);
  return list;
}
ArgumentList *crb_create_argument_list(Expression *expression) {
  auto list = new ArgumentList();
  list->add_argument(expression);
  return list;
}
ArgumentList *crb_chain_argument_list(ArgumentList *list, Expression *expr) {
  list->add_argument(expr);
  return list;
}
StatementList *crb_create_statement_list(Statement *statement) {
  auto list = new StatementList();
  list->add_statement(statement);
  return list;
}
StatementList *crb_chain_statement_list(StatementList *list,
                                        Statement *statement) {
  list->add_statement(statement);
  return list;
}
ExpressionList *crb_create_expression_list(Expression *expression) {
  auto list = new ExpressionList();
  list->add_expression(expression);
  return list;
}
ExpressionList *crb_chain_expression_list(ExpressionList *list, Expression *expression) {
  list->add_expression(expression);
  return list;
}
Expression *crb_create_comma_expression(Expression *left, Expression *right) {
  return new CommaExpression(left, right);
}
Expression *crb_create_assign_expression(CRB_TYPE::ExpressionType assign_type, Expression *variable,
                                             Expression *operand) {
  return new AssignExpression(assign_type, variable, operand);
}

Expression *crb_create_binary_expression(CRB_TYPE::ExpressionType operator_type,
                                         Expression *left,
                                         Expression *right) {
  // constant folding
  Expression* result;
  if ((left->type == CRB_TYPE::INT_EXPRESSION  || 
       left->type == CRB_TYPE::DOUBLE_EXPRESSION) && 
      (right->type == CRB_TYPE::INT_EXPRESSION || 
       right->type == CRB_TYPE::DOUBLE_EXPRESSION)) {

    CRB_TYPE::Value* folding_value = BinaryExpression(operator_type, left, right).constant_folding_eval();
    if (folding_value->type == CRB_TYPE::INT_VALUE) {
      result = new IntExpression(dynamic_cast<CRB_TYPE::IntValue*>(folding_value)->int_value);
    } else if (folding_value->type == CRB_TYPE::DOUBLE_VALUE) {
      result = new DoubleExpression(dynamic_cast<CRB_TYPE::DoubleValue*>(folding_value)->double_value);
    }
    // folding_value->print();
    delete folding_value;
    CRB::assert(CRB::Interpreter::getInstance()->get_stack()->size() == 0, 
                "after constant folding eval stack should be empty");
  } else {
    result = new BinaryExpression(operator_type, left, right);
  }
  return result;
}
Expression *crb_create_minus_expression(Expression *operand) {
  // constant folding
  Expression* result;
  if (operand->type == CRB_TYPE::INT_EXPRESSION) {
    result = new IntExpression(-dynamic_cast<IntExpression*>(operand)->int_value);
    delete operand;
  } else if (operand->type == CRB_TYPE::DOUBLE_EXPRESSION) {
    result = new DoubleExpression(-dynamic_cast<DoubleExpression*>(operand)->double_value);
    delete operand;
  } else {
    result = new MinusExpression(operand);
  }
  return result;
}
Expression *crb_create_logical_not_expression(Expression *operand) {
  return new LogicalNotExpression(operand);
}
Expression *crb_create_identifier_expression(string *identifier) {
  return new IdentifierExpression(identifier);
}
Expression *crb_create_int_expression(int int_value) {
  return new IntExpression(int_value);
}
Expression *crb_create_double_expression(double double_value) {
  return new DoubleExpression(double_value);
}
Expression *crb_create_string_expression(string *string_value) {
  return new StringExpression(string_value);
}
Expression *crb_create_boolean_expression(bool boolean_value) {
 return new BooleanExpression(boolean_value); 
}
Expression *crb_create_null_expression(void) {
  return new NullExpression();
}
Expression *crb_create_function_call_expression(Expression *function,
                                                ArgumentList *argument_list) {
  return new FunctionCallExpression(function, argument_list);
}
Expression *crb_create_member_expression(Expression *expression,
                                         string *member_name) {
  return new MemberExpression(expression, member_name);
}
Expression *crb_create_array_expression(ExpressionList *list) {
  return new ArrayExpression(list);
}
Expression *crb_create_closure_definition(string *identifier,
                                          ParameterList *parameter_list,
                                          Block *block) {
  return new ClousreExpression(identifier, parameter_list, block);
}

Statement *crb_create_global_statement(IdentifierList *identifier_list) {
  return new GlobalStatement(identifier_list);
}

IdentifierList *crb_create_global_identifier_list(string *identifier) {
  auto list = new IdentifierList();
  list->add_identifier(identifier);
  return list;
}
IdentifierList *crb_chain_identifier_list(IdentifierList *list, string *identifier) {
  list->add_identifier(identifier);
  return list;
}
Statement *crb_create_if_statement(Expression *condition,
                                    Block *then_block, ElsifList *elsif_list,
                                    Block *else_block) {
  return new IfStatement(condition, then_block, elsif_list, else_block);
}
ElsifList *crb_create_elsif_list(Elsif *elsif) {
  auto list = new ElsifList;
  list->add_elsif(elsif);
  return list;
}
ElsifList *crb_chain_elsif_list(ElsifList *list, Elsif *elsif) {
  list->add_elsif(elsif);
  return list;
}
Elsif *crb_create_elsif(Expression *expr, Block *block) {
  return new Elsif(expr, block);
}
Statement *crb_create_while_statement(Expression *condition, Block *block) {
  return new WhileStatement(condition, block);
}
Statement *crb_create_for_statement(Expression *init, Expression *cond,
                                    Expression *post, Block *block) {
  return new ForStatement(init, cond, post, block);
}
Block *crb_create_block(StatementList *statement_list) {
  return new Block(statement_list);
}
Statement *crb_create_expression_statement(Expression *expression) {
  return new ExpressionStatement(expression);
}
Statement *crb_create_return_statement(Expression *expression) {
  return new ReturnStatement(expression);
}
Statement *crb_create_break_statement(void) {
  return new BreakStatement();
}
Statement *crb_create_continue_statement(void) {
  return new ContinueStatement();
}

Expression *crb_create_index_expression(Expression *array, Expression *index) {
  return new IndexExpression(array, index);
}
Expression *crb_create_incdec_expression(Expression *operand, CRB_TYPE::ExpressionType inc_or_dec) {
  if (inc_or_dec == CRB_TYPE::INCREMENT_EXPRESSION) {
    return new IncrementExpression(operand);
  } else if (inc_or_dec == CRB_TYPE::DECREMENT_EXPRESSION) {
    return new DecrementExpression(operand);
  } else {
    // impossible way
  }

}
