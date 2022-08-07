#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "SchemeFunctions.h"

int main() {
    for(;;) {
        startTokens(20);
        printf("Enter a valid lisp expression \n");
        strcpy(token, getToken());
        SExp list = create_list();
        list->head = S_Expression(0);
        SExp evalSExp = eval(list);
        printSExp(evalSExp);
    }
}

