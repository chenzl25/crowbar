#include "create.h"

void crb_function_define(string *identifier, ParameterList *parameter_list,
                         Block *block) {

}
ParameterList *crb_create_parameter_list(string *identifier) {

}
ParameterList *crb_chain_parameter_list(ParameterList *list,
                                   string *identifier) {

}
ArgumentList *crb_create_argument_list(Expression *expression) {

}
ArgumentList *crb_chain_argument_list(ArgumentList *list, Expression *expr) {

}
StatementList *crb_create_statement_list(Statement *statement) {

}
StatementList *crb_chain_statement_list(StatementList *list,
                                        Statement *statement) {

}
Expression *crb_alloc_expression(CRB_TYPE::ExpressionType type) {

}
ExpressionList *crb_create_expression_list(Expression *expression) {

}
ExpressionList *crb_chain_expression_list(ExpressionList *list, Expression *expr) {

}
Expression *crb_create_assign_expression(Expression *variable,
                                             Expression *operand) {

}
Expression *crb_create_binary_expression(CRB_TYPE::ExpressionType operator_type,
                                         Expression *left,
                                         Expression *right) {

}
Expression *crb_create_minus_expression(Expression *operand) {

}
Expression *crb_create_identifier_expression(string *identifier) {

}
Expression *crb_create_int_expression(int n) {

}
Expression *crb_create_double_expression(double d) {

}
Expression *crb_create_string_expression(string *s) {

}
Expression *crb_create_function_call_expression(string *func_name,
                                                ArgumentList *argument) {

}
Expression *crb_create_boolean_expression(CRB_TYPE::Boolean value) {

}
Expression *crb_create_null_expression(void) {

}
Statement *crb_create_global_statement(IdentifierList *identifier_list) {

}
IdentifierList *crb_create_global_identifier_list(string *identifier) {

}
IdentifierList *crb_chain_identifier_list(IdentifierList *list, string *identifier) {

}
Statement *crb_create_if_statement(Expression *condition,
                                    Block *then_block, Elsif *elsif_list,
                                    Block *else_block) {

}
Elsif *crb_chain_elsif_list(Elsif *list, Elsif *add) {

}
Elsif *crb_create_elsif(Expression *expr, Block *block) {

}
Statement *crb_create_while_statement(Expression *condition, Block *block) {

}
Statement *crb_create_for_statement(Expression *init, Expression *cond,
                                    Expression *post, Block *block) {

}
Block *crb_create_block(StatementList *statement_list) {

}
Statement *crb_create_expression_statement(Expression *expression) {

}
Statement *crb_create_return_statement(Expression *expression) {

}
Statement *crb_create_break_statement(void) {

}
Statement *crb_create_continue_statement(void) {

}

Expression *crb_create_index_expression(Expression *array, Expression *index) {

}
Expression *crb_create_method_call_expression(Expression *expression,
                                  string *method_name, ArgumentList *argument) {

}
Expression *crb_create_incdec_expression(Expression *operand, CRB_TYPE::ExpressionType inc_or_dec) {

}
Expression *crb_create_array_expression(ExpressionList *list) {

}