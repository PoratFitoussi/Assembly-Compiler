#include "order.h"
#include "checkText.h"

int addR1(optr *head, int rs, int rt, int rd, int *IC, int funct)
{
    optr newData = (optr)calloc(1, sizeof(order));
    optr temp;

    if (newData == NULL) /* it's mean we didn't able to make a room in the memory */
        return -8;
    /* put the information in the struct */
    newData->RandJ += (rs << 21);
    newData->RandJ += (rt << 16);
    newData->RandJ += (rd << 11);
    newData->RandJ += ((funct + 1) << 6);
    newData->next = NULL;
    newData->IC = *IC; /* the address */
    *IC += 4;          /* for the next use */
    newData->type = 1; /* R commands */
 
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

int addR2(optr *head, int rs, int rd, int *IC, int funct)
{
    optr newData = (optr)calloc(1, sizeof(order));
    optr temp;

    if (newData == NULL) /* it's mean we didn't able to make a room in the memory */
        return -1;
    /* put the information in the struct */
    newData->RandJ += (1 << 26);          /* the opcode for all the commands here*/
    newData->RandJ += (rs << 21);         /* the rs filed*/
    newData->RandJ += (rd << 11);         /*the rd field*/
    newData->RandJ += ((funct - 4) << 6); /* the fucnt of every command will be order -4*/
    newData->next = NULL;
    newData->IC = *IC; /* the address */
    *IC += 4;          /* for the next use */
    newData->type = 1; /* R commands */

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

int addI1(optr *head, int rs, int immed, int rt, int *IC, int opcode)
{
    optr newData = (optr)calloc(1, sizeof(order));
    optr temp;

    if (newData == NULL) /* it's mean we didn't able to make a room in the memory */
        return -1;
    /* put the information in the struct */
    newData->I += ((opcode + 2) << 10); /* the opcode of the command*/
    newData->I += (rs << 5);            /*  the rs filed*/
    newData->I += rt;                   /* the rt filed*/
    newData->immed = immed;             /* the immed filed*/
    newData->IC = *IC;                  /* the address */
    *IC += 4;                           /* for the next use */
    newData->type = 2;                  /* I commands */
    newData->next = NULL;

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

int addI2(optr *head, int rs, char *immed, int rt, int *IC, int opcode)
{
    optr newData = (optr)calloc(1, sizeof(order));
    optr temp;

    if (newData == NULL) /* it's mean we didn't able to make a room in the memory */
        return -1;
    /* put the information in the struct */
    newData->I += ((opcode + 2) << 10); /* the opcode of the command*/
    newData->I += (rs << 5);            /*  the rs filed*/
    newData->I += rt;                   /* the rt filed*/
    newData->symbolNameSave = immed;    /* the immed filed*/
    newData->IC = *IC;                  /* the address */
    *IC += 4;                           /* for the next use */
    newData->type = 3;                  /* I commands */
    newData->next = NULL;

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

int addJ(optr *head, int addressNum, char *addressStr, int *IC, int opcode)
{
    optr newData = (optr)calloc(1, sizeof(order));
    optr temp;

    if (newData == NULL) /* it's mean we didn't able to make a room in the memory */
        return -1;
    /* put the information in the struct */

    if (addressNum != -1) /*in case the x variable didn't send enpty,*/
    {
        newData->RandJ += ((opcode + 7) << 26); /* the opcode is the number of order plus 7*/
        newData->RandJ += (1 << 25);            /* the reg filed */
        newData->RandJ += addressNum;           /*the number that send*/
        newData->IC = *IC;                      /* the address */
        *IC += 4;                               /* for the next use */
        newData->type = 1;                      /* the same as R commands */
        newData->next = NULL;

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
    else /* it's mean a symbol have been send*/
    {
        newData->RandJ += ((opcode + 7) << 26); /* the opcode is the number of order plus 7*/
        newData->symbolNameSave = addressStr;   /* we still can't know if the label is a local or not*/
        newData->IC = *IC;                      /* the address */
        *IC += 4;                               /* for the next use */
        newData->type = 4;                      /* J without jump with register and stop  like I2 type*/
        newData->next = NULL;

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
    return 1; /* never get here */
}

int addStop(optr *head, int *IC)
{
    optr newData = (optr)calloc(1, sizeof(order));
    optr temp;

    if (newData == NULL) /* it's mean we didn't able to make a room in the memory */
        return -1;
    /* put the information in the struct */

    newData->RandJ += (63 << 26); /* the opcode of the stop command is 63 and every other filed is zero*/
    newData->IC = *IC;            /* the address */
    *IC += 4;                     /* for the next use */
    newData->type = 1;            /* stop order */
    newData->next = NULL;

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

void releaseORDER(optr *head) /* release the order sturct*/
{
    optr pt, temp;
    temp = *head;
    while (temp)
    {
        pt = temp->next;
        free(temp);
        temp = pt;
    }
    *head = NULL;
}