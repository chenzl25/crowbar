#include <string>
#include <iostream>
#include "util.h"
using namespace std;
// #define DEBUG
void debug(string s) {
#ifdef DEBUG
  cout << s << endl;
#endif
}
void assert(bool should, string msg) {
  if (!should) {
    cout << msg << endl;
    exit(-1);
  }
}
void error(string msg) {
  cout << msg << endl;
}
bool isdigit(char c) {
  if (c >= '0' && c <= '9') {
    return true;
  } else {
    return false;
  }
}
bool is_uppercase_letter(char c) {
  if (c >= 'A' && c <= 'Z') {
    return true;
  }
  return false;
}
bool is_lowercase_letter(char c) {
  if (c >= 'a' && c <= 'z') {
    return true;
  }
  return false;
}