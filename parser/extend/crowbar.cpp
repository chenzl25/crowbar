#include "crowbar.h"
#include "CRB.h"
#include "crowbar_type.h"
#include "eval.h"
#include "execute.h"
#include "crowbar_util.h"
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
  type = CRB_TYPE::CROWBAR_FUNCTION_DEFINITION; // default tpye
  proc = NULL;
}

FunctionDefinition::~FunctionDefinition() {
  delete parameter_list;
  delete block;
}

ParameterList::ParameterList() {
  
}
ParameterList::~ParameterList() {
  for (int i = 0; i < _parameter_vec.size(); i++) {
    if(_parameter_vec[i] != NULL) delete _parameter_vec[i];
    _parameter_vec[i] = NULL;
  }
}
void ParameterList::add_parameter(string* identifier) {
  _parameter_vec.push_back(identifier);
}
ArgumentList::ArgumentList() {
  
}
ArgumentList::~ArgumentList() {
  for (int i = 0; i < _argument_vec.size(); i++) {
    if(_argument_vec[i] != NULL) delete _argument_vec[i];
    _argument_vec[i] = NULL;
  }
}
void ArgumentList::add_argument(Expression* expression) {
  _argument_vec.push_back(expression);
}
Expression::Expression(CRB_TYPE::ExpressionType type_) {
  type = type_;
  line = Interpreter::getInstance()->get_line();
}
CRB_TYPE::Value* Expression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void Expression::eval() {

}

IntExpression::IntExpression(int int_value_) : Expression(CRB_TYPE::INT_EXPRESSION) {
  int_value = int_value_;
}
IntExpression::~IntExpression() {

}
CRB_TYPE::Value* IntExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void IntExpression::eval() {
  auto value = new CRB_TYPE::IntValue(int_value);
  Interpreter::getInstance()->get_stack()->push(value);
  // cout << int_value << endl;
}
DoubleExpression::DoubleExpression(double double_value_) : Expression(CRB_TYPE::DOUBLE_EXPRESSION) {
  double_value = double_value_;
}
CRB_TYPE::Value* DoubleExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
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
CRB_TYPE::Value* BooleanExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
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
CRB_TYPE::Value* NullExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
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
  if(string_value != NULL) delete string_value;
  string_value = NULL;
  // cout << "success delete the slr new string" << endl;
}
CRB_TYPE::Value* StringExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
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
  if(identifier != NULL) delete identifier;
  identifier = NULL;
  // cout << "success delete the slr new string" << endl;
}
CRB_TYPE::Value* IdentifierExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void IdentifierExpression::eval() {
  auto Ienv = Interpreter::getInstance()->get_environment();
  auto Istack = Interpreter::getInstance()->get_stack();
  auto search_result = Ienv->search_variable(*(this->identifier));
  if (search_result == NULL) {
    auto fd = Ienv->search_function(*(this->identifier));
    assert(fd != NULL, std::to_string(this->line)+ " :undefined identifier :" + *(this->identifier));
    auto closure_value = new CRB_TYPE::Closure(fd, NULL);
    Istack->push(closure_value);
  } else {
    // here we copy the environment value. array and object will just the same without copying
    Istack->push(value_copy(search_result)); // copy maintains stack delete invariant
  }
}
MinusExpression::MinusExpression(Expression *operand_): Expression(CRB_TYPE::MINUS_EXPRESSION) {
  operand_ = operand_;
}
MinusExpression::~MinusExpression() {
  if(operand != NULL) delete operand;
  operand = NULL;
}
CRB_TYPE::Value* MinusExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
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
    CRB::error(std::to_string(this->line) + " wrong type in minus expression");
  }
}
LogicalNotExpression::LogicalNotExpression(Expression *operand_): Expression(CRB_TYPE::LOGICAL_NOT_EXPRESSION) {
  operand = operand_;
}
LogicalNotExpression::~LogicalNotExpression() {
  if(operand != NULL) delete operand;
  operand = NULL;
}
CRB_TYPE::Value* LogicalNotExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void LogicalNotExpression::eval() {
  operand->eval();
  auto value = Interpreter::getInstance()->get_stack()->peek(0);
  if (value->type == CRB_TYPE::BOOLEAN_VALUE) {
    CRB_TYPE::BooleanValue* value =  dynamic_cast<CRB_TYPE::BooleanValue*>(value);
    value->boolean_value = !value->boolean_value;
  } else {
    CRB::error(std::to_string(this->line) + " wrong type in logical not expression");
  }
}
BinaryExpression::BinaryExpression(CRB_TYPE::ExpressionType operator_type_,
                                   Expression *left_,
                                   Expression *right_): Expression(operator_type_) {
  left = left_;
  right = right_;
}
BinaryExpression::~BinaryExpression() {
  if(left != NULL) delete left;
  left = NULL;
  if(right != NULL) delete right;
  right = NULL;
}
CRB_TYPE::Value*  BinaryExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void  BinaryExpression::eval() {
  auto Istack = Interpreter::getInstance()->get_stack();
  Istack->push(this->constant_folding_eval());
}
CRB_TYPE::Value* BinaryExpression::constant_folding_eval() { 
  // return value used for constant folding
  CRB_TYPE::Value *left_value, *right_value;
  CRB_TYPE::Value *result_value;
  // logic short cut
  if (this->type == CRB_TYPE::LOGICAL_OR_EXPRESSION) {
    left_value = left->eval_and_pop();
    assert(left_value->type == CRB_TYPE::BOOLEAN_VALUE, "left side of || should be boolean");
    CRB_TYPE::BooleanValue* cast_left_value  =  dynamic_cast<CRB_TYPE::BooleanValue*>(left_value);
    if (cast_left_value->boolean_value) {
      return new CRB_TYPE::BooleanValue(true);
    }
    right_value = right->eval_and_pop();
    assert(left_value->type == CRB_TYPE::BOOLEAN_VALUE, "right side of || should be boolean");
    CRB_TYPE::BooleanValue* cast_right_value =  dynamic_cast<CRB_TYPE::BooleanValue*>(right_value);
    return new CRB_TYPE::BooleanValue(cast_right_value->boolean_value);
  }
  if (this->type == CRB_TYPE::LOGICAL_AND_EXPRESSION) {
    left_value = left->eval_and_pop();
    assert(left_value->type == CRB_TYPE::BOOLEAN_VALUE, "left side of && should be boolean");
    CRB_TYPE::BooleanValue* cast_left_value  =  dynamic_cast<CRB_TYPE::BooleanValue*>(left_value);
    if (cast_left_value->boolean_value == false) {
      return new CRB_TYPE::BooleanValue(false);
    }
    right_value = right->eval_and_pop();
    assert(left_value->type == CRB_TYPE::BOOLEAN_VALUE, "right side of && should be boolean");
    CRB_TYPE::BooleanValue* cast_right_value =  dynamic_cast<CRB_TYPE::BooleanValue*>(right_value);
    return new CRB_TYPE::BooleanValue(cast_right_value->boolean_value);
  }
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
    eval_compare_string(this->type, left_value, right_value,
                        result_value, this->line);
  } else if (left_value->type == CRB_TYPE::NULL_VALUE
             || right_value->type == CRB_TYPE::NULL_VALUE) {
    eval_binary_null(this->type, left_value, right_value,
                     result_value, this->line);
  } else if (is_object_value(left_value->type) &&
             is_object_value(right_value->type)) {
      bool b = (left_value == right_value); // compare the address
      result_value = new CRB_TYPE::BooleanValue(b);
  } else {
      CRB::error(std::to_string(this->line) + " :invalid binary operation");
  }
  CRB::stack_value_delete(Istack->pop()); // delete when pop
  CRB::stack_value_delete(Istack->pop()); // delete then pop
  // result_value->print();
  return result_value;
}
IncrementExpression::IncrementExpression(Expression *operand_): Expression(CRB_TYPE::INCREMENT_EXPRESSION) {
  operand = operand_;
}
IncrementExpression::~IncrementExpression() {
  if(operand != NULL) delete operand;
  operand = NULL;
}
CRB_TYPE::Value* IncrementExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void IncrementExpression::eval() {
  auto Istack = Interpreter::getInstance()->get_stack();
  auto Ienv = Interpreter::getInstance()->get_environment();
  string line_string = std::to_string(this->line);
  assert(this->operand != NULL, line_string + " : IncrementExpression: operand_expression should exist");
  
  CRB_TYPE::Value*  operand_value;  
  if (operand->type == CRB_TYPE::IDENTIFIER_EXPRESSION) {
    operand_value = Ienv->search_variable(*(dynamic_cast<IdentifierExpression*>(operand)->identifier));
  } else {
    operand_value = operand->eval_and_pop();// we can pop, because it should be the int, GC will not collect it.
  }
  assert(operand_value != NULL, line_string + " : IncrementExpression: operand_value should exist");
  assert(operand_value->type == CRB_TYPE::INT_VALUE, line_string + " : operator ++ should only work on interger");
  auto cast_operand_value = dynamic_cast<CRB_TYPE::IntValue*>(operand_value);
  int old_int = cast_operand_value->int_value;
  if (this->operand->type == CRB_TYPE::INDEX_EXPRESSION) {
    cast_operand_value->int_value++;
    assign_array_element(dynamic_cast<IndexExpression*>(this->operand), 
                         cast_operand_value); // reuse the cast_operand_value, because cast_operand_value from the stack must be copied
  } else {
    // maybe it should extend the object later, like obj.a++;
    cast_operand_value->int_value++;
  }
  Istack->push(new CRB_TYPE::IntValue(old_int));
}
DecrementExpression::DecrementExpression(Expression *operand_): Expression(CRB_TYPE::DECREMENT_EXPRESSION) {
  operand = operand_;
}
DecrementExpression::~DecrementExpression() {
  if(operand != NULL) delete operand;
  operand = NULL;
}
CRB_TYPE::Value* DecrementExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void DecrementExpression::eval() {
  auto Istack = Interpreter::getInstance()->get_stack();
  auto Ienv = Interpreter::getInstance()->get_environment();
  string line_string = std::to_string(this->line);
  assert(this->operand != NULL, line_string + " : DecrementExpression: operand_expression should exist");
  
  CRB_TYPE::Value*  operand_value;  
  if (operand->type == CRB_TYPE::IDENTIFIER_EXPRESSION) {
    operand_value = Ienv->search_variable(*(dynamic_cast<IdentifierExpression*>(operand)->identifier));
  } else {
    operand_value = operand->eval_and_pop();// we can pop, because it should be the int, GC will not collect it.
  }
  assert(operand_value != NULL, line_string + " : DecrementExpression: operand_value should exist");
  assert(operand_value->type == CRB_TYPE::INT_VALUE, line_string + " : operator ++ should only work on interger");
  auto cast_operand_value = dynamic_cast<CRB_TYPE::IntValue*>(operand_value);
  int old_int = cast_operand_value->int_value;
  if (this->operand->type == CRB_TYPE::INDEX_EXPRESSION) {
    cast_operand_value->int_value--;
    assign_array_element(dynamic_cast<IndexExpression*>(this->operand), 
                         cast_operand_value); // reuse the cast_operand_value, because cast_operand_value from the stack must be copied
  } else {
    // maybe it should extend the object later, like obj.a--;
    cast_operand_value->int_value--;
  }
  Istack->push(new CRB_TYPE::IntValue(old_int));
}
IndexExpression::IndexExpression(Expression *array_, Expression *index_): Expression(CRB_TYPE::INDEX_EXPRESSION) {
  array = array_;
  index = index_;
}
IndexExpression::~IndexExpression() {
  if(array != NULL) delete array;
  array = NULL;
  if(index != NULL) delete index;
  index = NULL;
}
CRB_TYPE::Value* IndexExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void IndexExpression::eval() {
  auto Istack = Interpreter::getInstance()->get_stack();
  string line_string = std::to_string(this->line);
  // we use eval insteat of eval pop to avoid GC collect the temporary variable
  this->array->eval();
  this->index->eval();
  auto index_value = Istack->pop();
  auto array_value = Istack->pop();
  CRB::assert(array_value->type == CRB_TYPE::ARRAY_VALUE, line_string + ": not a array");
  CRB::assert(index_value->type == CRB_TYPE::INT_VALUE, line_string + ": index not a integer");
  auto cast_index_value = dynamic_cast<CRB_TYPE::IntValue*>(index_value);
  auto cast_array_value = dynamic_cast<CRB_TYPE::Array*>(array_value);
  CRB::assert(cast_index_value->int_value >= 0, line_string + ": the index should not lest than 0");
  CRB::assert(cast_index_value->int_value < cast_array_value->vec.size(), line_string +
          ": index exceeds the array size");
  Istack->push(value_copy(cast_array_value->vec[cast_index_value->int_value]));
}
AssignExpression::AssignExpression( CRB_TYPE::ExpressionType assign_type_, 
                                    Expression *variable_,
                                    Expression *operand_): Expression(assign_type_) {
  variable = variable_;
  operand = operand_;
}
AssignExpression::~AssignExpression() {
  if(variable != NULL) delete variable;
  variable = NULL;
  if(operand != NULL) delete operand;
  operand = NULL;
}
CRB_TYPE::Value* AssignExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void AssignExpression::eval() {
  auto Ienv = CRB::Interpreter::getInstance()->get_environment();
  auto Istack = Interpreter::getInstance()->get_stack();
  operand->eval();
  auto src = Istack->peek(0); // for the result of eval 
  if (this->variable->type == CRB_TYPE::MEMBER_EXPRESSION) {
    // TODO;
    return;
  }
  if (this->variable->type == CRB_TYPE::INDEX_EXPRESSION) {
    // for the stack delete reason
    assign_array_element(dynamic_cast<IndexExpression*>(this->variable), value_copy(src));
    return;
  } 
  // find the the dest value
  CRB_TYPE::Value* dest = Ienv->search_variable(*(dynamic_cast<IdentifierExpression*>(this->variable)->identifier));

  // if no dest value, we init it
  if (this->variable->type == CRB_TYPE::IDENTIFIER_EXPRESSION && dest == NULL) {
    // init assign
    if (this->type != CRB_TYPE::NORMAL_ASSIGN_EXPRESSION) {
      CRB::error("init assign must use = ");
    }
    string variable_name = *(dynamic_cast<IdentifierExpression*>(this->variable)->identifier);
    Ienv->add_variable(variable_name, value_copy(src));
  } else {
    CRB::assert(dest != NULL, "dest muse not equal NULL");
    // The most difficult problem is we can't change what *Value point to
    if (this->variable->type == CRB_TYPE::IDENTIFIER_EXPRESSION) {
      string variable_name = *(dynamic_cast<IdentifierExpression*>(this->variable)->identifier);
      do_assign(variable_name, value_copy(src), dest, this->type, this->line);
    } else {

    }
  }
}

FunctionCallExpression::FunctionCallExpression(Expression *function_, 
                                               ArgumentList *argument_list_) : Expression(CRB_TYPE::FUNCTION_CALL_EXPRESSION) {
  function = function_;
  argument_list = argument_list_;
}
FunctionCallExpression::~FunctionCallExpression() {
  if(function != NULL) delete function;
  function = NULL;
  if(argument_list != NULL) delete argument_list;
  argument_list = NULL;
}
CRB_TYPE::Value* FunctionCallExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void FunctionCallExpression::eval() {
  auto Ienv = Interpreter::getInstance()->get_environment();
  auto Istack = Interpreter::getInstance()->get_stack();
  CRB_TYPE::Closure* closure_value = NULL;
  this->function->eval(); // push function
  auto function_value = value_copy(Istack->peek(0));
  if (function_value->type == CRB_TYPE::CLOSURE_VALUE) {
    closure_value = dynamic_cast<CRB_TYPE::Closure*>(function_value);
  } else if (function_value->type == CRB_TYPE::FAKE_METHOD_VALUE) {
    
  } else {
    CRB::error(std::to_string(this->line) + ": not function call");
  }
  auto caller_env = Ienv->get_use_env();  // before call we record the caller_env
  Ienv->alloc_env(closure_value->scope_chain);
  Ienv->use_env(Ienv->get_top_env());
  if (closure_value && closure_value->function_definition->is_closure
      && closure_value->function_definition->name) {
    // support name closure recursion
    Ienv->add_variable(*closure_value->function_definition->name, closure_value);
  }
  do_function_call(this, closure_value, caller_env); // eval function call
  Ienv->dealloc_env(); // dealloc the callee_env
  Ienv->use_env(caller_env);  // reset the use_env
  auto return_value = Istack->pop();  // pop function return value
  CRB::stack_value_delete(Istack->pop()); // delete function
  Istack->push(return_value);
}

MemberExpression::MemberExpression(Expression *expression_, 
                                   string *member_name_) : Expression(CRB_TYPE::MEMBER_EXPRESSION) {
  expression = expression_;
  member_name = member_name_;
}
MemberExpression::~MemberExpression() {
  if(expression != NULL) delete expression;
  expression = NULL;
  if(member_name != NULL) delete member_name;
  member_name = NULL;
}
CRB_TYPE::Value* MemberExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void MemberExpression::eval() {
  //TODO
}

ArrayExpression::ArrayExpression(ExpressionList *array_literal_):Expression(CRB_TYPE::ARRAY_EXPRESSION) {
  array_literal = array_literal_;
}
ArrayExpression::~ArrayExpression() {
  if(array_literal != NULL) delete array_literal;
  array_literal = NULL;
}
CRB_TYPE::Value* ArrayExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void ArrayExpression::eval() {
  auto Istack = Interpreter::getInstance()->get_stack();
  auto Iheap = Interpreter::getInstance()->get_heap();
  auto array_value = dynamic_cast<CRB_TYPE::Array*>(Iheap->alloc(CRB_TYPE::ARRAY_OBJECT));
  int array_size = array_literal->_expression_vec.size();
  array_value->vec.resize(array_size);
  Istack->push(array_value); // push the value
  for (int i = 0; i < array_size; i++) {
    array_value->vec[i] = array_literal->_expression_vec[i]->eval_and_pop(); 
  }
}

ClousreExpression::ClousreExpression(string *identifier_, ParameterList *parameter_list_,
                                     Block *block_):Expression(CRB_TYPE::CLOSURE_EXPRESSION) {
  function_definition = new FunctionDefinition(
        identifier_, parameter_list_, block_, true);
}
ClousreExpression::~ClousreExpression() {
  if(function_definition != NULL) delete function_definition;
  function_definition = NULL;
}
CRB_TYPE::Value* ClousreExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void ClousreExpression::eval() {
  auto Ienv = Interpreter::getInstance()->get_environment();
  auto Istack = Interpreter::getInstance()->get_stack();
  auto caller_env = Ienv->get_use_env();
  CRB_TYPE::ScopeChain* caller_scope_chain;
  if (caller_env) {
    caller_scope_chain = caller_env->_scope_chain;
  } else {
    caller_scope_chain = NULL;
  }
  auto closure_value = new CRB_TYPE::Closure(this->function_definition, caller_scope_chain);
  Istack->push(closure_value);
}

CommaExpression::CommaExpression(Expression *left_, 
                                 Expression *right_): Expression(CRB_TYPE::COMMA_EXPRESSION) {
  left = left_;
  right = right_;
}
CommaExpression::~CommaExpression() {
  if(left != NULL) delete left;
  left = NULL;
  if(right != NULL) delete right;
  right = NULL;
}
CRB_TYPE::Value* CommaExpression::eval_and_pop() {
  eval();
  return Interpreter::getInstance()->get_stack()->pop();
}
void CommaExpression::eval() {
  stack_value_delete(left->eval_and_pop());
  right->eval();
}


ElsifList::ElsifList() {
  
}
ElsifList::~ElsifList() {
  for (int i = 0; i < _elsif_vec.size(); i++) {
    if(_elsif_vec[i] != NULL) delete _elsif_vec[i];
    _elsif_vec[i] = NULL;
  }
}
void ElsifList::add_elsif(Elsif* elsif) {
  _elsif_vec.push_back(elsif);
}
CRB_TYPE::StatementResult* ElsifList::execute() {
  for (int i = 0 ; i < _elsif_vec.size(); i++) {
    auto result = _elsif_vec[i]->execute();
    if (result != NULL) {
      return result;
    }
  }
  return NULL;
}
Elsif::Elsif(Expression *condition_, Block *block_) {
  condition = condition_;
  block = block_;
}
Elsif::~Elsif() {
  if(condition != NULL) delete condition;
  condition = NULL;
  if(block != NULL) delete block;
  block = NULL;
}
CRB_TYPE::StatementResult* Elsif::execute() {
  auto value = condition->eval_and_pop();
  assert(value->type == CRB_TYPE::BOOLEAN_VALUE, 
         " condition expression should be bool");
  if (dynamic_cast<CRB_TYPE::BooleanValue*>(value)->boolean_value)  {
    stack_value_delete(value); // delete the stack pop
    return this->block->execute();
  } else {
    return NULL;
  }

}
IdentifierList::IdentifierList() {
  
}
IdentifierList::~IdentifierList() {
  for (int i = 0; i < _identifier_vec.size(); i++) {
    if(_identifier_vec[i] != NULL) delete _identifier_vec[i];
    _identifier_vec[i] = NULL;
  }
}
void IdentifierList::add_identifier(string *identifier) {
  _identifier_vec.push_back(identifier);
}

ExpressionList::ExpressionList() {

}
ExpressionList::~ExpressionList() {
  for (int i = 0; i < _expression_vec.size(); i++) {
    if(_expression_vec[i] != NULL) delete _expression_vec[i];
    _expression_vec[i] = NULL;
  }
}
void ExpressionList::add_expression(Expression* expression_) {
  _expression_vec.push_back(expression_);
}
Statement::Statement(CRB_TYPE::StatementType type_) {
  type = type_;
  line = Interpreter::getInstance()->get_line();
}
Statement::~Statement() {
  
}
CRB_TYPE::StatementResult* Statement::execute() {

}
StatementList::StatementList() {
  
}
StatementList::~StatementList() {
  for (int i = 0; i < _statement_vec.size(); i++) {
    if(_statement_vec[i] != NULL) delete _statement_vec[i];
    _statement_vec[i] = NULL;
  }
}
void StatementList::add_statement(Statement *statement) {
  _statement_vec.push_back(statement);
}
CRB_TYPE::StatementResult* StatementList::execute() {
  CRB_TYPE::StatementResult* result;
  for (int i = 0; i < _statement_vec.size(); i++) {
    result = _statement_vec[i]->execute();
    if (result->type != CRB_TYPE::NORMAL_STATEMENT_RESULT) {
      return result;
    } else {
      delete result;
    }
  }
  return new CRB_TYPE::StatementResult(CRB_TYPE::NORMAL_STATEMENT_RESULT);
}

IfStatement::IfStatement(Expression *condition_, Block *then_block_, ElsifList *elsif_list_,
                         Block *else_block_) : Statement(CRB_TYPE::IF_STATEMENT) {
  condition = condition_;
  then_block = then_block_;
  elsif_list = elsif_list_;
  else_block = else_block_;
}
IfStatement::~IfStatement() {
  if(condition != NULL) delete condition;
  condition = NULL;
  if(then_block != NULL) delete then_block;
  then_block = NULL;
  if(elsif_list != NULL) delete elsif_list;
  elsif_list = NULL;
  if(else_block != NULL) delete else_block;
  else_block = NULL;
}
CRB_TYPE::StatementResult* IfStatement::execute() {
  CRB_TYPE::StatementResult* result;
  auto value = condition->eval_and_pop();
  assert(value->type == CRB_TYPE::BOOLEAN_VALUE, 
         std::to_string(this->line) + " condition expression should be bool");
  if (dynamic_cast<CRB_TYPE::BooleanValue*>(value)->boolean_value) {
    stack_value_delete(value); // delete the stack pop
    result = this->then_block->statement_list->execute();
  } else if (this->elsif_list) {
    result = this->elsif_list->execute();
    if (result != NULL) {
      return result;
    } else if (this->else_block) {
      result = this->else_block->execute();
    }
  } else if (this->else_block) {
    result = this->else_block->execute();
  } else {
    result = new CRB_TYPE::StatementResult(CRB_TYPE::NORMAL_STATEMENT_RESULT);
  }
  return result;
}
WhileStatement::WhileStatement(Expression *condition_,
                               Block *block_): Statement(CRB_TYPE::WHILE_STATEMENT) {
  condition = condition_;
  block = block_;
}
WhileStatement::~WhileStatement() {
  if(condition != NULL) delete condition;
  condition = NULL;
  if(block != NULL) delete block;
  block = NULL;
}
CRB_TYPE::StatementResult* WhileStatement::execute() {
  CRB_TYPE::StatementResult* result;
  while(true) {
    auto value = condition->eval_and_pop();
    assert(value->type == CRB_TYPE::BOOLEAN_VALUE, 
           std::to_string(this->line) + " condition expression should be bool");
    if (dynamic_cast<CRB_TYPE::BooleanValue*>(value)->boolean_value) {
      stack_value_delete(value); // delete the stack pop
      result = this->block->execute();
      if (result->type == CRB_TYPE::BREAK_STATEMENT_RESULT) {
        delete result;
        break;
      } else if(result->type == CRB_TYPE::CONTINUE_STATEMENT_RESULT) {
        delete result;
        // do nothing beacuse we don't use label
      } else {
        delete result;
        // do nothing
      }
    } else {
      break;
    }
  }
  return new CRB_TYPE::StatementResult(CRB_TYPE::NORMAL_STATEMENT_RESULT);
}
ForStatement::ForStatement(Expression *init_, Expression *cond_, Expression *post_, 
                           Block *block_): Statement(CRB_TYPE::FOR_STATEMENT) {
  init = init_;
  cond = cond_;
  post = post_;
  block = block_;
}
ForStatement::~ForStatement() {
  if(init != NULL) delete init;
  init = NULL;
  if(cond != NULL) delete cond;
  cond = NULL;
  if(post != NULL) delete post;
  post = NULL;
  if(block != NULL) delete block;
  block = NULL;
}
CRB_TYPE::StatementResult* ForStatement::execute() {
  CRB_TYPE::StatementResult* result;
  if (this->init) {
    this->init->eval_and_pop();
  }
  while(true) {
    CRB_TYPE::Value* value;
    if (this->cond) {
      value = this->cond->eval_and_pop();
    } else {
      value = new CRB_TYPE::BooleanValue(true);
    }
    assert(value->type == CRB_TYPE::BOOLEAN_VALUE, 
           std::to_string(this->line) + " condition expression should be bool");
    if (dynamic_cast<CRB_TYPE::BooleanValue*>(value)->boolean_value) {
      stack_value_delete(value); // delete the stack pop
      result = this->block->execute();
      if (result->type == CRB_TYPE::RETURN_STATEMENT_RESULT) {
        return result;
      } else if (result->type == CRB_TYPE::BREAK_STATEMENT_RESULT) {
        delete result;
        break;
      } else if(result->type == CRB_TYPE::CONTINUE_STATEMENT_RESULT) {
        delete result;
        // do nothing beacuse we don't use label
      } else {
        delete result;
        // do nothing
      }
      if (this->post) {
        this->post->eval_and_pop();
      }
    } else { // conditoin false
      break;
    }
  }
  return new CRB_TYPE::StatementResult(CRB_TYPE::NORMAL_STATEMENT_RESULT);
}
ExpressionStatement::ExpressionStatement(Expression *expression_): Statement(CRB_TYPE::EXPRESSION_STATEMENT) {
  expression = expression_;
}
ExpressionStatement::~ExpressionStatement() {
  if(expression != NULL) delete expression;
  expression = NULL;
}
CRB_TYPE::StatementResult* ExpressionStatement::execute() {
  // cout << CRB::expression_type_to_string(expression->type) << endl;
  CRB::stack_value_delete(expression->eval_and_pop());
  return new CRB_TYPE::StatementResult(CRB_TYPE::NORMAL_STATEMENT_RESULT);
}
ReturnStatement::ReturnStatement(Expression *expression_): Statement(CRB_TYPE::RETURN_STATEMENT) {
  expression = expression_;
}
ReturnStatement::~ReturnStatement() {
  if(expression != NULL) delete expression;
  expression = NULL;
}
CRB_TYPE::StatementResult* ReturnStatement::execute() {
  if (this->expression) {
    auto value = this->expression->eval_and_pop();
    // because the stack pop need to delete
    // here we use it instead of value_copy
    return new CRB_TYPE::StatementResult(value); 
  } else {
    return new CRB_TYPE::StatementResult(new CRB_TYPE::Value());
  }
}
BreakStatement::BreakStatement(): Statement(CRB_TYPE::BREAK_STATEMENT) {

}
BreakStatement::~BreakStatement() {

}
CRB_TYPE::StatementResult* BreakStatement::execute() {
  return new CRB_TYPE::StatementResult(CRB_TYPE::BREAK_STATEMENT_RESULT);
}
ContinueStatement::ContinueStatement(): Statement(CRB_TYPE::CONTINUE_STATEMENT) {

}
ContinueStatement::~ContinueStatement() {

}
CRB_TYPE::StatementResult* ContinueStatement::execute() {
  return new CRB_TYPE::StatementResult(CRB_TYPE::CONTINUE_STATEMENT_RESULT);
}
GlobalStatement::GlobalStatement(IdentifierList* identifier_list_) :Statement(CRB_TYPE::GLOBAL_STATEMENT) {
  identifier_list = identifier_list_;
}
GlobalStatement::~GlobalStatement() {
  if(identifier_list != NULL) delete identifier_list;
  identifier_list = NULL;
}
CRB_TYPE::StatementResult* GlobalStatement::execute() {
  auto Ienv = Interpreter::getInstance()->get_environment();
  for (int i = 0; i < identifier_list->_identifier_vec.size(); i++) {
    Ienv->add_global_declare(*identifier_list->_identifier_vec[i]);
  }
  return new CRB_TYPE::StatementResult(CRB_TYPE::NORMAL_STATEMENT_RESULT);
}
Block::Block(StatementList* statement_list_) {
  statement_list = statement_list_;
}
Block::~Block() {
  if(statement_list != NULL) delete statement_list;
  statement_list = NULL;
}
CRB_TYPE::StatementResult* Block::execute() {
  if (statement_list) {
    return statement_list->execute();
  } else {
    return new CRB_TYPE::StatementResult(CRB_TYPE::NORMAL_STATEMENT_RESULT);
  }
}
