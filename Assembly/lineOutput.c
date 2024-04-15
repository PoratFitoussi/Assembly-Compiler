#include <string.h>
#include "order.h"
#include "symbol.h"
#include "addEx.h"
#include "lineOutput.h"

extern int ICF; /* the last address of the order */
int putFullList(sptr symbol, optr order, fptr *head, externptr *externHead)
{
    int check;
    while (order)
    {
        if (order->type == 1) /* in case the order is all R commands, jmp command with register or stop command */
        {
            if ((check = setFullListOrder1(order->IC, order->RandJ, head)) < 0) /* in case we didn't able to make place in memory */
                return -1;
        }
        if (order->type == 2) /* for I type with register*/
        {
            if ((check = setFullListOrder2(order->IC, order->I, order->immed, head)) < 0)
                return -1;
        }
        if (order->type == 3) /* for I type with a symbol */
        {
            if ((check = findMissingInfo(order->IC, order->symbolNameSave, symbol)) == 0)
                return -2;
            if ((check = setFullListOrder2(order->IC, order->I, check, head)) < 0)
                return -1;
        }
        if (order->type == 4) /* for J type with a symbol */
        {
            if ((check = getAddress(order->symbolNameSave, symbol, order->IC, externHead)) < -1)
                return -3;
            else
                order->RandJ += check;
            if ((check = setFullListOrder1(order->IC, order->RandJ, head)) < 0) /* in case we didn't able to make place in memory */
                return -1;
        }
        order = order->next;
    }
    return 1;
}

int setFullListOrder1(int address, int value, fptr *head)
{
    int index;
    fptr newData = (fptr)calloc(1,sizeof(fullList));
    fptr temp;
    newData->next = NULL;

    if (newData == NULL) /* in case we didn't able to make place */
        return -1;

    for (index = 0; index < 32; index++)
    {
        if (0 <= index && index <= 3)
        {
            if (value & 1 << index)
                newData->A1 += 1 << index;
        }

        if (4 <= index && index <= 7)
        {
            if (value & 1 << index)
                newData->A2 += 1 << (index - 4);
        }
        if (8 <= index && index <= 11)
        {
            if (value & 1 << index)
                newData->B1 += 1 << (index - 8);
        }
        if (12 <= index && index <= 15)
        {
            if (value & 1 << index)
                newData->B2 += 1 << (index - 12);
        }
        if (16 <= index && index <= 19)
        {
            if (value & 1 << index)
                newData->C1 += 1 << (index - 16);
        }
        if (20 <= index && index <= 23)
        {
            if (value & 1 << index)
                newData->C2 += 1 << (index - 20);
        }
        if (24 <= index && index <= 27)
        {
            if (value & 1 << index)
                newData->D1 += 1 << (index - 24);
        }
        if (28 <= index && index <= 31)
        {
            if (value & 1 << index)
                newData->D2 += 1 << (index - 28);
        }
    }
    newData->address = address;

    if (*head == NULL)
    {
        *head = newData;
        return 1;
    }
    temp = *head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newData;
    return 1;
}

int setFullListOrder2(int address, int value, int valueTwo, fptr *head)
{
    int index;
    fptr newData = (fptr)calloc(1,sizeof(fullList));
    fptr temp;
    newData->next = NULL;

    if (newData == NULL) /* in case we didn't able to make place */
        return -1;

    for (index = 0; index < 16; index++)
    {
        if (0 <= index && index <= 3)
        {
            if (value & 1 << index)
                newData->C1 += 1 << index;
        }

        if (4 <= index && index <= 7)
        {
            if (value & 1 << index)
                newData->C2 += 1 << (index - 4);
        }
        if (8 <= index && index <= 11)
        {
            if (value & 1 << index)
                newData->D1 += 1 << (index - 8);
        }
        if (12 <= index && index <= 15)
        {
            if (value & 1 << index)
                newData->D2 += 1 << (index - 12);
        }
    }
    for (index = 0; index < 16; index++)
    {
        if (0 <= index && index <= 3)
        {
            if (valueTwo & 1 << index)
                newData->A1 += 1 << index;
        }

        if (4 <= index && index <= 7)
        {
            if (valueTwo & 1 << index)
                newData->A2 += 1 << (index - 4);
        }
        if (8 <= index && index <= 11)
        {
            if (valueTwo & 1 << index)
                newData->B1 += 1 << (index - 8);
        }
        if (12 <= index && index <= 15)
        {
            if (valueTwo & 1 << index)
                newData->B2 += 1 << (index - 12);
        }
    }
    newData->address = address;

    if (*head == NULL)
    {
        *head = newData;
        return 1;
    }
    temp = *head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newData;

    return 1;
}

int findMissingInfo(int address, char *symbol, sptr head)
{

    int value;
    while (head)
    {
        if (!strcmp(head->name, symbol))
        {
            if (head->type == 2) /* can't be extern type */
                return -1;
            else if (head->type == 1)
                value = head->address + ICF; /* the address of the code type */
            else
                value = head->address;
        }
        head = head->next;
    }
    value -= address;
    return value;
}

int getAddress(char *symbol, sptr head, int address, externptr *externHead)
{
    int value;
    while (head)
    {
        if (!strcmp(head->name, symbol))
        {
            if (head->type == 2) /* extern type */
            {
                externptr newData = (externptr)calloc(1,sizeof(externList)); /* save the address of the extern */
                externptr temp;
                newData->next = NULL;
                newData->address = address; /* the address of where the extern is */
                newData->name = (char *)malloc((30 + 1) * sizeof(char));
                newData->name = symbol; /* save the name of the symbol */

                if (*externHead == NULL) /* in case the list is empty */
                {
                    *externHead = newData;
                }
                else
                {
                    temp = *externHead;
                    while (temp->next != NULL)
                        temp = temp->next;
                    temp->next = newData;
                }

                return 1;
            }
            else if (head->type == 1)
            {
                value = head->address + ICF;

                return value; /* the address of the code type */
            }
            else if (head->type == 0)
            {
                value = head->address;
                return value;
            }
        }
        head = head->next;
    }
    return -1; /* missing a symbol with the same name*/
}

void releaseFullList(fptr *head)
{
    fptr pt, temp;
    temp = *head;

    while (temp)
    {
        pt = temp->next;
        free(temp);
        temp = pt;
    }
    
    *head = NULL;
}