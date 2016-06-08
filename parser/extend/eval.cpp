#include <cmath>
#include <string>
#include "CRB.h"
#include "eval.h"
#include "crowbar.h"
#include "crowbar_util.h"
#include "crowbar_type.h"
using namespace std;


void eval_binary_int(CRB_TYPE::ExpressionType op,
                    int left, int right,
                    CRB_TYPE::Value *&result_value, int line_number) {
    if (is_math_operator(op)) {
      // do nothing
    } else if (is_compare_operator(op)) {
      // do nothint
    } else {
      CRB::assert(is_logical_operator(op), "must logical_operator");
      CRB::error("logical_operator over binary int");
    }

    switch (op) {
    case CRB_TYPE::BOOLEAN_EXPRESSION:        /* FALLTHRU */
    case CRB_TYPE::INT_EXPRESSION:            /* FALLTHRU */
    case CRB_TYPE::DOUBLE_EXPRESSION:         /* FALLTHRU */
    case CRB_TYPE::STRING_EXPRESSION:         /* FALLTHRU */
    case CRB_TYPE::REGEXP_EXPRESSION:         /* FALLTHRU */
    case CRB_TYPE::IDENTIFIER_EXPRESSION:     /* FALLTHRU */
    case CRB_TYPE::COMMA_EXPRESSION:          /* FALLTHRU */
    case CRB_TYPE::NORMAL_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::ADD_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::SUB_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::MUL_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::DIV_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::MOD_ASSIGN_EXPRESSION:  /* FALLTHRU */
        CRB::error(std::to_string(line_number) + "bad case.CRB_TYPE::" + CRB::expression_type_to_string(op));
        break;
    case CRB_TYPE::ADD_EXPRESSION:
        result_value = new CRB_TYPE::IntValue(left + right);
        break;
    case CRB_TYPE::SUB_EXPRESSION:
        result_value = new CRB_TYPE::IntValue(left - right);
        break;
    case CRB_TYPE::MUL_EXPRESSION:
        result_value = new CRB_TYPE::IntValue(left * right);
        break;
    case CRB_TYPE::DIV_EXPRESSION:
        if (right == 0) {
            CRB::error(std::to_string(line_number) + ": div 0");
        }
        result_value = new CRB_TYPE::IntValue(left / right);
        break;
    case CRB_TYPE::MOD_EXPRESSION:
        if (right == 0) {
            CRB::error(std::to_string(line_number) + ": mod 0");
        }
        result_value = new CRB_TYPE::IntValue(left % right);
        break;
    case CRB_TYPE::EQ_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left == right);
        break;
    case CRB_TYPE::NE_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left != right);
        break;
    case CRB_TYPE::GT_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left > right);
        break;
    case CRB_TYPE::GE_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left >= right);
        break;
    case CRB_TYPE::LT_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left < right);
        break;
    case CRB_TYPE::LE_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left <= right);
        break;
    case CRB_TYPE::LOGICAL_AND_EXPRESSION:        /* FALLTHRU */
    case CRB_TYPE::LOGICAL_OR_EXPRESSION: /* FALLTHRU */
    case CRB_TYPE::MINUS_EXPRESSION:      /* FALLTHRU */
    case CRB_TYPE::LOGICAL_NOT_EXPRESSION:        /* FALLTHRU */
    case CRB_TYPE::FUNCTION_CALL_EXPRESSION:      /* FALLTHRU */
    case CRB_TYPE::MEMBER_EXPRESSION:     /* FALLTHRU */
    case CRB_TYPE::NULL_EXPRESSION:       /* FALLTHRU */
    case CRB_TYPE::ARRAY_EXPRESSION:      /* FALLTHRU */
    case CRB_TYPE::CLOSURE_EXPRESSION:    /* FALLTHRU */
    case CRB_TYPE::INDEX_EXPRESSION:      /* FALLTHRU */
    case CRB_TYPE::INCREMENT_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::DECREMENT_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::EXPRESSION_TYPE_COUNT_PLUS_1:  /* FALLTHRU */
    default:
        CRB::error(std::to_string(line_number) + " :bad case.CRB_TYPE::.." + 
              CRB::expression_type_to_string(op));
    }
}

void eval_binary_double(CRB_TYPE::ExpressionType op,
                   double left, double right,
                   CRB_TYPE::Value *&result_value, int line_number) {
    if (is_math_operator(op)) {
      // do nothing
    } else if (is_compare_operator(op)) {
      // do nothint
    } else {
      CRB::assert(is_logical_operator(op), "must logical_operator");
      CRB::error("logical_operator over binary int");
    }

    switch (op) {
    case CRB_TYPE::BOOLEAN_EXPRESSION:        /* FALLTHRU */
    case CRB_TYPE::INT_EXPRESSION:            /* FALLTHRU */
    case CRB_TYPE::DOUBLE_EXPRESSION:         /* FALLTHRU */
    case CRB_TYPE::STRING_EXPRESSION:         /* FALLTHRU */
    case CRB_TYPE::REGEXP_EXPRESSION:         /* FALLTHRU */
    case CRB_TYPE::IDENTIFIER_EXPRESSION:     /* FALLTHRU */
    case CRB_TYPE::COMMA_EXPRESSION:          /* FALLTHRU */
    case CRB_TYPE::NORMAL_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::ADD_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::SUB_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::MUL_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::DIV_ASSIGN_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::MOD_ASSIGN_EXPRESSION:  /* FALLTHRU */
        CRB::error(std::to_string(line_number) + "bad case.CRB_TYPE::" + CRB::expression_type_to_string(op));
        break;
    case CRB_TYPE::ADD_EXPRESSION:
        result_value = new CRB_TYPE::DoubleValue(left + right);
        break;
    case CRB_TYPE::SUB_EXPRESSION:
        result_value = new CRB_TYPE::DoubleValue(left - right);
        break;
    case CRB_TYPE::MUL_EXPRESSION:
        result_value = new CRB_TYPE::DoubleValue(left * right);
        break;
    case CRB_TYPE::DIV_EXPRESSION:
        if (right == 0) {
            CRB::error(std::to_string(line_number) + ": div 0");
        }
        result_value = new CRB_TYPE::DoubleValue(left / right);
        break;
    case CRB_TYPE::MOD_EXPRESSION:
        if (right == 0) {
            CRB::error(std::to_string(line_number) + ": mod 0");
        }
        result_value = new CRB_TYPE::DoubleValue(fmod(left, right));
        break;
    case CRB_TYPE::EQ_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left == right);
        break;
    case CRB_TYPE::NE_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left != right);
        break;
    case CRB_TYPE::GT_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left > right);
        break;
    case CRB_TYPE::GE_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left >= right);
        break;
    case CRB_TYPE::LT_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left < right);
        break;
    case CRB_TYPE::LE_EXPRESSION:
        result_value = new CRB_TYPE::BooleanValue(left <= right);
        break;
    case CRB_TYPE::LOGICAL_AND_EXPRESSION:        /* FALLTHRU */
    case CRB_TYPE::LOGICAL_OR_EXPRESSION: /* FALLTHRU */
    case CRB_TYPE::MINUS_EXPRESSION:      /* FALLTHRU */
    case CRB_TYPE::LOGICAL_NOT_EXPRESSION:        /* FALLTHRU */
    case CRB_TYPE::FUNCTION_CALL_EXPRESSION:      /* FALLTHRU */
    case CRB_TYPE::MEMBER_EXPRESSION:     /* FALLTHRU */
    case CRB_TYPE::NULL_EXPRESSION:       /* FALLTHRU */
    case CRB_TYPE::ARRAY_EXPRESSION:      /* FALLTHRU */
    case CRB_TYPE::CLOSURE_EXPRESSION:    /* FALLTHRU */
    case CRB_TYPE::INDEX_EXPRESSION:      /* FALLTHRU */
    case CRB_TYPE::INCREMENT_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::DECREMENT_EXPRESSION:  /* FALLTHRU */
    case CRB_TYPE::EXPRESSION_TYPE_COUNT_PLUS_1:  /* FALLTHRU */
    default:
        CRB::error(std::to_string(line_number) + " :bad case.CRB_TYPE::.." + 
              CRB::expression_type_to_string(op));
    }
}


void eval_binary_numeric(CRB_TYPE::ExpressionType op,
                         CRB_TYPE::Value *left_value, CRB_TYPE::Value *right_value,
                         CRB_TYPE::Value *&result_value, int line_number) {
    if (left_value->type == CRB_TYPE::INT_VALUE
      && right_value->type == CRB_TYPE::INT_VALUE) {
      CRB_TYPE::IntValue* cast_left_value =  dynamic_cast<CRB_TYPE::IntValue*>(left_value);
      CRB_TYPE::IntValue* cast_right_value =  dynamic_cast<CRB_TYPE::IntValue*>(right_value);
      eval_binary_int(op, cast_left_value->int_value, cast_right_value->int_value,
                      result_value, line_number);
    } else if (left_value->type == CRB_TYPE::DOUBLE_VALUE
               && right_value->type == CRB_TYPE::DOUBLE_VALUE) {
      CRB_TYPE::DoubleValue* cast_left_value =  dynamic_cast<CRB_TYPE::DoubleValue*>(left_value);
      CRB_TYPE::DoubleValue* cast_right_value =  dynamic_cast<CRB_TYPE::DoubleValue*>(right_value);
      eval_binary_double(op, cast_left_value->double_value, cast_right_value->double_value,
                         result_value, line_number);
    } else if (left_value->type == CRB_TYPE::INT_VALUE
               && right_value->type == CRB_TYPE::DOUBLE_VALUE) {
        CRB_TYPE::IntValue* cast_left_value =  dynamic_cast<CRB_TYPE::IntValue*>(left_value);
        CRB_TYPE::DoubleValue* cast_right_value =  dynamic_cast<CRB_TYPE::DoubleValue*>(right_value);
        eval_binary_double(op, (double)cast_left_value->int_value,
                           cast_right_value->double_value,
                           result_value, line_number);
    } else if (left_value->type == CRB_TYPE::DOUBLE_VALUE
               && right_value->type == CRB_TYPE::INT_VALUE) {
      CRB_TYPE::DoubleValue* cast_left_value =  dynamic_cast<CRB_TYPE::DoubleValue*>(left_value);
      CRB_TYPE::IntValue* cast_right_value =  dynamic_cast<CRB_TYPE::IntValue*>(right_value);
      eval_binary_double(op, cast_left_value->double_value,
                         (double)cast_right_value->int_value,
                         result_value, line_number);
    }
}
void eval_binary_boolean(CRB_TYPE::ExpressionType op,
                         CRB_TYPE::Value *left_value, CRB_TYPE::Value *right_value, 
                         CRB_TYPE::Value *&result_value, int line_number) {
    CRB_TYPE::BooleanValue* cast_left_value  =  dynamic_cast<CRB_TYPE::BooleanValue*>(left_value);
    CRB_TYPE::BooleanValue* cast_right_value =  dynamic_cast<CRB_TYPE::BooleanValue*>(right_value);
    if (op == CRB_TYPE::EQ_EXPRESSION) {
      result_value = new CRB_TYPE::BooleanValue(cast_left_value->boolean_value == cast_right_value->boolean_value);
    } else if (op == CRB_TYPE::NE_EXPRESSION) {
      result_value = new CRB_TYPE::BooleanValue(cast_left_value->boolean_value != cast_right_value->boolean_value);
    } else {
      CRB::error(std::to_string(line_number) + " :bad case.CRB_TYPE::" + 
                 CRB::expression_type_to_string(op));
    }
}
void chain_string(CRB_TYPE::Value *left_value, CRB_TYPE::Value *right_value, 
                         CRB_TYPE::Value *&result_value, int line_number) {
    auto Iheap = CRB::Interpreter::getInstance()->get_heap();
    string right_std_string = CRB::value_to_string(right_value, line_number); // return a std string
    string concat_std_string = *(dynamic_cast<CRB_TYPE::String*>(left_value)->string_value) + right_std_string;
    result_value = Iheap->alloc(&concat_std_string, false);
}
CRB_TYPE::Value* get_lvalue(Expression* expression) {
    auto Ienv = CRB::Interpreter::getInstance()->get_environment();
    CRB_TYPE::Value* dest = NULL;
    if (expression->type == CRB_TYPE::IDENTIFIER_EXPRESSION) {
        dest = Ienv->search_variable(*(dynamic_cast<IdentifierExpression*>(expression)->identifier));
    } else if (expression->type == CRB_TYPE::INDEX_EXPRESSION) {
        //TODO
        // dest = get_array_element_lvalue(inter, env, expr);
    } else {
        CRB::error("not lvalue in assign left side");
    }
    return dest;
}
void do_assign(string variable_name, CRB_TYPE::Value* src, CRB_TYPE::Value* dest, 
               CRB_TYPE::ExpressionType type, int line_number) {
    auto Ienv = CRB::Interpreter::getInstance()->get_environment();
    CRB_TYPE::Value* result;
    if (type == CRB_TYPE::NORMAL_ASSIGN_EXPRESSION) {
        Ienv->assign_variable(variable_name, src);
    } else {
        switch (type) {
        case CRB_TYPE::NORMAL_ASSIGN_EXPRESSION:
            CRB::error(("NORMAL_ASSIGN.\n"));
        case CRB_TYPE::ADD_ASSIGN_EXPRESSION:
            type = CRB_TYPE::ADD_EXPRESSION;
            break;
        case CRB_TYPE::SUB_ASSIGN_EXPRESSION:
            type = CRB_TYPE::SUB_EXPRESSION;
            break;
        case CRB_TYPE::MUL_ASSIGN_EXPRESSION:
            type = CRB_TYPE::MUL_EXPRESSION;
            break;
        case CRB_TYPE::DIV_ASSIGN_EXPRESSION:
            type = CRB_TYPE::DIV_EXPRESSION;
            break;
        case CRB_TYPE::MOD_ASSIGN_EXPRESSION:
            type = CRB_TYPE::MOD_EXPRESSION;
            break;
        default:
            CRB::error(("bad default.\n"));
        }
        if (dest->type == CRB_TYPE::STRING_VALUE
            && type == CRB_TYPE::ADD_EXPRESSION) {
            chain_string(dest, src, result, line_number);
        } else {
            eval_binary_numeric(type, dest, src, result, line_number);
        }
        Ienv->assign_variable(variable_name, result);
    }
}
void do_function_call(FunctionCallExpression* expression, 
                      CRB_TYPE::Value* func_value) {
    if (func_value->type == CRB_TYPE::FAKE_METHOD_VALUE) {
        // call_fake_method(inter, env, caller_env, expr, &func->u.fake_method);
        return;
    }

    CRB::assert(func_value->type == CRB_TYPE::CLOSURE_VALUE, "should be closure_value in do_function_call");
    auto closure_value = dynamic_cast<CRB_TYPE::Closure*>(func_value);
    switch (closure_value->function_definition->type) {
    case CRB_TYPE::CROWBAR_FUNCTION_DEFINITION:
        call_crowbar_function(expression, closure_value);
        break;
    case CRB_TYPE::NATIVE_FUNCTION_DEFINITION:
        call_native_function(expression, closure_value);
        break;
    case CRB_TYPE::FUNCTION_DEFINITION_TYPE_COUNT_PLUS_1:
    default:
        CRB::error(std::to_string(expression->line) + " :bad case in do_function_call");
    }
}
void call_native_function(FunctionCallExpression* expression, 
                          CRB_TYPE::Closure* closure_value) {
    auto Ienv = CRB::Interpreter::getInstance()->get_environment();
    auto Istack = CRB::Interpreter::getInstance()->get_stack();
    int argument_size = expression->argument_list->_argument_vec.size();
    Ienv->use_caller_env();
    for (int i = 0; i < argument_size; i++) {
        expression->argument_list->_argument_vec[i]->eval();
    }
    Ienv->use_callee_env();
    auto result_value = closure_value->function_definition->proc(argument_size);
    for(int i = 0; i < argument_size; i++) {
        CRB::stack_value_delete(Istack->pop());
    }
    Istack->push(result_value);
}
void call_crowbar_function(FunctionCallExpression* expression, 
                           CRB_TYPE::Closure* closure_value) {
    auto Ienv = CRB::Interpreter::getInstance()->get_environment();
    auto Istack = CRB::Interpreter::getInstance()->get_stack();
    int argument_size = 0 ;
    int parameter_size = 0;
    if (expression->argument_list) {
        argument_size = expression->argument_list->_argument_vec.size();
    }
    if (closure_value->function_definition->parameter_list) {
        parameter_size = closure_value->function_definition->parameter_list->_parameter_vec.size();
    }
    CRB_TYPE::StatementResult* statement_result;
    CRB_TYPE::Value* result_value;
    if (argument_size > parameter_size) {
        CRB::error(std::to_string(expression->line) +
                   " : too much argument for function : " + 
                   *closure_value->function_definition->name +
                   " which need " + std::to_string(parameter_size));
    } else if (argument_size < parameter_size) {
        CRB::error(std::to_string(expression->line) +
                   " : too few argument for function : " + 
                   *closure_value->function_definition->name +
                   " which need " + std::to_string(parameter_size));
    }
    for (int i = 0; i < argument_size; i++) {
        Ienv->use_caller_env();
        auto arg_value = expression->argument_list->_argument_vec[i]->eval_and_pop();
        Ienv->use_callee_env();
        Ienv->add_variable(*closure_value->function_definition->parameter_list->_parameter_vec[i],
                           arg_value);
    }
    statement_result = closure_value->function_definition->block->execute();
    if (statement_result->type == CRB_TYPE::RETURN_STATEMENT_RESULT) {
        result_value = statement_result->value;
    } else {
        result_value = new CRB_TYPE::Value();
    }
    Istack->push(result_value);
    delete statement_result; // delete statement_result will not delete the return value
}