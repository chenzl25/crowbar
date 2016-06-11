#include "CRB.h"
#include "native_func.h"
#include "fake_method.h"
#include "crowbar_util.h"
#include <set>
using namespace std;

namespace CRB {

Interpreter* Interpreter::_instance = NULL;

Interpreter::Interpreter() {
  _heap = new Heap();
  _stack = new Stack();
  _environment = new Environment();
  _statement_list = new StatementList();
}
Interpreter::~Interpreter() { // order is importance
  delete _stack;
  delete _environment;
  delete _heap;  // heap should delete after environment
  delete _statement_list;  // statemet at last
}
void Interpreter::dealloc() {
  cout << "\n=======================================\n";
  delete _instance;
  _instance = NULL;
}

Interpreter* Interpreter::getInstance() {
  if (_instance == NULL) {
    _instance = new Interpreter();
    _instance->before_hook();
  } 
  return _instance;
}
void Interpreter::before_hook() {
  CRB::add_native_function(); // from native_fun.cpp
  CRB::add_fake_method();     // from fake_method.cpp
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

// when we use Stack. we need to delete when stack pop garbage (which doesn't use again)
// to simplify , we just need to delete.
// the value will just delete itself without deleting Object (which maintains by Heap)
// so when we alloc a Object we need to use heap
// however the Value itself, we need to delete by hand.
// sometimes what we pop will be used for other aspect, like environment and other
// then we use copy_value to main the pop-delete invariant
// copy_value may be deep or shallow
Interpreter::Stack::Stack() {

}
Interpreter::Stack::~Stack() {
  assert(_stack_vec.size() == 0, "after execution, there should nothing in the stack");
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
  CRB::assert(_stack_vec.size() > pos, "can't peek the stack over the size");
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
    // cout << *it << endl;
    // (*it)->print();
    auto cast_value = dynamic_cast<CRB_TYPE::Value*>(*it);
    heap_value_delete(cast_value);
  }
  cout << "-------------------------------------------------" << endl;
}



CRB_TYPE::Object* Interpreter::Heap::alloc(CRB_TYPE::ObjectType type_) {
  CRB_TYPE::Object* alloc_ptr;
  switch (type_) {
    case CRB_TYPE::STRING_OBJECT:
      CRB::error("should use another string alloc");
      break;
    case CRB_TYPE::ARRAY_OBJECT:
      alloc_ptr = new CRB_TYPE::Array();
      _current_size += sizeof(CRB_TYPE::Array);
      _heap_list.push_back(alloc_ptr);
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
      CRB::error("heap alloc type error");
  }
  return alloc_ptr;
}

set<CRB_TYPE::Object*> fix_bug_set;

CRB_TYPE::Object* Interpreter::Heap::alloc(string* string_value, bool is_literal_) {
  CRB_TYPE::Object* alloc_ptr = NULL;
  AGAIN:
  if (is_literal_) { // when gc we do not sweep literal
    alloc_ptr = new CRB_TYPE::String(string_value, is_literal_);
  } else {
    alloc_ptr = new CRB_TYPE::String(new string(*string_value), is_literal_);
    _current_size += sizeof(CRB_TYPE::String) + string_value->length();
  }
  if (fix_bug_set.count(alloc_ptr)) { // so strang here
    goto AGAIN;
  }
  fix_bug_set.insert(alloc_ptr);
  // cout << alloc_ptr << endl;
  _heap_list.push_back(alloc_ptr);
  return alloc_ptr;
}
void Interpreter::execute() {
  _statement_list->execute();
}


Interpreter::Environment* Interpreter::get_environment() {
  return _environment;
}
Interpreter::Environment::Environment() {
  _use_env = NULL;
}
Interpreter::Environment::~Environment() {
  cout << "-------------------------------------------------" << endl;
  cout << "Environment delete : " << endl;
  for (auto it = _global_variable_map.begin(); it != _global_variable_map.end(); it++) {
    env_value_delete(it->second);
  }
  for (auto it = _global_function_map.begin(); it != _global_function_map.end(); it++) {
    // cout << "delete function: " << *it->second->name << endl;
    if (it->second->type = CRB_TYPE::NATIVE_FUNCTION_DEFINITION) {
      delete it->second->name;
    }
    delete it->second;
  }
  for (auto it = _global_fake_method_map.begin(); it != _global_fake_method_map.end(); it++) {
    // cout << "delete fake method: " << *it->second->name << endl;
    delete it->second->name;
    delete it->second;
  }
  cout << "-------------------------------------------------" << endl;
}

void Interpreter::Environment::alloc_env(CRB_TYPE::ScopeChain* next_) {
  _local_env_vec.push_back(new LocalEnv(next_));
}

LocalEnv::LocalEnv(CRB_TYPE::ScopeChain *next_) {
  auto Iheap = CRB::Interpreter::getInstance()->get_heap();
  _scope_chain = dynamic_cast<CRB_TYPE::ScopeChain*>(Iheap->alloc(CRB_TYPE::SCOPE_CHAIN_OBJECT));
  _scope_chain->frame = dynamic_cast<CRB_TYPE::Assoc*>(Iheap->alloc(CRB_TYPE::ASSOC_OBJECT));
  _scope_chain->next = next_;
}
LocalEnv::~LocalEnv() {
  // delete _scope_chain;   remain for heap
  _scope_chain = NULL;
}
void Interpreter::Environment::dealloc_env() {
  assert(_local_env_vec.size() > 0, "dealloc_env size should > 0");
  delete _local_env_vec[_local_env_vec.size()-1];
  _local_env_vec.erase(_local_env_vec.end()-1);
}



// also use to add global varible
void Interpreter::Environment::add_variable(string name, CRB_TYPE::Value* local_value) {
  if (_use_env == NULL) { 
    if (_global_function_map.count(name)) {
      CRB::error("there are same function name: " +name+ " in global");
    }
    CRB::assert(_global_variable_map.count(name) == 0, "add variable should just init");
    _global_variable_map[name] = local_value;
    // cout << "add global varible: " << name << endl;
    // local_value->print();
  } else {
    // cout << "add local varible: " << name << endl;
    // local_value->print();
    _use_env->_scope_chain->frame->add_member(name, local_value);
  }
}
LocalEnv* Interpreter::Environment::get_top_env() {
  if (_local_env_vec.empty()) {
    return NULL;
  } else {
    return _local_env_vec[_local_env_vec.size()-1];
  }
}
LocalEnv* Interpreter::Environment::get_use_env() {
  return _use_env;
}
void Interpreter::Environment::use_env(LocalEnv* use_env) {
  _use_env = use_env;
}
void Interpreter::Environment::add_global_declare(string name) {
  if (_use_env == NULL) {
    CRB::error("global statement in global environment");
  } else {
    if (_global_variable_map.count(name)) {
      _use_env->_global_declare_map[name] = _global_variable_map[name]; 
    } else {
      CRB::error("without varible : < " + name + " > in global environment");
    }
  }
}
CRB_TYPE::Value* Interpreter::Environment::search_variable(string name) {
  if (_use_env == NULL) {
    if (_global_variable_map.count(name)) {
      return _global_variable_map[name];
    } else {
      return NULL;
    }
  } else {
    CRB_TYPE::Value* result;
    auto head_scope = _use_env->_scope_chain;
    result = _use_env->_global_declare_map[name];
    if (result) return result;

    do {
      result = head_scope->frame->search_member(name);
      if (result != NULL) return result;
      else head_scope = head_scope->next;
    } while(head_scope);

  }
  return NULL;
}

void Interpreter::Environment::assign_variable(string name, CRB_TYPE::Value* assign_value) {
  if (_use_env == NULL) {
    CRB::assert(_global_variable_map.count(name), "the value be assigned should exist");
    env_value_delete(_global_variable_map[name]);
    _global_variable_map[name] = assign_value;
    // cout << "assign global varible: " << name << endl;
    // assign_value->print();
    return;
  } else {
    CRB_TYPE::Value* dest;
    auto head_scope = _use_env->_scope_chain;
    if (_use_env->_global_declare_map.count(name)) {
      env_value_delete(_global_variable_map[name]);
      _global_variable_map[name] = assign_value;
      _use_env->_global_declare_map[name] = assign_value;
      return;
    }

    do {
      dest = head_scope->frame->search_member(name);
      if (dest != NULL) head_scope->frame->assign_member(name, assign_value);
      else head_scope = head_scope->next;
    } while(head_scope);

    CRB::error("the value be assigned should exist");
  }
}
void Interpreter::Environment::add_function(FunctionDefinition* fd) {
  if (_global_function_map.count(*(fd->name))) {
    CRB::error("multi definition of " + *(fd->name));
  }
  _global_function_map[*(fd->name)] = fd;
  // cout << "add function: " << *(fd->name) << endl;
}
FunctionDefinition* Interpreter::Environment::search_function(string name) {
  if (_global_function_map.count(name)) {
    return _global_function_map[name];
  } else {
    return NULL;
  }
}
void Interpreter::Environment::add_fake_method(FunctionDefinition* fd) {
  if (_global_fake_method_map.count(*(fd->name))) {
    CRB::error("multi definition of fake method " + *(fd->name));
  }
  _global_fake_method_map[*(fd->name)] = fd;
}
FunctionDefinition* Interpreter::Environment::search_fake_method(string name) {
  if (_global_fake_method_map.count(name)) {
    return _global_fake_method_map[name];
  } else {
    return NULL;
  }
}

} // CRB

