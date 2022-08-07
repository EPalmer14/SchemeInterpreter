//
// Created by Zeke Palmer on 2/24/22.
//

/****************************************************************
 File: lexer.c
 -------------
 This file implements the interface given in lexer.h.
****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"

/****************************************************************
 Data members
 ------------
 lexeme:    "String" variable that contains the token.
 c:         The current character in the input stream from the keyboard.
 lookahead: Set to 1 iff the previous call to getToken() required
            looking ahead.
****************************************************************/
static char *lexeme;
static char c;
static int lookahead;

/****************************************************************
 Function: newToken()
 --------------------
 Private function that de-allocates the existing lexeme and
 allocates a new one (a maxLength-character array). This is called
 when startTokens() is called. The variable lexeme remains
 as a dynamically allocated array. If a new token stream is
 desired, the lexeme array is re-allocated.
****************************************************************/
static void newToken (int maxLength)
{
    if (lexeme != NULL)
        free(lexeme);

    if ((lexeme = (char *) calloc(maxLength, sizeof(char))) == NULL)
    {
        printf("Out of memory, too many tokens.\n");
        exit(0);
    }
}//nextToken

/****************************************************************
 startTokens(): See header file for documentation.
****************************************************************/
void startTokens (int maxLength)
{
    lookahead = 0;
    lexeme = NULL;
    newToken(maxLength);
}//startTokens

/****************************************************************
 getToken() implementation notes: The function works by getting
 the first character, in case the previous call required lookahead,
 then skipping over whitespace. The main part is the "if" statement
 that handles 4 cases:
     (1) Current character is ")" or "'" (single quote).
         Then return the character as a string.
     (2) Current character is "(".
         Then scan for ")". If found, return "()".
         Otherwise, return "(".
     (3) Current character is "#". Only accepted following characters
         are t and f, in which case "#t" or "#f" are returned.
     (4) Default case: Scan for a string of characters, and return as
         a string (a string cannot contain any whitespace character, or
         "(" or ")").
****************************************************************/
char *getToken ()
{
    if (!lookahead)                   //get first char
        c = getchar();

    while ((c == ' ') || (c == '\n') || (c == '\t')) //skip white space
        c = getchar();

    if ((c == ')') || (c == '\'')) {  //Case (1): right paren or quote
        lexeme[0] = c;
        lexeme[1] = '\0';
        lookahead = 0;
    }
    else if (c == '(') {              //Case (2): left paren or ()
        lookahead = 1;
        c = getchar();
        while ((c == ' ') || (c == '\n') || (c == '\t'))
            c = getchar();
        if (c == ')') {
            strcpy(lexeme, "()");          //empty list token
            lookahead = 0;
        }
        else
            strcpy(lexeme, "(");
    }
    else if (c == '#') {              //Case (3): #t or #f
        lookahead = 0;
        c = getchar();
        if ((c != 't') && (c != 'f')) {
            printf("Illegal symbol %c after #.\n", c);
            exit(1);
        }
        // We need to look ahead here to make sure the next char is whitespace or
        // a paren   #tt is not legal
        lookahead = 1;
        char next = getchar();
        if (!((next == ' ') || (next == '\n') || (next == '\t') ||
              (next == '(') || (next == ')'))) {
            printf("Illegal symbol %c after #%c.\n", next, c);
            exit(1);
        }
        if (c == 't')
            strcpy(lexeme, "#t");
        else
            strcpy(lexeme, "#f");
        c = next;
    }
    else {                            //Case (4): scan for symbol
        int i = 0;
        lookahead = 1;
        while ((c != '(') && (c != ')') && (c != ' ') && (c != '\n') && (c != '\t')) {
            lexeme[i++] = c;
            c = getchar();
        }/* while */
        lexeme[i] = '\0';
    }

    return lexeme;
} //getToken