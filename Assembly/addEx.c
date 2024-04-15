#include <string.h>
#include "symbol.h"
#include "addEx.h"

extern int ICF; /* the last address of the order */

int checkEntry(char *command, int from, int end, entryptr *head, sptr SYMBOL_HEAD)
{
    int newIndex = 1, added, value = -1;                /* the index of the new symbol string */
    char *newSymbol = (char *)calloc(31, sizeof(char)); /* here we gonna keep the symbol from entry */

    if (!(isalpha(command[from])))
        return -1; /* the first character in the symbol need to be a letter */
    else
        newSymbol[0] = command[from]; /* put the first character in the place we gonna keep the symbol from entry */
    from++;                           /* skip the first character */

    while (command[from] != '\0') /* a loop until the end of the string */
    {
        if (!(isdigit(command[from])) && !(isalpha(command[from]))) /* the character in the symbol can be digit or letter */
            return -1;
        else
            newSymbol[newIndex] = command[from]; /* making the new symbol */

        newIndex++;
        from++;
    }
    newSymbol[newIndex] = '\0'; /* the end of the string */

    if (strlen(newSymbol) > 30) /* the symbol can't be longer than 30 */
        return -1;

    while (SYMBOL_HEAD)
    {
        if (!strcmp(SYMBOL_HEAD->name, newSymbol))
        {
            if (SYMBOL_HEAD->type == 2)      /* meaning that it's data or code */
                return -1;                   /* entry and extern can't have the same operand */
            else if (SYMBOL_HEAD->type == 1) /* meaning a data type */
                value = SYMBOL_HEAD->address + ICF;
            else
                value = SYMBOL_HEAD->address; /* geting the avddress of the symbol */
        }
        SYMBOL_HEAD = SYMBOL_HEAD->next;
    }
    if ((added = setEntery(newSymbol, value, head)) < 0)
        return -1;

    return 1;
}

int setEntery(char *symbolName, int value, entryptr *head)
{
    entryptr newData = (entryptr)calloc(1, sizeof(entryList));
    entryptr temp1, temp2;

    if (newData == NULL) /* it's mean we didn't abel to make a romm in the memory */
        return -1;

    newData->name = (char *)calloc((30 + 1), sizeof(char));
    newData->name = symbolName;
    newData->address = value;
    newData->next = NULL;

    if (*head == NULL)
    {
        *head = newData;
        return 1;
    }
    temp1 = *head;
    while ((temp1) && (temp1->address < value))
    {
        temp2 = temp1;
        temp1 = temp1->next;
    }
    if (temp1 == *head)
    {
        newData->next = temp1;
        *head = newData;
        return 1;
    }
    newData->next = temp1;
    temp2->next = newData;
    return 1;
}

void releaseEntry(entryptr *head) /* release the entry struct*/
{
    entryptr pt, temp;
    temp = *head;

    while (temp)
    {
        pt = temp->next;
        free(temp);
        temp = pt;
    }
    *head = NULL;
}

void releaseExtern(externptr *head) /* release the extern struct */
{
    externptr pt, temp;
    temp = *head;

    while (temp)
    {
        pt = temp->next;
        free(temp);
        temp = pt;
    }
    *head = NULL;
}