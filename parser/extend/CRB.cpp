#include "CRB.h"
#include "../lexer/src/util.h"

namespace CRB {

Interpreter* Interpreter::_instance = NULL;

Interpreter::Interpreter() {
  _heap = new Heap();
  _stack = new Stack();
  _environment = new Environment();
  _statement_list = new StatementList();
}
Interpreter::~Interpreter() { // order is importance
  delete _environment;
  delete _stack;
  delete _heap;  // heap should delete after environment
  delete _statement_list;  // statemet at last
}
void Interpreter::dealloc() {
  delete _instance;
  _instance = NULL;
}

Interpreter* Interpreter::getInstance() {
  if (_instance == NULL) {
    _instance = new Interpreter();
  } 
  return _instance;
}

void Interpreter::chain_statement_list(Statement* statement) {
  _statement_list->add_statement(statement);
}
void Interpreter::set_line(int line_) {
  _line = line_;
}
int Interpreter::get_line() {
  return _line;
}
Interpreter::Heap* Interpreter::get_heap() {
  return _heap;
}
Interpreter *create_interpreter(void) {

}
void compile(Interpreter *interpreter, string code_path) {

}
void interpret(Interpreter *interpreter) {

}
void dispose_interpreter(Interpreter *interpreter) {

}
Interpreter::Stack* Interpreter::get_stack() {
  return _stack;
}
Interpreter::Stack::Stack() {

}
Interpreter::Stack::~Stack() {
  // TODO
}
int Interpreter::Stack::size() {
  return _stack_vec.size();
}
CRB_TYPE::Value* Interpreter::Stack::pop() {
  auto result = _stack_vec[_stack_vec.size()-1];
  _stack_vec.erase(_stack_vec.end()-1);
  return result;
}
CRB_TYPE::Value* Interpreter::Stack::peek(int pos) {
  assert(_stack_vec.size() > pos, "can't peek the stack over the size");
  return _stack_vec[_stack_vec.size() - pos - 1];
}
void Interpreter::Stack::push(CRB_TYPE::Value* v) {
  _stack_vec.push_back(v);
}
Interpreter::Heap::Heap() {
  _threshold = (1024 * 256); // 256K
  _current_size = 0;
}
Interpreter::Heap::~Heap() {
  // TODO
  cout << "-------------------------------------------------" << endl;
  cout << "Heap delete :" << endl; 
  for (auto it = _heap_list.begin(); it != _heap_list.end(); it++) {
    cout << *it << endl;
    (*it)->print();
    delete *it;
  }
}
CRB_TYPE::Object* Interpreter::Heap::alloc(CRB_TYPE::ObjectType type_) {
  CRB_TYPE::Object* alloc_ptr;
  switch (type_) {
    case CRB_TYPE::STRING_OBJECT:
      error("should use another string alloc");
      break;
    case CRB_TYPE::ARRAY_OBJECT:

      break;
    case CRB_TYPE::SCOPE_CHAIN_OBJECT:
      alloc_ptr = new CRB_TYPE::ScopeChain();
      _current_size += sizeof(CRB_TYPE::ScopeChain);
      _heap_list.push_back(alloc_ptr);
      break;
    case CRB_TYPE::ASSOC_OBJECT: {
      alloc_ptr = new CRB_TYPE::Assoc();
      _current_size += sizeof(CRB_TYPE::Assoc);
      _heap_list.push_back(alloc_ptr);
      break;
    }
    default:
      error("heap alloc type error");
  }
}
CRB_TYPE::Object* Interpreter::Heap::alloc(string* string_value, bool is_literal_) {
  CRB_TYPE::Object* alloc_ptr;
  if (is_literal_) { // when gc we do not sweep literal
    alloc_ptr = new CRB_TYPE::String(string_value, is_literal_);
    _heap_list.push_back(alloc_ptr);
  } else {
    alloc_ptr = new CRB_TYPE::String(new string(*string_value), is_literal_);
    _current_size += sizeof(CRB_TYPE::String) + string_value->length();
    _heap_list.push_back(alloc_ptr);
  }
  return alloc_ptr;
}
void Interpreter::execute() {
  _statement_list->execute();
}


Interpreter::Environment* Interpreter::get_environment() {
  return _environment;
}
Interpreter::Environment::Environment() {
  _in_global = true;
}
Interpreter::Environment::~Environment() {
  cout << "-------------------------------------------------" << endl;
  cout << "Environment delete : " << endl;
  for (auto it = _global_variable_map.begin(); it != _global_variable_map.end(); it++) {
    if (!is_object_value(it->second->type)) {
      cout << it->first << " : " << endl;
      it->second->print();
      delete it->second;
    }
  }
  // for (auto it : _global_function_map) {
  //   delete it.second;
  // }
}
void Interpreter::Environment::alloc_env(CRB_TYPE::ScopeChain* next_) {
  _scope_chain = new CRB_TYPE::ScopeChain(next_);
  _in_global = false;
}
void Interpreter::Environment::dealloc_env() {
  delete _scope_chain;
  _in_global = true;
}



// also use to add global varible
void Interpreter::Environment::add_variable(string name, CRB_TYPE::Value* local_value) {
  if (_in_global) { 
    if (_global_function_map.count(name)) {
      error("there are same function name: " +name+ " in global");
    }
    _global_variable_map[name] = local_value;
    cout << "add global varible: " << name << endl;
    local_value->print();
  } else {
    _scope_chain->frame->add_member(name, local_value);
  }
}
void Interpreter::Environment::add_global_declare(string name) {
  if (_in_global) { 
    error("global statement in global environment");
  } else {
    if (_global_variable_map.count(name)) {
      _global_declare_map[name] = _global_variable_map[name]; 
    } else {
      error("without this varible in global environment");
    }
  }
}
CRB_TYPE::Value* Interpreter::Environment::search_variable(string name) {
  if (_in_global) {
    return _global_variable_map[name];
  } else {
    CRB_TYPE::Value* result = _scope_chain->frame->search_member(name);
    if (result != NULL) return result;
    else return _global_declare_map[name];
  }
  return NULL;
}
FunctionDefinition* Interpreter::Environment::search_function(string name) {
  return _global_function_map[name];
}
void Interpreter::Environment::assign_variable(string name, CRB_TYPE::Value* assign_value) {
  if (_in_global) {
    assert(_global_variable_map.count(name), "the value be assigned should exist");
    if (is_object_value(_global_variable_map[name]->type)) {
      // remain for the Heap to delete
    } else {
      delete _global_variable_map[name];
      _global_variable_map.erase(name);
    }
    _global_variable_map[name] = assign_value;
    cout << "assign global varible: " << name << endl;
    assign_value->print();
    return;
  } else {
    _scope_chain->frame->assign_member(name, assign_value);
  }
}
void Interpreter::Environment::add_function(FunctionDefinition* fd) {
  if (_global_function_map.count(*(fd->name))) {
    error("multi definition of " + *(fd->name));
  }
  _global_function_map[*(fd->name)] = fd;
  cout << "add function: " << *(fd->name) << endl;
}


} // CRB

