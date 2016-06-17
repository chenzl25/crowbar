#include <string>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <cmath>
#include <random>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>
#include "crowbar_util.h"
#include "CRB.h"
using namespace std;
namespace CRB {
// #define DEBUG

void debug(string s) {
#ifdef DEBUG
  cout << s << endl;
#endif
}
void debug(string s, int a) {
#ifdef DEBUG
  cout << s << a << endl;
#endif
}
void assert(bool should, string msg) {
  if (!should) {
    cout << msg << endl;
    exit(-1);
  }
}
void error (int line, string msg) {
  cout << line << " : " << msg << endl;
  exit(-1);
}
void error(string msg) {
  string file_name = Interpreter::getInstance()->get_current_run_code_filename();
  cout << file_name << ":" << endl;
  cout << msg << endl;
  exit(-1);
}
string double_to_member_string(double d) {
  string std_string = std::to_string(d);
  int dot_pos = std_string.find_first_of('.');
  if (dot_pos != -1) {
    int last_of_not_zero = std_string.find_last_not_of('0');
    return std_string.substr(0, last_of_not_zero+1);
  }
  return std_string;
}
string calculate_path(string old_path, string related_path) {
  while (related_path.length() >= 2 && related_path[0] == '.' && related_path[1] == '/') {
    related_path.erase(0,2);
  }
  assert(related_path.length() > 0, "related_path should have file name");
  while(related_path.length() >= 3 && related_path[0] == '.' && 
        related_path[1] == '.'    && related_path[2] == '/') {
    int last_slash_pos = old_path.find_last_of('/');
    old_path = old_path.substr(0, last_slash_pos);
    related_path.erase(0,3);
  }
  return old_path + "/" + related_path;
}
bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}
double gaussian_rand() {
  return sqrt(-2*log(uniform_rand())*cos(2*3.141592653589793*uniform_rand()));
}

double uniform_rand() {
  std::random_device rd;     
  std::mt19937 rng(rd());    
  std::uniform_real_distribution<double> uni(0,1); 
  return uni(rng);
}
string expression_type_to_string(CRB_TYPE::ExpressionType type) {
  switch(type) {
      case CRB_TYPE::BOOLEAN_EXPRESSION:
        return "BOOLEAN_EXPRESSION";
      case CRB_TYPE::INT_EXPRESSION:
        return "INT_EXPRESSION";
      case CRB_TYPE::DOUBLE_EXPRESSION:
        return "DOUBLE_EXPRESSION";
      case CRB_TYPE::STRING_EXPRESSION:
        return "STRING_EXPRESSION";
      case CRB_TYPE::REGEXP_EXPRESSION:
        return "REGEXP_EXPRESSION";
      case CRB_TYPE::IDENTIFIER_EXPRESSION:
        return "IDENTIFIER_EXPRESSION";
      case CRB_TYPE::COMMA_EXPRESSION:
        return "COMMA_EXPRESSION";
      case CRB_TYPE::NORMAL_ASSIGN_EXPRESSION:
        return "NORMAL_ASSIGN_EXPRESSION";
      case CRB_TYPE::ADD_ASSIGN_EXPRESSION:
        return "ADD_ASSIGN_EXPRESSION";
      case CRB_TYPE::SUB_ASSIGN_EXPRESSION:
        return "SUB_ASSIGN_EXPRESSION";
      case CRB_TYPE::MUL_ASSIGN_EXPRESSION:
        return "MUL_ASSIGN_EXPRESSION";
      case CRB_TYPE::DIV_ASSIGN_EXPRESSION:
        return "DIV_ASSIGN_EXPRESSION";
      case CRB_TYPE::MOD_ASSIGN_EXPRESSION:
        return "MOD_ASSIGN_EXPRESSION";
      case CRB_TYPE::ADD_EXPRESSION:
        return "ADD_EXPRESSION";
      case CRB_TYPE::SUB_EXPRESSION:
        return "SUB_EXPRESSION";
      case CRB_TYPE::MUL_EXPRESSION:
        return "MUL_EXPRESSION";
      case CRB_TYPE::DIV_EXPRESSION:
        return "DIV_EXPRESSION";
      case CRB_TYPE::MOD_EXPRESSION:
        return "MOD_EXPRESSION";
      case CRB_TYPE::EQ_EXPRESSION:
        return "EQ_EXPRESSION";
      case CRB_TYPE::NE_EXPRESSION:
        return "NE_EXPRESSION";
      case CRB_TYPE::GT_EXPRESSION:
        return "GT_EXPRESSION";
      case CRB_TYPE::GE_EXPRESSION:
        return "GE_EXPRESSION";
      case CRB_TYPE::LT_EXPRESSION:
        return "LT_EXPRESSION";
      case CRB_TYPE::LE_EXPRESSION:
        return "LE_EXPRESSION";
      case CRB_TYPE::LOGICAL_AND_EXPRESSION:
        return "LOGICAL_AND_EXPRESSION";
      case CRB_TYPE::LOGICAL_OR_EXPRESSION:
        return "LOGICAL_OR_EXPRESSION";
      case CRB_TYPE::MINUS_EXPRESSION:
        return "MINUS_EXPRESSION";
      case CRB_TYPE::LOGICAL_NOT_EXPRESSION:
        return "LOGICAL_NOT_EXPRESSION";
      case CRB_TYPE::FUNCTION_CALL_EXPRESSION:
        return "FUNCTION_CALL_EXPRESSION";
      case CRB_TYPE::MEMBER_EXPRESSION:
        return "MEMBER_EXPRESSION";
      case CRB_TYPE::NULL_EXPRESSION:
        return "NULL_EXPRESSION";
      case CRB_TYPE::ARRAY_EXPRESSION:
        return "ARRAY_EXPRESSION";
      case CRB_TYPE::INDEX_EXPRESSION:
        return "INDEX_EXPRESSION";
      case CRB_TYPE::INCREMENT_EXPRESSION:
        return "INCREMENT_EXPRESSION";
      case CRB_TYPE::DECREMENT_EXPRESSION:
        return "DECREMENT_EXPRESSION";
      case CRB_TYPE::CLOSURE_EXPRESSION:
        return "CLOSURE_EXPRESSION";
      default:
        return "undefined type";
  }
}
string statement_type_to_string(CRB_TYPE::StatementType type) {
  switch(type) {
    case CRB_TYPE::EXPRESSION_STATEMENT:
      return "EXPRESSION_STATEMENT";
    case CRB_TYPE::GLOBAL_STATEMENT:
      return "GLOBAL_STATEMENT";
    case CRB_TYPE::IF_STATEMENT:
      return "IF_STATEMENT";
    case CRB_TYPE::WHILE_STATEMENT:
      return "WHILE_STATEMENT";
    case CRB_TYPE::FOR_STATEMENT:
      return "FOR_STATEMENT";
    case CRB_TYPE::FOREACH_STATEMENT:
      return "FOREACH_STATEMENT";
    case CRB_TYPE::RETURN_STATEMENT:
      return "RETURN_STATEMENT";
    case CRB_TYPE::BREAK_STATEMENT:
      return "BREAK_STATEMENT";
    case CRB_TYPE::CONTINUE_STATEMENT:
      return "CONTINUE_STATEMENT";
    case CRB_TYPE::TRY_STATEMENT:
      return "TRY_STATEMENT";
    case CRB_TYPE::THROW_STATEMENT:
      return "THROW_STATEMENT";
    default:
      return "undefined type";
  }
}
string value_type_to_string(CRB_TYPE::ValueType type) {
  switch(type) {
    case CRB_TYPE::BOOLEAN_VALUE:
      return "BOOLEAN_VALUE";
    case CRB_TYPE::INT_VALUE:
      return "INT_VALUE";
    case CRB_TYPE::DOUBLE_VALUE:
      return "DOUBLE_VALUE";
    case CRB_TYPE::NULL_VALUE:
      return "NULL_VALUE";
    case CRB_TYPE::ARRAY_VALUE:
      return "ARRAY_VALUE";
    case CRB_TYPE::STRING_VALUE:
      return "STRING_VALUE";
    case CRB_TYPE::ASSOC_VALUE:
      return "ASSOC_VALUE";
    case CRB_TYPE::CLOSURE_VALUE:
      return "CLOSURE_VALUE";
    case CRB_TYPE::FAKE_METHOD_VALUE:
      return "FAKE_METHOD_VALUE";
    case CRB_TYPE::SCOPE_CHAIN_VALUE:
      return "SCOPE_CHAIN_VALUE";
    default:
      return "undefined type";
  }
}
string value_to_string(CRB_TYPE::Value *value, int line_number) {
  string result;
  switch (value->type) {
    case CRB_TYPE::BOOLEAN_VALUE: {
      if (dynamic_cast<CRB_TYPE::BooleanValue*>(value)->boolean_value == true) {
        result = string("true");
      } else {
        result = string("false");
      }
      break;
    }
    case CRB_TYPE::INT_VALUE: {
      result = std::to_string(dynamic_cast<CRB_TYPE::IntValue*>(value)->int_value);
      break;
    }
    case CRB_TYPE::DOUBLE_VALUE: {
      result = std::to_string(dynamic_cast<CRB_TYPE::DoubleValue*>(value)->double_value);
      break;
    }
    case CRB_TYPE::STRING_VALUE: {
      result =  *(dynamic_cast<CRB_TYPE::String*>(value)->string_value);
      break;
    }
    case CRB_TYPE::NULL_VALUE: {
      result = string("null");
      break;
    }
  case CRB_TYPE::ARRAY_VALUE: {
    auto array_value = dynamic_cast<CRB_TYPE::Array*>(value);
    result += "[";
    for (int i = 0; i < array_value->vec.size(); i++) {
      if (array_value->vec[i]->type == CRB_TYPE::STRING_VALUE) {
        result += "\"" + CRB::value_to_string(array_value->vec[i], line_number) + "\"";
      } else {
        result += CRB::value_to_string(array_value->vec[i], line_number);        
      }
      if (i != array_value->vec.size() -1) {
        result += ", ";
      }
    }
    result += "]";
    break;
  }
  case CRB_TYPE::CLOSURE_VALUE: {
    auto closure_value = dynamic_cast<CRB_TYPE::Closure*>(value);
    result += "closure(";
    if (closure_value->function_definition->name) {
      result += *closure_value->function_definition->name;
    } else {
      result += "null";
    }
    result += ")";
    break;
  }
  case CRB_TYPE::ASSOC_VALUE: {
    auto assoc_value = dynamic_cast<CRB_TYPE::Assoc*>(value);
    result += "{";
    int cnt = 0;
    for (auto it  = assoc_value->member_map.begin(); 
              it != assoc_value->member_map.end();
              it++) {
      result += it->first;
      result += ": ";
      if (it->second->type == CRB_TYPE::STRING_VALUE) {
        result += "\"" + CRB::value_to_string(it->second, line_number) + "\"";
      } else {
        result += CRB::value_to_string(it->second, line_number);
      }
      if (cnt != assoc_value->member_map.size() -1) {
        result += ", ";
      }
      cnt++;
    }
    result += "}";
    break;
  }
  // case CRB_TYPE::FAKE_METHOD_VALUE:
  //     CRB_mbstowcs("fake_method(", wc_buf);
  //     crb_vstr_append_string(&vstr, wc_buf);
  //     {
  //         CRB_Char *new_str;

  //         new_str = CRB_mbstowcs_alloc(inter, env, line_number,
  //                                      value->u.fake_method.method_name);
  //         DBG_assert(new_str != NULL, ("new_str is null.\n"));
  //         crb_vstr_append_string(&vstr, new_str);
  //         MEM_free(new_str);
  //     }
  //     CRB_mbstowcs(")", wc_buf);
  //     crb_vstr_append_string(&vstr, wc_buf);
  //     break;
    case CRB_TYPE::SCOPE_CHAIN_VALUE: /* FALLTHRU*/
    default:
        CRB::error(std::to_string(line_number) + " :value->type " + 
                  CRB::value_type_to_string(value->type));
  }

  return result;
}

template<typename T> void wprint(T t, const int& width) {
  cout << std::left << setw(width) << setfill(' ') << t;
}

void stack_value_delete(CRB_TYPE::Value *value) {
  non_object_delete(value);
}
void env_value_delete(CRB_TYPE::Value *value) {
  basic_value_delete(value);
}
void array_value_delete(CRB_TYPE::Value *value) {
  basic_value_delete(value);
}
void assoc_value_delete(CRB_TYPE::Value *value) {
  basic_value_delete(value);
}
void statement_result_value_delete(CRB_TYPE::Value *value) {
  basic_value_delete(value);
}
void heap_value_delete(CRB_TYPE::Value *value) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  switch(value->type) {
    case CRB_TYPE::STRING_VALUE: {
      auto string_value = dynamic_cast<CRB_TYPE::String*>(value);
      Iheap->_current_size -= sizeof(CRB_TYPE::String);
      if (!string_value->is_literal) {
        Iheap->_current_size -= string_value->string_value->length();
      }
      delete value;
      break;
    } 
    case CRB_TYPE::ARRAY_VALUE: {
      auto array_value =  dynamic_cast<CRB_TYPE::Array*>(value);
      array_value->ref_cnt--;
      if (array_value->ref_cnt == 0)  {
        Iheap->_current_size -= sizeof(CRB_TYPE::Array);
        delete value;
      }
      break;
    } 
    case CRB_TYPE::ASSOC_VALUE: {
      auto assoc_value = dynamic_cast<CRB_TYPE::Assoc*>(value);
      assoc_value->ref_cnt--;
      if (assoc_value->ref_cnt == 0) {
        Iheap->_current_size -= sizeof(CRB_TYPE::Assoc);
        delete value;
      }
      break;
    } 
    case CRB_TYPE::SCOPE_CHAIN_VALUE: {
      Iheap->_current_size -= sizeof(CRB_TYPE::ScopeChain);
      delete value;
      break;
    }
    default:
      CRB::error("unexpected type in to_delete_list");
  } // switch
}

void non_object_delete(CRB_TYPE::Value *value) {
  if (!is_object_value(value->type)) {
    delete value;
  } 
}

inline void basic_value_delete(CRB_TYPE::Value *value) {
  if (!is_object_value(value->type)) {
    delete value;
  } else {
    if (value->type == CRB_TYPE::ARRAY_VALUE) {
      auto array_value =  dynamic_cast<CRB_TYPE::Array*>(value);
      array_value->ref_cnt--;
      if (array_value->ref_cnt == 0) delete value;
    } else if(value->type == CRB_TYPE::ASSOC_VALUE) {
      auto assoc_value = dynamic_cast<CRB_TYPE::Assoc*>(value);
      assoc_value->ref_cnt--;
      if (assoc_value->ref_cnt == 0) delete value;
    } else {
      //TODO other object type
    }
  }
}

} // CRB

