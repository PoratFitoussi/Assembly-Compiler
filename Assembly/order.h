#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "checkText.h"
#include "convertText.h"
#include "input.h"

typedef struct orderList *optr; /* pointer to order list */
typedef struct orderList
{
    unsigned int RandJ : 32; /* filed for R and J command */
    unsigned int I : 16;     /* filed for I commands */
    int immed : 16;          /* filed for the data that come from I commands */
    char *symbolNameSave;    /* in the first step we can't fill all the filed so we keep the symbol here to be filed in the second step*/
    int IC;                  /* the address for the list */
    short type;              /* that's how we can know what kind of command enter, 1 for R, 2 for I and 3 for J */
    optr next;               /* a pointer to the next list */
} order;

int checkAndAddR1(char *, int, int *, optr *, short);

int checkAndAddR2(char *, int, int *, optr *, short);

int checkAndAddI1(char *, int, int *, optr *, short);

int checkAndAddI2(char *, int, int *, optr *, short);

int checkAndAddJ(char *, int, int *, optr *, short);

int checkAndAddStop(char *, int, int *, optr *, short);

int addR1(optr *head, int rs, int rt, int rd, int *IC, int funct);

int addR2(optr *head, int rs, int rd, int *IC, int funct);

int addI1(optr *, int, int, int, int *, int);

int addI2(optr *head, int rs, char *immed, int rt, int *IC, int opcode);

int addJ(optr *, int, char *, int *IC, int);

int addStop(optr *head, int *IC);

/* A skip on the whit character in the line */
void removeSpaces(char *str);

/* A function that release the order list*/
void releaseORDER(optr *head);