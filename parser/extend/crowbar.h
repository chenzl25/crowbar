#ifndef CROWBAR_H
#define CROWBAR_H
#include <string>
#include <vector>
#include "crowbar_type.h"

#define is_numeric_type(type) \
  ((type) == CRB_TYPE::INT_VALUE || (type) == CRB_TYPE::DOUBLE_VALUE)
#define is_math_operator(op) \
  ((op) == CRB_TYPE::ADD_EXPRESSION || (op) == CRB_TYPE::SUB_EXPRESSION || \ 
   (op) == CRB_TYPE::MUL_EXPRESSION || (op) == CRB_TYPE::DIV_EXPRESSION || \
   (op) == CRB_TYPE::MOD_EXPRESSION)
#define is_compare_operator(op) \
  ((op) == CRB_TYPE::EQ_EXPRESSION || (op) == CRB_TYPE::NE_EXPRESSION || \ 
   (op) == CRB_TYPE::GT_EXPRESSION || (op) == CRB_TYPE::GE_EXPRESSION || \
   (op) == CRB_TYPE::LT_EXPRESSION || (op) == CRB_TYPE::LE_EXPRESSION)
#define is_logical_operator(op) \
  ((op) == CRB_TYPE::LOGICAL_AND_EXPRESSION || (op) == CRB_TYPE::LOGICAL_OR_EXPRESSION)
#define is_object_value(type) \
  ((type) == CRB_TYPE::STRING_VALUE || (type) == CRB_TYPE::ARRAY_VALUE || \
   (type) == CRB_TYPE::ASSOC_VALUE  || (type) == CRB_TYPE::SCOPE_CHAIN_VALUE)

using namespace std;

class ParameterList {
public:  
  ParameterList();
  ~ParameterList();
  void add_parameter(string* identifier);
  vector<string*> _parameter_vec;
private:

}; 

class Expression;
class ExpressionList;
class Block;  
class FunctionDefinition;

class ArgumentList {
public:  
  ArgumentList();
  ~ArgumentList();
  void add_argument(Expression* expression);
  vector<Expression*> _argument_vec;
private:

};

class Expression {
public:  
  Expression(CRB_TYPE::ExpressionType type_);
  virtual ~Expression() = default;
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  int line;
  CRB_TYPE::ExpressionType type;
private:
};
class IntExpression : public Expression {
public:  
  IntExpression(int int_value_);
  virtual ~IntExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  int int_value;
};
class DoubleExpression : public Expression {
public:  
  DoubleExpression(double double_value_);
  virtual ~DoubleExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  double double_value;
};
class StringExpression : public Expression {
public:  
  StringExpression(string *string_value_);
  virtual ~StringExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  string* string_value;
};
class BooleanExpression : public Expression {
public:  
  BooleanExpression(bool boolean_value_);
  virtual ~BooleanExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  bool boolean_value;
};
class NullExpression : public Expression {
public:  
  NullExpression();
  virtual ~NullExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
};
class IdentifierExpression : public Expression {
public:  
  IdentifierExpression(string *identifier_);
  virtual ~IdentifierExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  string *identifier;
};
class MinusExpression : public Expression {
public:  
  MinusExpression(Expression *operand_);
  virtual ~MinusExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  Expression *operand;
};
class LogicalNotExpression : public Expression {
public:  
  LogicalNotExpression(Expression *operand_);
  virtual ~LogicalNotExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  Expression *operand;
};
class BinaryExpression : public Expression {
public:  
  BinaryExpression(CRB_TYPE::ExpressionType operator_type_,
                   Expression *left_,
                   Expression *right_);
  virtual ~BinaryExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  CRB_TYPE::Value* constant_folding_eval();
  Expression *left, *right;
};

class IncrementExpression : public Expression {
public:  
  IncrementExpression(Expression *operand_);
  virtual ~IncrementExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  Expression *operand;
};

class DecrementExpression : public Expression {
public:  
  DecrementExpression(Expression *operand_);
  virtual ~DecrementExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  Expression *operand;
};


class IndexExpression : public Expression {
public:  
  IndexExpression(Expression *array, Expression *index);
  virtual ~IndexExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  Expression *array;
  Expression *index;
};

class FunctionCallExpression :public Expression {
public:
  FunctionCallExpression(Expression *function_, ArgumentList *argument_list_);
  virtual ~FunctionCallExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  Expression* function;
  ArgumentList* argument_list;
private:
};

class MemberExpression :public Expression {
public:
  MemberExpression(Expression *expression_, string *member_name_);
  virtual ~MemberExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  Expression* expression;
  string* member_name;
private:
};


class ArrayExpression :public Expression {
public:
  ArrayExpression(ExpressionList *array_literal_);
  virtual ~ArrayExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  ExpressionList* array_literal;
private:
};
class ClousreExpression :public Expression {
public:
  ClousreExpression(string *identifier_, ParameterList *parameter_list_,
                   Block *block_);
  virtual ~ClousreExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  FunctionDefinition* function_definition;
private:
};

class AssignExpression : public Expression {
public:  
  AssignExpression( CRB_TYPE::ExpressionType assign_type_, 
                    Expression *variable_,
                    Expression *operand_);
  virtual ~AssignExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  Expression *variable, *operand;
};
class CommaExpression : public Expression {
public:  
  CommaExpression(Expression *left_, Expression *right_);
  virtual ~CommaExpression();
  virtual void eval();
  virtual CRB_TYPE::Value* eval_and_pop();
  Expression *left, *right;
};

class Elsif {
public:  
  Elsif(Expression *condition_, Block *block_);
  ~Elsif();
  CRB_TYPE::StatementResult* execute();
  Expression* condition;
  Block* block;
private:

};           
class ElsifList {
public:  
  ElsifList();
  ~ElsifList();
  CRB_TYPE::StatementResult* execute();
  void add_elsif(Elsif * elsif);
  vector<Elsif*> _elsif_vec;
private:

};            

class IdentifierList {
public:  
  IdentifierList();
  ~IdentifierList();
  void add_identifier(string *identifier);
  vector<string*> _identifier_vec;
private:

};     
class ExpressionList {
public:
  ExpressionList();
  ~ExpressionList();
  void add_expression(Expression* expression_);
  vector<Expression*> _expression_vec;
private:

};

typedef CRB_TYPE::Value* NativeFunctionProc(int arg_cnt);

class FunctionDefinition {
public:
  FunctionDefinition();
  FunctionDefinition(string *name_, ParameterList *parameter_list_, Block *block_, bool is_closure_);
  ~FunctionDefinition();
  string *name;
  CRB_TYPE::FunctionDefinitionType  type;
  bool            is_closure;
  ParameterList   *parameter_list;
  Block           *block;
  NativeFunctionProc *proc;
};


////////////////////////////////////////////////////////////////////////////////////

class Statement {
public:  
  Statement(CRB_TYPE::StatementType type_);
  virtual ~Statement();
  virtual CRB_TYPE::StatementResult* execute();
  CRB_TYPE::StatementType type;
  int line;
private:

};          
class StatementList {
public:  
  StatementList();
  ~StatementList();
  virtual CRB_TYPE::StatementResult* execute();
  void add_statement(Statement *statement);
  vector<Statement*> _statement_vec;
private:

};  



class IfStatement : public Statement {
public:  
  IfStatement(Expression *condition_, Block *then_block_, 
              ElsifList *elsif_list_, Block *else_block_);
  virtual ~IfStatement();
  virtual CRB_TYPE::StatementResult* execute();
  Expression* condition;
  Block*      then_block;
  ElsifList*  elsif_list;
  Block*      else_block;
private:
};

class WhileStatement : public Statement {
public:  
  WhileStatement(Expression *condition_, Block *block_);
  virtual ~WhileStatement();
  virtual CRB_TYPE::StatementResult* execute();
  Expression* condition;
  Block*      block;
private:
};
class ForStatement : public Statement {
public:  
  ForStatement(Expression *init_, Expression *cond_,
               Expression *post_, Block *block_);
  virtual ~ForStatement();
  virtual CRB_TYPE::StatementResult* execute();
  Expression* init;
  Expression* cond;
  Expression* post;
  Block*      block;
private:
};

class ExpressionStatement : public Statement {
public:  
  ExpressionStatement(Expression *expression_);
  virtual ~ExpressionStatement();
  virtual CRB_TYPE::StatementResult* execute();
  Expression* expression;
private:
};

class ReturnStatement : public Statement {
public:  
  ReturnStatement(Expression *expression_);
  virtual ~ReturnStatement();
  virtual CRB_TYPE::StatementResult* execute();
  Expression* expression;
private:
};
class BreakStatement : public Statement {
public:  
  BreakStatement();
  virtual ~BreakStatement();
  virtual CRB_TYPE::StatementResult* execute();
private:
};
class ContinueStatement : public Statement {
public:  
  ContinueStatement();
  virtual ~ContinueStatement();
  virtual CRB_TYPE::StatementResult* execute();
private:
};
class GlobalStatement : public Statement {
public:  
  GlobalStatement(IdentifierList* identifier_list_);
  virtual ~GlobalStatement();
  virtual CRB_TYPE::StatementResult* execute();
  IdentifierList* identifier_list;
private:
};
class Block {
public:  
  Block(StatementList* statement_list_);
  ~Block();
  CRB_TYPE::StatementResult* execute();
  StatementList* statement_list;
private:

};

#endif