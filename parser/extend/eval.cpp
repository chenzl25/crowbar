#include <cmath>
#include "CRB.h"
#include "eval.h"
#include "crowbar.h"
#include "crowbar_util.h"
#include "crowbar_type.h"



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