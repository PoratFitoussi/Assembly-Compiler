#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "input.h"

#define symbolMax 30 /* the max length that can be for a symbol */

/* A function that check if the word from the line is an directive command,and return the index from the word */
int isDirective(char *command, int from);
/* A function that check if a word is a symbol */
int isSymbol(char *command, int from);
/* A function that check if there is a number, if there is it will return the number of charcater in the number
if there is'nt it will return -1 for mistake */
int isNumber(char *command, int from);
/* A function that make a from a text a number */
int textToNum(char *text, int from, int end);
/* A function that check if the word from the line is an order command,and return the index from the word */
int isOrder(char *command, int from);
/* A function that return the number of digit in a number */
int count_dig(int a);
