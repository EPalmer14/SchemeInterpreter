//
// Created by Zeke Palmer on 4/21/22.
//

#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "parser.h"

// car returns the first element of the input list
SExp car(SExp firstSExp){
    // create the list to return
    SExp car = create_list();
    // Set the list head to the car of the input list if the car of the input list is not null
    if(firstSExp->head->car != NULL){
        car->head = firstSExp->head->car;
        return car;
    }
    // If the car of the input list is null, return NULL
    else{
        return NULL;
    }
}

// cdr returns the rest of the input list
SExp cdr(SExp firstSExp){
    // create the list to return
    SExp cdr = create_list();
    // Set the list head to the cdr of the input list if the cdr of the input list is not null
    if(firstSExp->head->cdr != NULL){
        cdr->head = firstSExp->head->cdr;
        return cdr;
    }
    // If the cdr of the input list is null, return NULL
    else{
        return NULL;
    }
}

// Quote returns the input list without evaluating it
SExp quote(SExp firstSExp){
    // The list to return
    SExp quote;
    // Set it equal to the rest of the input list (without the cell containing the symbol quote
    quote = car(cdr(firstSExp));
    // Return the new list
    return quote;
}


// Cons takes two lists and builds one list which is the first list and the second list combined
SExp cons(SExp firstSExp, SExp secondSExp){
    // Create the list to return
    SExp cons = create_list();
    // Create the first conscell for the list
    cons->head = create_conscell();
    // If the first list is empty check if the second is empty
    if(isEmptyList(firstSExp->head)){
        // If the second list is also empty, return the new list as an empty list
        if(isEmptyList(secondSExp->head)){
            return cons;
        }
        // If just the first list is empty just have car of the list to return point to the start of the second list
        cons->head->car = secondSExp->head;
    }
    // If the first list is not empty check to see if the second list is empty
    else if(isEmptyList(secondSExp->head)){
        // If the second list is empty then just have the car of the list to return point to the start of the first list
        cons->head->car = firstSExp->head;
    }
    // If neither list is empty set the car and cdr of the new list to the first and second list respectively
    else{
        cons->head->car = firstSExp->head;
        cons->head->cdr = secondSExp->head;
    }
    // Return the new list
    return cons;
}


// Symbol returns a list with the head symbol either #t or #f depending on if the input list is a single symbol cell or not
SExp Symbol(SExp firstSExp){

    // Create the list to return
    SExp symbol = create_list();
    // Creat the conscell for the list
    symbol->head = create_conscell();
    // If the input list is a symbol cell
    if (isSymbol(firstSExp->head)){
        strcpy(symbol->head->symbol, "#t");
    }
    // Otherwise the list is not a single symbol cell
    else{
        strcpy(symbol->head->symbol, "#f");
    }

    // Return the new list
    return symbol;
}


// Null returns a list with the head symbol either #t or #f depending on if the input list is null or not
SExp null(SExp firstSExp){

    // Create the list to return
    SExp null = create_list();
    // Create the conscell for the list
    null->head = create_conscell();
    // If input list is empty then it is null
    if(isEmptyList(firstSExp->head)){
        strcpy(null->head->symbol, "#t");
    }
    // Otherwise the list not null
    else{
        strcpy(null->head->symbol, "#f");
    }
    // Return the new list
    return null;
}



// Append takes two arguments and returns a list that is the first list with the second list appended to its end
SExp append(SExp firstSExp, SExp secondSExp){
    // Create two lists for the car and cdr of the first list that we can then cons with the second list
    SExp Car = create_list();
    SExp Cdr = create_list();
    // If the first list is empty return the second
    if(isEmptyList(firstSExp->head)){
        return secondSExp;
    }
    // If the second list is empty return the first
    else if(isEmptyList(secondSExp->head)){
        return firstSExp;
    }
    // If neither are empty begin consing lists together
    else {
        // If there are no more pieces to the first list the cdr will equal NULL. As long as it does not equal NULL continue with this block
        if(cdr(firstSExp) != NULL){
            // Get the car of the first list and the cdr of the first list
            Car = car(firstSExp);
            Cdr = cdr(firstSExp);
            // Recursive call. Cons the car of the first list with a recursive call to append with the rest of the first list and the second list
            return cons(Car, append(Cdr, secondSExp));
        }
        // The cdr of the first list is NULL, thus we have reached the last element of the first list
        else {
            // Get the last element of the first list
            Car = car(firstSExp);
            // cons it with the second list
            return cons(Car, secondSExp);
        }
    }
}

SExp equal(SExp firstSExp, SExp secondSExp){
    // Create four lists for the car a cdr of both lists
    // Since we need to traverse both input lists recursively we need both the car and cdr of both lists
    SExp CarL1 = create_list();
    SExp CdrL1 = create_list();
    SExp CarL2 = create_list();
    SExp CdrL2 = create_list();
    // Fifth list is a single conscell containing the symbol #t or #f depending on the evaluation of the two lists
    SExp Equal = create_list();
    Equal->head = create_conscell();
    // Final list is used to compare the symbols of two symbol cells in the same place of a conscell structure
    SExp symbolChecker = create_list();

    // Base cases
    // If the first list and second list are both empty, then they are equal
    if(isEmptyList(firstSExp->head)){
        // then they are equal
        if(isEmptyList(secondSExp->head)){
            strcpy(Equal->head->symbol, "#t");
            return Equal;
        }
            // If the first list is empty but the second list is not, then they are not equal
        else{
            strcpy(Equal->head->symbol, "#f");
            return Equal;
        }
    }
        // If the first list is not empty but the second is, then they are not equal
    else if(isEmptyList(secondSExp->head)){
        strcpy(Equal->head->symbol, "#f");
        return Equal;
    }
    // If the current conscell of the first list is a symbol and the current conscell of the second list is also
    // a symbol, then compare the symbols
    else if(isSymbol(firstSExp->head)){
        if(isSymbol(secondSExp->head)){
            if(strcmp(firstSExp->head->symbol, secondSExp->head->symbol) != 0){
                strcpy(Equal->head->symbol, "#f");
                return Equal;
            }
            else {
                strcpy(Equal->head->symbol, "#t");
                return Equal;
            }
        }
        // If the first list is currently a symbol cell but the second is not than they are not equal
        else{
            strcpy(Equal->head->symbol, "#f");
            return Equal;
        }
    }
        // If neither list is empty and they are not symbol cells
    else {
        // If neither of the lists' cdr is NULL, then recursively evaluate both the car and cdr of both lists
        if(cdr(firstSExp) != NULL){
            if(cdr(secondSExp) != NULL){
                CdrL1 = cdr(firstSExp);
                CdrL2 = cdr(secondSExp);
                CarL1 = car(firstSExp);
                CarL2 = car(secondSExp);

                // Check the cars of both lists, if it returns true, then check the cdrs as well
                symbolChecker = equal(CarL1, CarL2);
                if(strcmp(symbolChecker->head->symbol, "#f") == 0){
                    strcpy(Equal->head->symbol, "#f");
                    return Equal;
                }
                else{
                    return equal(CdrL1, CdrL2);
                }

            }
            // If just the first lists cdr is NULL then they are not equal
            else{
                strcpy(Equal->head->symbol, "#f");
                return Equal;
            }
        }
        // If just the second lists cdr is NULL then they are not equal
        else if(cdr(secondSExp) != NULL){
            strcpy(Equal->head->symbol, "#f");
            return Equal;
        }
        // If both lists cdr are NULL, recursively check their cars
        else{
            CarL1 = car(firstSExp);
            CarL2 = car(secondSExp);
            return equal(CarL1, CarL2);
        }
    }
}

SExp assoc(SExp symb, SExp assocList){
    // Create an SExp to return and set it's symbol equal to #f. It will only need to be false because
    // assoc does not return #t
    SExp assocReturn = create_list();
    assocReturn->head = create_conscell();
    strcpy(assocReturn->head->symbol, "#f");
    // Create a list to check if equal to a symbol
    SExp check = create_list();
    // Check if the symbol and the first element of the current pair of the association list
    check = equal(symb, car(car(assocList)));
    // If equal returns true, return the current pair
    if(strcmp(check->head->symbol, "#t") == 0){
        return car(assocList);
    }
    // Otherwise check if there are more pairs and if so recursively call assoc with the rest of the pairs
    else{
        if(cdr(assocList) != NULL){
            assocReturn = assoc(symb, cdr(assocList));
        } else{
            return assocReturn;
        }
    }
    return assocReturn;
}
