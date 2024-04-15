#include <stdio.h>

#ifndef commanSize
#define commanSize 81 /* the length of the size*/
#endif

/* A function that read the line from a file and return a stirng */
char *getLinefromFile(FILE *);
/* A function that read a string from a file and return character */
char getNext(FILE *);
/* A function that put a character in array */
void setNext(char c);
/* A function that check if there it's the end of the text */
int isTextLeft(char *command, int from);
/* A function that skip on space from a charater and retrn the index of the next character */
int jumpSpace(char *command, int from);
/* A function that return the number of character there is in a line */
int endOfText(char *command, int from);
/* A function that skip on comma from a string */
int jumpBreak(char *command, int from);