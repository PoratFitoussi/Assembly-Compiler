#include <stdio.h>
#include <stdlib.h>
#include "data.h"

int addDx(char *command, int from, int *DC, dptr *head, int type)
{
    int iterator = from + dxSize; /* the sum of the index after the directive commands */
    int endNum;
    int number;
    iterator = jumpSpace(command, iterator); /* skiping the whit characters */
    if (!isTextLeft(command, iterator))      /* if there is'nt text left than we missing the numbers */
        return -1;
    if ((endNum = isNumber(command, iterator)) < 0) /* we make sure to knwo how many character there is in the number, if there is'nt a number it will reutrn -1 */
        return endNum;
    number = addNumber(command, iterator, endNum - 1, DC, head, type); /* send the data to put in the struct */
    if (number < 0)                                                    /* check if the data put in the struct */
        return number;
    iterator = endNum; /* it will be the index after the directive */

    /* a loop until the end of the line that put the numbers in the struct */
    while (isTextLeft(command, iterator))
    {
        iterator = jumpSpace(command, iterator); /* skip on the space */
        if (command[iterator] != ',')            /* must be a comma after a number */
            return iterator;
        iterator = jumpSpace(command, iterator + 1);    /* we can skip on the comma */
        if ((endNum = isNumber(command, iterator)) < 0) /* we make sure to knwo how many character there is in the number, if there is'nt a number it will reutrn -1 */
            return endNum;
        number = addNumber(command, iterator, endNum - 1, DC, head, type); /* send the data to put in the struct */
        if (number < 0)                                                    /* check if the data put in the struct */
            return number;
        iterator = endNum; /* put the index after the number */
    }
    return iterator;
}

int addNumber(char *command, int iterator, int endNum, int *DC, dptr *head, int type)
{
    /* send the data to a function and in return we get the number from the line */
    int newNumber = textToNum(command, iterator, endNum);
    int added;
    if (type == 1) /* if the directive is db */
    {
        if (newNumber > 512 || newNumber < -512) /* the number need to be in this limit */
            return -1;
    }
    if (type == 2) /* if the directive is dh */
    {
        if (newNumber > 32767 || newNumber < -32768) /* the number need to be in this limit */
            return -1;
    }
    /* send the number to enter to the struct */
    if ((added = addNode(head, newNumber, DC, type) < 0))
        return added;
    return 1;
}

int addAsciz(char *command, int from, int *DC, dptr *head)
{
    int iterator = from + ascizSize;         /* put the index after the name of the directive asciz */
    iterator = jumpSpace(command, iterator); /* skip the space after the directive order from the line */
    return addAscizToStruct(command, iterator, head, DC);
}

int addAscizToStruct(char *command, int from, dptr *head, int *DC)
{
    int iterator = from; /* it will be the index */
    int added;
    if (command[iterator] != '\"') /* if the first character is'nt quotation marks that it's an error */
        return -1;
    iterator++; /* can skip on the quotation marks*/

    /* a loop that that run until the quotation marks or to the end of the line */
    while (command[iterator] != '\"' && isTextLeft(command, iterator))
    {
        /* send the data to be add to the struct */
        if ((added = addNode(head, command[iterator], DC, 1)) < 0) /* the same type as db */
            return added;
        iterator++;
    }
    if (command[iterator] != '\"') /* the last character most be quotation marks */
        return -1;
    if ((added = addNode(head, '\0', DC, 1)) < 0) /* add the zero the the struct */
        return added;
    return iterator + 1;
}

int addNode(dptr *head, int number, int *DC, int type)
{
    int index;
    dptr newData = (dptr)calloc(1, sizeof(directiveList)); /* making a new place in memory */
    dptr temp;
    if (newData == NULL) /* it's mean there is no free place in memory */
        return -2;

    newData->DC = *DC; /* put the address in the list */
    newData->next = NULL;
    if (type == 1) /* in case the directive is db */
    {
        newData->type = 1;
        for (index = 0; index < 8; index++) /* it's need to enter to the 8 bits filed */
        {
            if (0 <= index && index <= 3)
            {
                if (number & 1 << index)
                    newData->A1 += 1 << index;
            }
            if (4 <= index && index <= 7)
            {
                if (number & 1 << index)
                    newData->A2 += 1 << (index - 4);
            }
        }
        *DC += 1; /* the address rise by 1 */
    }

    if (type == 2) /* in case the directive is dh */
    {
        newData->type = 2;                   /* mark it's dh */
        for (index = 0; index < 16; index++) /* it's need to enter to the 16 bits filed */
        {
            if (0 <= index && index <= 3)
            {
                if (number & 1 << index)
                    newData->A1 += 1 << index;
            }
            if (4 <= index && index <= 7)
            {
                if (number & 1 << index)
                    newData->A2 += 1 << (index - 4);
            }
            if (8 <= index && index <= 11)
            {
                if (number & 1 << index)
                    newData->B1 += 1 << (index - 8);
            }
            if (12 <= index && index <= 15)
            {
                if (number & 1 << index)
                    newData->B2 += 1 << (index - 12);
            }
        }
        *DC += 2; /* the address rise by 2 */
    }
    if (type == 3) /* in case the directive is dw */
    {
        newData->type = 3;

        for (index = 0; index < 32; index++) /* it's need to enter to the 32 bits filed */
        {
            if (0 <= index && index <= 3)
            {
                if (number & 1 << index)
                    newData->A1 += 1 << index;
            }
            if (4 <= index && index <= 7)
            {
                if (number & 1 << index)
                    newData->A2 += 1 << (index - 4);
            }
            if (8 <= index && index <= 11)
            {
                if (number & 1 << index)
                    newData->B1 += 1 << (index - 8);
            }
            if (12 <= index && index <= 15)
            {
                if (number & 1 << index)
                    newData->B2 += 1 << (index - 12);
            }
            if (16 <= index && index <= 19)
            {
                if (number & 1 << index)
                    newData->C1 += 1 << (index - 16);
            }
            if (20 <= index && index <= 23)
            {
                if (number & 1 << index)
                    newData->C2 += 1 << (index - 20);
            }
            if (24 <= index && index <= 27)
            {
                if (number & 1 << index)
                    newData->D1 += 1 << (index - 24);
            }
            if (28 <= index && index <= 31)
            {
                if (number & 1 << index)
                    newData->D2 += 1 << (index - 28);
            }
        }
        *DC += 4; /* the address rise by 4 */
    }

    if (*head == NULL) /* in case this is the first line that enter */
    {
        *head = newData;
        return 1;
    }
    temp = *head;

    while (temp->next != NULL) /* move the struct to the end */
    {
        temp = temp->next;
    }
    temp->next = newData;
    return 1;
}

/*A function that tell us the number of line in the output file */
int numLine(dptr *head, int address)
{
    int place = 0, value = 0;
    dptr data = *head;

    while (data) /*count how many line there is in data for output*/
    {
        if (data->type == 1)
        {
            if (place == 0)
            {
                value++;
                place = 2;
                data = data->next;
                continue;
            }
            if (place == 1)
            {
                place = 3;
                data = data->next;
                continue;
            }
            if (place == 2)
            {
                place = 4;
                data = data->next;
                continue;
            }
            if (place == 3)
            {
                place = 5;
                data = data->next;
                continue;
            }
            if (place == 4)
            {
                place = 6;
                data = data->next;
                continue;
            }
            if (place == 5)
            {
                place = 7;
                data = data->next;
                continue;
            }
            if (place == 6)
            {
                place = 0;
                data = data->next;
                continue;
            }
            if (place == 7)
            {
                place = 1;
                value++;
                data = data->next;
                continue;
            }
        }
        if (data->type == 2)
        {
            if (place == 0)
            {
                place = 4;
                value++;
                data = data->next;
                continue;
            }
            if (place == 1)
            {
                place = 5;
                data = data->next;
                continue;
            }
            if (place == 2)
            {
                place = 6;
                data = data->next;
                continue;
            }
            if (place == 3)
            {
                place = 7;
                data = data->next;
                continue;
            }
            if (place == 4)
            {
                place = 0;
                data = data->next;
                continue;
            }
            if (place == 5)
            {
                place = 1;
                value++;
                data = data->next;
                continue;
            }
            if (place == 6)
            {
                place = 2;
                value++;
                data = data->next;
                continue;
            }
            if (place == 7)
            {
                place = 3;
                value++;
                data = data->next;
                continue;
            }
        }
        if (data->type == 3)
        {
            if (place == 0)
            {
                data = data->next;
                value++;
                continue;
            }
            if (place == 1)
            {
                data = data->next;
                value++;
                continue;
            }
            if (place == 2)
            {
                data = data->next;
                value++;
                continue;
            }
            if (place == 3)
            {
                data = data->next;
                value++;
                continue;
            }
            if (place == 4)
            {
                data = data->next;
                value++;
                continue;
            }
            if (place == 5)
            {
                data = data->next;
                value++;
                continue;
            }
            if (place == 6)
            {
                data = data->next;
                value++;
                continue;
            }
            if (place == 7)
            {
                data = data->next;
                value++;
                continue;
            }
        }
    }
    return ((address - 100) / 4 + value - 1);
}

void releaseData(dptr *head) /* realse the data struct*/
{
    dptr pt, temp;
    temp = *head;
    while (temp)
    {
        pt = temp->next;
        free(temp);
        temp = pt;
    }
    *head = NULL;
}