#ifndef UTIL_H
#define UTIL_H
#include <string>
using namespace std;
void debug(string s);
void assert(bool should, string msg);
void error(string msg); 
bool isdigit(char c);
bool is_uppercase_letter(char c);
bool is_lowercase_letter(char c);

#endif