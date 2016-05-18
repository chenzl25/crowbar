#include <iostream>
#include <string>
#include <stdlib.h>
#include "re_parse.h"
using namespace std;
//*********************************
// regular expression like that
// (), |, `concatenate`, , *, .
// '.' stand for arbitrary symbol
//*********************************

// #define DEGUG
#define EOF -1

struct TreeNode;

static string input_string; // the global input_string
static int cur_pos;         // current position of input_string  
static char cur_char, lookahead;       // input_string[cur_pos]

TreeNode* parse_char();
TreeNode* parse_or();
TreeNode* parse_cat();
TreeNode* parse_star();
TreeNode* parse_re();
TreeNode* parse(string s);
void init();
void move();
void match(char t);
void visit_print(TreeNode* root);
void debug(string s);


// int main(int argc, char**argv) {
//   TreeNode* root = parse("((ab|cd|e)*(hg)*)*)");
//   visit_print(root);
// }

// the api for the other files
TreeNode* parse(string s) {
  input_string = s;
  init();
  return parse_re();
}


//  Grammer
//  re   -> or
//  or   -> or '|' cat | cat
//  cat  -> cat star   | star
//  star -> char '*'   | char
//  char -> '(' re ')' | '.' | 'a' | ..... | 'z' | .....(other characters)

// Invariant:
// 1.before to parse we let cur_pos point to the start position
// 2.after the parse we let cur_pos point to the last after the end position 


TreeNode* parse_re() {
  debug("parse_re");
  return parse_or();
}
TreeNode* parse_or() {
  debug("parse_or");
  TreeNode* root = parse_cat();
  while (cur_char == '|') {
    move();
    root = new TreeNode(ENUM::TYPE_OR, NULL, root, parse_cat());
  }
  return root;
}
TreeNode* parse_cat() {
  debug("parse_cat");
  TreeNode* root = parse_star();
  while (cur_char != EOF &&
         cur_char != '*' &&
         cur_char != ')' &&
         cur_char != '|') {
    root = new TreeNode(ENUM::TYPE_CAT, NULL, root, parse_star());
  }
  return root;
}

TreeNode* parse_star() {
  debug("parse_star");
  TreeNode* root = parse_char();
  if (cur_char == '*' && lookahead == '*') {
  	cout << "repetition-operator *" <<endl;
    exit(-1);
  }
  if (cur_char == '*' && lookahead != '*') {
    move();
    root = new TreeNode(ENUM::TYPE_STAR, NULL, root, NULL);
  }
  return root;
}

TreeNode* parse_char() {
  debug("parse_char");
  TreeNode* root = NULL;
  switch (cur_char) {
    case '(' :
      move();
      root = parse_re();
      match(')');
      break;
    default:
      root = new TreeNode(ENUM::TYPE_CHAR, cur_char, NULL, NULL);
      move();
  }
  return root;
}



// move the cur_pos
void move() {
  cur_pos++;
  if (cur_pos < input_string.length()) cur_char = input_string[cur_pos];
  else cur_char = EOF;

  if (cur_pos+1 < input_string.length()) lookahead = input_string[cur_pos+1];
  else lookahead = EOF;
}
// assert to match the character
void match(char t) {
  if (cur_char != t) {
    cout << "syntax error, we expect " << t << endl;
    exit(-1);
  }
  move();
}

void visit_print(TreeNode* root) {
  if (root) {
    visit_print(root->left);
    visit_print(root->right);
    switch (root->type) {
      case ENUM::TYPE_CAT:
        cout << " -";
        break;
      case ENUM::TYPE_OR:
        cout << " |";
        break;
      case ENUM::TYPE_STAR:
        cout << " *";
        break;
      case ENUM::TYPE_CHAR:
        cout << " "  << root->ch;
        break;
      default:
        break;
    }
  }
}

void init() {
  // here we avoid the repetition of the * and spacelike character
  for (int i = 0; i < input_string.length(); i++) {
    if (input_string[i] == ' ' || 
        input_string[i] == '\t'||
        input_string[i] == '\n') {
      cout << "the regular expression should not contain the spacelike character" << endl;
      exit(-1);
    }
  }
  // prepare to parse. Before to parse we let the cur_pos point to the start position
  cur_pos = -1;
  move();
}

void debug(string s) {
#ifdef DEGUG
  cout << s << endl;
#endif
}

