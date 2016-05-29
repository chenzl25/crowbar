#ifndef YYSTYPE_H
#define YYSTYPE_H

#include <string>
using namespace std;

/*union*/
typedef union YYSTYPE
{
    int          int_value;
    double       double_value;
    string       *string_value;
};

#endif