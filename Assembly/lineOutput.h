#include <stdio.h>
#include <stdlib.h>

typedef struct fullList *fptr; /* a list that point to the finished struct that will be the output */
typedef struct fullList
{
    unsigned int A1 : 4; /* the first characer in the outpur text*/
    unsigned int A2 : 4; /* the second characer in the outpur text*/
    unsigned int B1 : 4; /* the third characer in the outpur text*/
    unsigned int B2 : 4; /* the fourth characer in the outpur text*/
    unsigned int C1 : 4; /* the fifth characer in the outpur text*/
    unsigned int C2 : 4; /* the sixth characer in the outpur text*/
    unsigned int D1 : 4; /* the seventh characer in the outpur text*/
    unsigned int D2 : 4; /* the eighth characer in the outpur text*/
    int address;         /* the address of the struct */
    fptr next;           /* a pointer to the next struct */

} fullList;

/* A function that put all the information that we have into a struct that will be the output to the file */
int putFullList(sptr symbol, optr order, fptr *head, externptr *);
/* A function that put the first type of order commands into the list */
int setFullListOrder1(int address, int value, fptr *);
/* A function that put the second type of order commands into the list */
int setFullListOrder2(int address, int value, int valueTwo, fptr *);
/* A function that find the remainder of the address */
int findMissingInfo(int address, char *symbol, sptr);
/* A function that get the address of a symbpl */
int getAddress(char *symbol, sptr head, int, externptr *);
/* A function that release the full list  */
void releaseFullList(fptr *head);