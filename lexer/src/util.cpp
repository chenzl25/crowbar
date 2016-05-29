#include <string>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include "util.h"
using namespace std;

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
  cout << msg << endl;
  exit(-1);
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

string trim(string& str) {
  size_t first = str.find_first_not_of(' ');
  size_t last  = str.find_last_not_of(' ');
  return str.substr(first, last - first+1);
}


int make_dir(string path) {
  return mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
