#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "symbol.h"
#include "checkText.h"

int checkExist(sptr head, char *name)
{
    while (head) /* a loop that run until the end of the list  */
    {
        if (strcmp(head->name, name) == 0) /* check if the symbol already on the list */
            return 1;
        head = head->next;
    }
    return 0;
}

int SetSymbol(sptr *head, char *command, int from, int length, int adress, short type)
{
    sptr pt;
    if (*head)
    {
        pt = *head;
        while (pt->next)
        {
            pt = pt->next;
        }
        pt->next = (sptr)malloc(sizeof(symbolList));
        pt = pt->next;
    }
    else
    {
        *head = (sptr)malloc(sizeof(symbolList));
        pt = *head;
    }
    if (pt && (pt->name = (char *)malloc((length + 1) * sizeof(char))))
    {
        pt->next = NULL;
        pt->name = strncpy(pt->name, command + from, length);
        pt->name[length] = '\0';
        pt->address = adress;
        pt->type = type;
    }
    else
    {
        return -2;
    }
    return 1;
}

int addSymbols(sptr *head, char *command, int from)
{
    int start, end, count = 0;
    char *externSymbol; /* the name of the symbol from extern */
    sptr checkSymbol = *head;
    while (isTextLeft(command, from) && from >= 0)
    {
        start = jumpSpace(command, from);
        end = endOfText(command, start);
        if (end > -1)
        {
            externSymbol = (char *)malloc((end - start + 1) * sizeof(char)); /* making a place for the name of the operator from extern */
            strncpy(externSymbol, command + start, end - start);             /* define the name of the symbol */
            if (checkExist(checkSymbol, externSymbol))                       /* the same symbol can't appear twice*/
                return -4;

            if (SetSymbol(head, command, start, end - start, 0, 2) < 0)
                return -2;

            count++;
            from = jumpBreak(command, end);
            if ((from == -1 && isTextLeft(command, end)) || (from != -1 && !isTextLeft(command, from)))
                return -3;
        }
        else
        {
            return -1;
        }
    }

    return count;
}

void releaseSymbol(sptr *head)
{
    sptr pt, next;
    if (!isSYEmpty(*head))
    {
        pt = *head;
        while (pt)
        {
            next = pt->next;
            free(pt->name);
            free(pt);
            pt = next;
        }
        *head = NULL;
    }
}

int isSYEmpty(sptr head)
{
    return (head) ? 0 : 1;
}