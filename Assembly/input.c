#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "input.h"

char buffer[100];
int indexFile = -1;

char *getLinefromFile(FILE *fp1) /*Input: pointer to file. Output: char[81] that in the end of comman: /0. Discounts: file is open for reading*/
{
    char c;
    char *arr;
    int i;
    if ((c = getNext(fp1)) != EOF)
        setNext(c);
    else
        return NULL;
    arr = (char *)malloc(commanSize * sizeof(char));
    if (arr)
    {
        i = 0;
        /*a loop that put in the array in charactet from the file until */
        while ((c = getNext(fp1)) != EOF && c != '\n' && i < (commanSize - 1))
        {
            arr[i] = c;
            i++;
        }
        arr[i] = '\0';
        if (i <= (commanSize - 1) && c != EOF && c != '\n')
        {
            setNext(c);
            while ((c = getNext(fp1)) != EOF && c != '\n')
                ;
            if (c == EOF)
                setNext(c);
            return "`Syntax error, too much long command.\0";
        }
        else
            return arr;
    }
    else
    {
        return "`Dynamic allocatio error, can not create space.\0";
    }
}

char getNext(FILE *fp1)
{
    if (indexFile > -1) /* if the index is less the -1 than */
    {
        return buffer[indexFile--];
    }
    return getc(fp1);
}

void setNext(char c) /*Input: next char.*/
{
    buffer[++indexFile] = c;
}

int isTextLeft(char *command, int from)
{
    from = jumpSpace(command, from);
    return (command[from] != '\0') ? 1 : 0;
}

int jumpSpace(char *command, int from)
{
    char c;
    while ((c = command[(from)]) != '\0' && isspace(c))
        from++;
    return from;
}

int endOfText(char *command, int from)
{
    char c;
    while ((c = command[(from)]) != '\0' && !isspace(c) && (c != ','))
        from++;
    return from;
}

int jumpBreak(char *command, int from) 
{
    from = jumpSpace(command, from);
    if (command[from] == ',') /* in case there is a comman we skip on it*/
    {
        from++;
        return from;
    }
    else
    {
        return -1;
    }
}
