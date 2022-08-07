//
// Created by Zeke Palmer on 2/24/22.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "SchemeFunctions.h"
#include "lexer.h"
#include "parser.h"

// Initializes a conscell
CC create_conscell() {
    CC NewCell = (CC) malloc(sizeof(struct conscell));
    strcpy(NewCell->symbol, "");
    NewCell->car = NULL;
    NewCell->cdr = NULL;
    return NewCell;
}

// Initializes an SExp
SExp create_list() {
    SExp list = (SExp) malloc(sizeof(struct SExp));
    list->head = NULL;
    return list;
}

// Checks if a conscell is a list cell
int isList(CC c) {
    if (c->car == NULL && c->cdr == NULL) {
        return 0;
    } else {
        return 1;
    }
}

// Checks if a conscell is empty AKA everything is null
int isEmptyList(CC c) {
    if (c->cdr == NULL && c->car == NULL && strcmp(c->symbol, "") == 0) {
        return 1;
    } else {
        return 0;
    }
}

// Checks if a conscell is a symbol cell
int isSymbol(CC c) {
    if (strcmp(c->symbol, "") != 0) {
        return 1;
    } else {
        return 0;
    }
}

// Will print an SExp by using a helper function that is recursive and takes a conscell as its first argument
void printSExp(SExp startOfList) {
    printSExpHelper(startOfList->head, 1);
    printf("\n");
}

// A helper function for printing out a Scheme Expression
void printSExpHelper(CC CCell, int paren) {
    // Start printing the expression recursively as long as it is not empty
    if (isEmptyList(CCell) == 0) {
        // If the current cell is a symbol than print it
        if (isSymbol(CCell)) {
            printf(" %s ", CCell->symbol);
        }
            // If not a symbol
        else {
            // If paren variable is currently 1 then it is the start of a new list, so print an open paren
            if (paren == 1) {
                printf("(");
            }
            // Function calls itself regardless of whether it just printed an open paren or not. It is either the start
            // of another list or a symbol in which case it will print the symbol and not reach the print open paren.
            // If it is not a symbol than it is the start of a new list so it will print the open paren again, thus always
            // call the function recusively with paren set to 1 here
            printSExpHelper(CCell->car, 1);
            // After printing the car of the cell, start the process of printing the cdr as long as cdr != NULL
            // If the cdr of this cell is null, print a closed paren as it is the end of the current list
            if (CCell->cdr == NULL) {
                printf(")");
            }
                // If the cdr is not null, begin to print the rest of this list
            else {
                printSExpHelper(CCell->cdr, 0);
            }
        }
    }
        // Otherwise it is an empty list so print ()
    else {
        printf("()");
    }
}


CC S_Expression(int depth) {
    // create two conscells
    CC list;
    CC listBuildHelper;
    // For a quote element. Can be entered as ' or by typing the word quote
    if (strcmp(token, "quote") == 0 || strcmp(token, "\'") == 0) {
        // For an input ' make the car's symbol quote
        if (strcmp(token, "\'") == 0) {
            // create a new conscell and make the symbol "quote"
            list = create_conscell();
            strcpy(list->symbol, "quote");
        }
            // If token already is quote, just strcpy the token
        else {
            // create a new conscell and make the symbol "quote"
            list = create_conscell();
            strcpy(list->symbol, token);
        }
        // If depth is 0 then there are no more tokens to read. Otherwise, get next token
        // Quote must be followed by more tokens so this should always get the next token
        if (depth != 0) {
            strcpy(token, getToken());
        }
        // return the conscell
        return list;
    } else if (strcmp(token, "()") == 0) {
        // Empty list is the same as null, so create a new CC and let everything remain null
        list = create_conscell();
        // create_conscell() already initializes everything to NULL so this is all that is needed
        // If depth is 0 then there are no more tokens to read. Otherwise, get next token
        if (depth != 0) {
            strcpy(token, getToken());
        }
        // return the conscell
        return list;
    }
    // check if token is an open parentheses
    else if (strcmp(token, "(") == 0) {
        // get the next token
        strcpy(token, getToken());
        // initialize current conscell
        list = create_conscell();
        // recursively find car of list
        list->car = S_Expression(depth + 1);
        // set the listBuildHelper equal to the current conscell of list
        listBuildHelper = list;
        // Recursively build the rest of the list until the list is closed.
        while (strcmp(token, ")") != 0) {
            // Create a new conscell for the rest of the list
            listBuildHelper->cdr = create_conscell();
            // Set the listBuildHelper equal to the cdr and build the car recursively from there
            listBuildHelper = listBuildHelper->cdr;
            listBuildHelper->car = S_Expression(depth + 1);
        }
        // If depth is 0 then there are no more tokens to read. Otherwise, get next token
        if (depth != 0) {
            strcpy(token, getToken());
        }
        // Return the built list
        return list;
    }
    // If none of the above, token must be a symbol
    else {
        // Token is symbol so create a new CC and strcpy the token into it's symbol
        list = create_conscell();
        strcpy(list->symbol, token);
        // If depth is 0 then there are no more tokens to read. Otherwise, get next token
        if (depth != 0) {
            strcpy(token, getToken());
        }
        // return the conscell
        return list;
    }
}

// Eval will evaluate an input S-Expression if it is containing any symbols that match with scheme commands
SExp eval(SExp expr) {
    // symbol will be the functions and any symbols in the input list
    char symbol[20];
    // Two SExp. One that will be returned to main and printed and one that will be recursively evaluated
    SExp returnSExp = create_list();
    SExp evaluationSExp = create_list();

    // if the input expr is an empty list return the list. Base case
    if (isEmptyList(expr->head)) {
        return expr;
    }

    // If the first element of the list is a symbol start the evaluation process
    if (isSymbol(expr->head->car)) {
        // copy the conscell symbol to the string symbol
        strcpy(symbol, expr->head->car->symbol);

        // exit the program if user types "exit"
        if (strcmp(symbol, "exit") == 0) {
            printf("Have a nice day!\n");
            exit(0);
        }
        // when there are no more symbols to evaluate just return the input
        if (expr->head->cdr == NULL) {
            return expr;
        }
            // Possible input commands. These can be used in tandem in larger calls because of the recursive evaluation
            // Car will return a list with the first element of the input
        else if (strcmp(symbol, "car") == 0) {
            // car(cdr(expr)) is the remaining elements of the list, so recursively call eval with the rest of the list
            evaluationSExp = eval(car(cdr(expr)));
            // call the car function on this part of the evaluation
            returnSExp = car(evaluationSExp);
            return returnSExp;
        }
            // Cdr will return a list with everything but the first element of the input
        else if (strcmp(symbol, "cdr") == 0) {
            // car(cdr(expr)) is the remaining elements of the list, so recursively call eval with the rest of the list
            evaluationSExp = eval(car(cdr(expr)));
            // call the cdr function on this part of the evaluation
            returnSExp = cdr(evaluationSExp);
            return returnSExp;
        }
            // Quote does not recursively call the eval function again because everything after the quote at it's level is not to be evaluated
            // Just call the quote function and return the list created
        else if (strcmp(symbol, "quote") == 0) {
            returnSExp = quote(expr);
            return returnSExp;
        }
            // cons will combine the two lists that follow the cons command
        else if (strcmp(symbol, "cons") == 0) {
            // create two lists. One for the left input list and one for the right input list
            SExp returnSExpLeft = create_list();
            SExp returnSExpRight = create_list();
            // recursively evaluate the rest of the first list
            returnSExpLeft = eval(car(cdr(expr)));
            // recursively evaluate the rest of the second list
            returnSExpRight = eval(car(cdr(cdr(expr))));
            // cons the two lists that have been recursively evaluated by calling the cons function
            returnSExp = cons(returnSExpLeft, returnSExpRight);
            return returnSExp;
        }
            // will return a single conscell list with the symbol set to either #t or #f
        else if (strcmp(symbol, "symbol?") == 0) {
            // recursively evaluate the rest of the list
            evaluationSExp = eval(car(cdr(expr)));
            // call the Symbol function with the evaluated list
            returnSExp = Symbol(evaluationSExp);
            return returnSExp;
        }
            // will return a single conscell list with the symbol set to either #t or #f
        else if (strcmp(symbol, "null?") == 0) {
            // recursively evaluate the rest of the list
            evaluationSExp = eval(car(cdr(expr)));
            // call the null function with the evaluated list
            returnSExp = null(evaluationSExp);
            return returnSExp;
        }
        else if (strcmp(symbol, "append") == 0){
            // create two lists. One for the left input list and one for the right input list
            SExp returnSExpLeft = create_list();
            SExp returnSExpRight = create_list();
            // recursively evaluate the rest of the first list
            returnSExpLeft = eval(car(cdr(expr)));
            // recursively evaluate the rest of the second list
            returnSExpRight = eval(car(cdr(cdr(expr))));
            // append the two lists that have been recursively evaluated by calling the append function
            returnSExp = append(returnSExpLeft, returnSExpRight);
            return returnSExp;
        }
        else if (strcmp(symbol, "equal?") == 0){
            // create two lists. One for the left input list and one for the right input list
            SExp returnSExpLeft = create_list();
            SExp returnSExpRight = create_list();
            // recursively evaluate the rest of the first list
            returnSExpLeft = eval(car(cdr(expr)));
            // recursively evaluate the rest of the second list
            returnSExpRight = eval(car(cdr(cdr(expr))));
            // Check if the two lists are equal
            returnSExp = equal(returnSExpLeft, returnSExpRight);
            return returnSExp;
        }
        else if (strcmp(symbol, "assoc") == 0){
            // create two lists. One for the left input list and one for the right input list
            SExp symb = create_list();
            SExp assocList = create_list();
            // recursively evaluate the rest of the first list
            symb = eval(car(cdr(expr)));
            // recursively evaluate the rest of the second list
            assocList = eval(car(cdr(cdr(expr))));
            // append the two lists that have been recursively evaluated by calling the append function
            returnSExp = assoc(symb, assocList);
            return returnSExp;
        }
    }
    // If the car is NULL or is not a symbol, just return the input expression
    return expr;
}
