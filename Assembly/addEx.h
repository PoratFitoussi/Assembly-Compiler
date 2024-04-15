#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "input.h"

typedef struct entryList *entryptr; /* pointer to entry or extern information */
typedef struct entryList
{
    char *name;    /* the name of the symbol */
    int address;   /* the address of the symbol */
    entryptr next; /* a pointer to the next struct */
} entryList;

typedef struct externList *externptr;
typedef struct externList
{
    char *name;     /* the name of the symbol */
    int address;    /* the address of the symbol */
    externptr next; /* a pointer to the next struct */
} externList;

/* A function that check if the operand of the entry is in the symbol list */
int checkEntry(char *command, int index, int end, entryptr *head,sptr SYMBOL_HEAD);
/* A function that set the information to the struct of entry and extern */
int setEntery(char *symbolName, int valute, entryptr *head);
/* A function that release the entry list*/
void releaseEntry(entryptr *head);
/* A function that release the extern list*/
void releaseExtern(externptr *head);