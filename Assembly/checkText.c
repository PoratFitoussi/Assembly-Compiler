#include "checkText.h"
/* an array to easily compare the first word to the command names */
char *directives[6] = {"db\0", "dh\0", "dw\0", "asciz\0", "entry\0", "extern\0"};

char *orders[27] = {"add\0", "sub\0", "and\0", "or\0", "nor\0", "move\0", "mvhi\0", "mvlo\0", "addi\0", "subi\0", "andi\0", "ori\0", "nori\0", "bne\0", "beq\0", "blt\0",
                    "bgt\0", "lb\0", "sb\0", "lw\0", "sw\0", "lh\0", "sh\0", "jmp\0", "la\0", "call\0", "stop\0"};

int isSymbol(char *command, int from)
{
    int iterator = 1;
    if (isalpha(command[from])) /* the first character need to be letter */
    {
        while (isalpha(command[from + iterator]) || isdigit(command[from + iterator]))
        {
            if (iterator > symbolMax)
            {
                return -1;
            }
            iterator++;
        }
        if (command[from + iterator] == ':') /* the last charcter need to be colon */
            return from + iterator;
    }
    return -1;
}

int isDirective(char *command, int from)
{
    int iterator = 1;  /* the  */
    char directive[7]; /* we put here the directive word */
    int i;
    if (command[from] == '.')
    {
        while ((command[from + iterator] != ' ') && (command[from + iterator] != '\t') && (command[from + iterator] != '\0'))
        {
            if (iterator >= 7)
                return -1;
            directive[iterator - 1] = command[from + iterator];
            iterator++;
        }
        directive[iterator - 1] = '\0';
        for (i = 0; i < 6; i++) /* a loop that run and check if the line is the same as each directive word */
        {
            if (strcmp(directive, directives[i]) == 0)
            {
                return i + 1;
            }
        }
        return -1;
    }
    return -1;
}

int isNumber(char *command, int from)
{
    int start = from; /* the value of the index */
    int end = 0;
    /* if there is '-' or '+' we will skip it */
    if (start + end < commanSize && (command[start + end] == '-' || command[start + end] == '+'))
        start++;
    /* a loop that run until the end of the line or if the next character is'nt a number */
    while (command[start + end] != '\0' && command[start + end] >= '0' && command[start + end] <= '9')
    {
        end++;
    }
    /* if end is zero than we didn't move in the loop and there is'nt a number so it's mistakes */
    return (end == 0) ? -1 : start + end;
}

int isOrder(char *command, int from)
{
    int iterator = 0;
    char order[6]; /*the length of the longest word*/
    int i;
    /* a loop that run through the first word and put it in string */
    while (iterator < 5 && command[from + iterator] != '\0' && !isspace(command[from + iterator]))
    {
        order[iterator] = command[from + iterator];
        iterator++;
    }
    order[iterator] = '\0';
    for (i = 0; i < 27; i++) /* try to comper the first word to the order word */
    {
        if (strcmp(order, orders[i]) == 0)
        {
            return i; /* return the index of the word in the array,that's hpw we can know the word */
        }
    }
    return -1;
}

int count_dig(int a)
{
    int count = 0;

    /* Run loop till num is greater than 0 */
    do
    {
        /* Increment digit count */
        count++;

        /* Remove last digit of 'num' */
        a /= 10;
    } while (a != 0);

    return count;
}

