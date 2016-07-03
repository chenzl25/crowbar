# Parser

In this project we will let parser and lexer work together to implement a `crowbar` interpreter.

**now the code only work on OS X**

### *How to run*

1. go to the parser root directory.
2. run `make` which will output a `myyacc`
3. run `.myyacc` which will output a `dist` directory
4. run `cd dist`
5. run `make` which will output a `dcompile` executable file
6. run `./dcompile -code xxx`, here `xxx` is the path of the code written by crowbar you want to interpret.

## File Structure
1. src
	1. main.cpp:  combine lexer and parser to generate the source code of crowbar
	2.  bnf_rule.(cpp/h):  BNF rule.
	3. slr.(cpp/h):   SLR  bottom-up parser
	4. yacc.(cpp/h): a SLR driven  parser generator.
	5. hardcode.(cpp/h): yacc generate hardcode(*bnf_rule, slr, lex*) here 
2. extend: include all the file about the implementation of crowbar
3. template: include some template used by yacc to produce it output.
4. data : include `crowbar.bnf` which describes the syntax of crowbar and `crowbar.lex` which describes the lexeme of crowbar. and other test code. *sorry for mix them here*

## *Sample Code*
BNF to describe the syntax of calculator. (really like yacc)
```
# -----------------------------------------------------------------
# union have default : type int_value, double_value, string_value
# token have default : <double_value> factor , <int_value>  number
# -----------------------------------------------------------------

# ----------------------------------------
%token <double_value> factor
%token <int_value>    number
# ----------------------------------------

%include {
  #include <vector>
}

%union {
    // int          int_value;
    // double       double_value;
    // string       *string_value;
}

%type <double_value> expression term primary_expression

$ expression          : term
                      | expression '+' term
                      {
                          $$ = $1 + $3;
                      }
                      | expression '-' term
                      {
                          $$ = $1 - $3;
                      }
                      | 'print' expression
                      {
                        printf("%lf\n",$2);
                      }
                      ;

$ term                : primary_expression
                      | term '*' primary_expression 
                      {
                          $$ = $1 * $3;
                      }
                      | term '/' primary_expression
                      {
                          $$ = $1 / $3;
                      }
                      ;

$ primary_expression  : number
                      | factor
                      | '(' expression ')'
                      {
                          $$ = $2;
                      }
                      | '-' primary_expression
                      {
                          $$ = -$2;
                      }
                      ;
```

