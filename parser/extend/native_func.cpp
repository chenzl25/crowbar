#include <vector>
#include <iostream>
#include <cstdio>
#include <string>
#include "native_func.h"
#include "crowbar_util.h"
#include "CRB.h"
using namespace std;

namespace CRB {

CRB_TYPE::Value* print_proc(int arg_cnt) {
  CRB::assert(arg_cnt == 1, "native print need 1 argument");
  auto Istack = CRB::Interpreter::getInstance()->get_stack();
  string s = CRB::value_to_string(Istack->peek(0), -1/*line_number*/);
  char *buf = new char[s.length()];
  int j = 0;
  for (int i = 0; i < s.length(); i++, j++) {
    if (s[i] == '\\') {
      if (i+1 >= s.length()) break;
      switch(s[i+1]) {
        case 'n' : 
          buf[j] = '\n';
          break;
        case 't' :  
          buf[j] = '\t';

        case 'b' :
          buf[j] = '\b';
          break;
        case '\\':
          buf[j] = '\\';
          break;
        case '\"':
          buf[j] = '\"';
          break;
        case '\'':
          buf[j] = '\'';
          break;
        default:
          CRB::error("can't not escape char :" + s[i]);
      }
      i++;
    } else {
      buf[j] = s[i];
    }
  }
  buf[j] = '\0';
  printf("%s", buf);
  delete []buf;
  return new CRB_TYPE::Value();
}
FunctionDefinition* create_print_native_function() {
  FunctionDefinition* fd = new FunctionDefinition();
  fd->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION;
  fd->name = new string("print");
  fd->is_closure = false;
  fd->block = NULL;
  fd->parameter_list = NULL;
  fd->proc = print_proc;
  return fd;
}



void add_native_function() {
  auto Ienv = CRB::Interpreter::getInstance()->get_environment();
  Ienv->add_function(create_print_native_function());
}



} // CRB

