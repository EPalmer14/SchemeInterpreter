//
// Created by Zeke Palmer on 2/24/22.
//

#ifndef PARSER
#define PARSER
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"

// A cons cell structure with pointers to a car and cdr and a place for a string for symbols
struct conscell {
    char symbol[20];
    struct conscell *car;
    struct conscell *cdr;
};

// New reference name CC
typedef struct conscell *CC;

// A struct for a Scheme Expression list. Contains just one cons cell as the head of the list
struct SExp {
    CC head;
};

typedef struct SExp *SExp;

char token[20];

CC create_conscell();

SExp create_list();

CC S_Expression(int depth);

SExp eval(SExp list);

int isSymbol(CC c);
int isList(CC c);
int isEmptyList(CC c);

void printSExp(SExp startOfList);
void printSExpHelper(CC cell, int paren);



#endif
