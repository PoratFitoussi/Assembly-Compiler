
#include <math.h>
#include <ctype.h>
#include "checkText.h"
#include "convertText.h"
#include "input.h"

#define dxSize 3     /* the length of .db, .dh and .dw */
#define ascizSize 6  /* the length of asciz */
#define externSize 7 /* the length of extern*/
#define entrySize 6  /* the length of entry */

typedef struct directiveList *dptr; /* pointer to directive list */
typedef struct directiveList
{
    unsigned int A1 : 4; /* the first characer in the outpur text, used for all directive*/
    unsigned int A2 : 4; /* the second characer in the outpur text,used for all directive*/
    unsigned int B1 : 4; /* the third characer in the outpur text, used for dw and dh only*/
    unsigned int B2 : 4; /* the fourth characer in the outpur text, used for dw and dh only*/
    unsigned int C1 : 4; /* the fifth characer in the outpur text,used for dw only*/
    unsigned int C2 : 4; /* the sixth characer in the outpur text, used for dw only*/
    unsigned int D1 : 4; /* the seventh characer in the outpur text,used for dw only*/
    unsigned int D2 : 4; /* the eighth characer in the outpur text,used for dw only*/
    int DC;              /* the address for the command */
    short type;          /* a variable to know which command enter, 1 for db and asciz, 2 for dh, 3 for dw  */
    dptr next;           /* a pointer to the next struct */
} directiveList;

/* A function that put the db,dh and dw commands in the directive struct */
int addDx(char *, int, int *, dptr *, int);
/* A function that check if the number is legal and send it to a function that add it to the struct */
int addNumber(char *c, int, int, int *, dptr *, int);
/* A function that add the data of directive command to the struct */
int addNode(dptr *, int, int *, int);
/* A function that send the line to put inside the directive struct */
int addAsciz(char *, int, int *, dptr *);
/* A function that check the if the line is legal and during that send the data to put in the struct */
int addAscizToStruct(char *command, int from, dptr *head, int *DC);
/*A function that tell us the number of line in the output file */
int numLine(dptr *, int);
/* A function that release the data type */
void releaseData(dptr *head);