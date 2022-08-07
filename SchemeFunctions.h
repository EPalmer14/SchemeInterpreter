//
// Created by Zeke Palmer on 4/21/22.
//

#ifndef PROJECT_SCHEMEFUNCTIONS_H
#define PROJECT_SCHEMEFUNCTIONS_H

#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "parser.h"

// Functions for evaluating different scheme commands

SExp car(SExp firstSExp);

SExp cdr(SExp firstSExp);

SExp quote(SExp firstSExp);

SExp cons(SExp firstSExp, SExp secondSExp);

SExp Symbol(SExp firstSExp);

SExp null(SExp firstSExp);

SExp append(SExp firstSExp, SExp secondSExp);

SExp equal(SExp firstSExp, SExp secondSExp);

SExp assoc(SExp symb, SExp assocList);

#endif //PROJECT_SCHEMEFUNCTIONS_H
