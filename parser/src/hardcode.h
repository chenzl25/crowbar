#ifndef HARDCODE_H
#define HARDCODE_H


#include <vector>
#include "../../lexer/src/lex.h"
#include "slr.h"
#include "bnf_rule.h"

using namespace std;

vector<Lex::LexRule> hard_code_lex_rule();
vector<BnfRule> hard_code_bnf_rule();
void hard_code_slr_table(SLR &slr);

#endif