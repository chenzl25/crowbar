#ifndef CROWBAR_UTIL_H
#define CROWBAR_UTIL_H
#include <string>
#include "crowbar_type.h"
using namespace std;
namespace CRB {

void debug(string s);
void debug(string s, int a);
void assert(bool should, string msg);
void error (int line, string msg);
void error(string msg); 
string expression_type_to_string(CRB_TYPE::ExpressionType type);
string statement_type_to_string(CRB_TYPE::StatementType type);
string value_type_to_string(CRB_TYPE::ValueType type);
string value_to_string(CRB_TYPE::Value *value, int line_number);
void stack_value_delete(CRB_TYPE::Value *value);
void env_value_delete(CRB_TYPE::Value *value);
void non_object_delete(CRB_TYPE::Value *value);
} // CRB
#endif