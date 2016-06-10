#ifndef EVAL_H
#define EVAL_H
#include "crowbar_type.h"
#include "crowbar.h"
void eval_binary_int(CRB_TYPE::ExpressionType op,
                    int left, int right,
                    CRB_TYPE::Value *&result, int line_number);
void eval_binary_double(CRB_TYPE::ExpressionType op,
                   double left, double right,
                   CRB_TYPE::Value *&result, int line_number);
void eval_binary_numeric(CRB_TYPE::ExpressionType op,
                        CRB_TYPE::Value *left_val, CRB_TYPE::Value *right_val,
                        CRB_TYPE::Value *&result, int line_number);
void eval_binary_boolean(CRB_TYPE::ExpressionType op,
                         CRB_TYPE::Value *left_value, CRB_TYPE::Value *right_value, 
                         CRB_TYPE::Value *&result_value, int line_number);
void chain_string(CRB_TYPE::Value *left_value, CRB_TYPE::Value *right_value, 
                         CRB_TYPE::Value *&result_value, int line_number);
void eval_compare_string(CRB_TYPE::ExpressionType op,
                         CRB_TYPE::Value *left_value, CRB_TYPE::Value *right_value, 
                         CRB_TYPE::Value *&result_value, int line_number);
void eval_binary_null( CRB_TYPE::ExpressionType op,
                       CRB_TYPE::Value *left_value, CRB_TYPE::Value *right_value, 
                       CRB_TYPE::Value *&result_value, int line_number);
void assign_array_element(IndexExpression* index_expression, CRB_TYPE::Value* assign_value);

CRB_TYPE::Value* get_array_element_lvalue(IndexExpression* index_expression);
void do_assign(string variable_name, CRB_TYPE::Value* src, CRB_TYPE::Value* dest, 
               CRB_TYPE::ExpressionType type, int line_number);
void do_function_call(FunctionCallExpression* expression, 
                      CRB_TYPE::Value* func_value, CRB::LocalEnv *caller_env);
void call_crowbar_function(FunctionCallExpression* expression, 
                           CRB_TYPE::Closure* closure_value, CRB::LocalEnv *caller_env);
void call_native_function(FunctionCallExpression* expression, 
                           CRB_TYPE::Closure* closure_value, CRB::LocalEnv *caller_env);

#endif