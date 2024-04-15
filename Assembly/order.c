#include "input.h"
#include "order.h"

/* Macros for common errors: */
#define MISSING_DOLLAR(ch) \
    if (ch != '$')         \
        return -1;

#define MISSING_NUMMBER(ch) \
    if (!(isdigit(ch)))     \
        return -2;

#define RANGE_VALUE(ch)                \
    if (atoi(ch) < 0 || 31 < atoi(ch)) \
        return -3;

#define MISSING_COMMA(ch) \
    if (ch != ',')        \
        return -4;

#define EXTRANEOUS_TEXT(ch) \
    if (ch)                 \
        return -5;

int checkAndAddR1(char *command, int from, int *IC, optr *head, short type)
{
    int firstRegister, secondRegister, thirdRegister, added;
    if (type != 3) /* meaning not or commands */
        from += 3;
    else
        from += 2;
    command += from;                     /* moving the commands to start after the commnds */
    removeSpaces(command);               /* removings all the white characters */
    MISSING_DOLLAR(command[0]);          /* dollar suppoeses to be the first character*/
    command += 1;                        /* we don't need the dollar character int the string */
    MISSING_NUMMBER(command[0]);         /* need to be a number after a dollar*/
    RANGE_VALUE(command);                /* if the number is bigger or smaller than that the register does not exists*/
    firstRegister = atoi(command);       /*the number to enetr the rs filed rs*/
    command += count_dig(firstRegister); /* no need for the number*/
    MISSING_COMMA(command[0]);           /*supposed to be a comma*/
    command += 1;                        /* no need for the comma */
    MISSING_DOLLAR(command[0]);          /* dollar suppoeses to be the first character*/
    command += 1;                        /* we don't need the dollar character int the string */
    MISSING_NUMMBER(command[0]);         /*sopposed to be a number*/
    RANGE_VALUE(command);                /* if the number is bigger or smaller than that the register does not exists*/
    secondRegister = atoi(command);      /*the number to enetr the rsfiled rt*/
    command += count_dig(atoi(command)); /* no need for the number*/
    MISSING_COMMA(command[0]);           /*supposed to be a comma*/
    command += 1;
    MISSING_DOLLAR(command[0]);          /* supposed to be a dollar character */
    command += 1;                        /* no need for the comma */
    MISSING_NUMMBER(command[0]);         /*sopposed to be a number*/
    RANGE_VALUE(command);                /* if the number is bigger or smaller than that the register does not exists*/
    thirdRegister = atoi(command);       /*the number to enetr the rsfiled rd*/
    command += count_dig(atoi(command)); /* no need for the number*/
    EXTRANEOUS_TEXT(command[0]);         /* in case there is more character in the line that's a mistake*/
    if ((added = addR1(head, firstRegister, secondRegister, thirdRegister, IC, type)) < 0)
        return -7;

    return 1;
}

int checkAndAddR2(char *command, int from, int *IC, optr *head, short type)
{
    int firstRegister, secondRegister, added;
    from += 4;                           /* the length of the orders */
    command += from;                     /* moving the commands to start after the commnds */
    removeSpaces(command);               /* removings all the white characters */
    MISSING_DOLLAR(command[0]);          /* dollar suppoeses to be the first character*/
    command += 1;                        /* we don't need the dollar character int the string */
    MISSING_NUMMBER(command[0]);         /* need to be a number after a dollar*/
    RANGE_VALUE(command);                /* if the number is bigger or smaller than that the register does not exists*/
    firstRegister = atoi(command);       /*the number to enetr the rs filed rs*/
    command += count_dig(firstRegister); /* no need for the number*/
    MISSING_COMMA(command[0]);           /*supposed to be a comma*/
    command += 1;                        /* no need for the comma */
    MISSING_DOLLAR(command[0]);          /* dollar suppoeses to be the first character*/
    command += 1;                        /* we don't need the dollar character int the string */
    MISSING_NUMMBER(command[0]);         /*sopposed to be a number*/
    RANGE_VALUE(command);                /* if the number is bigger or smaller than that the register does not exists*/
    secondRegister = atoi(command);      /*the number to enetr the rsfiled rt*/
    command += count_dig(atoi(command)); /* no need for the number*/
    EXTRANEOUS_TEXT(command[0]);         /* in case there is more character in the line that's a mistake*/

    if ((added = addR2(head, firstRegister, secondRegister, IC, type) < 0))
        return -7;
    return 1;
}

int checkAndAddI1(char *command, int from, int *IC, optr *head, short type)
{
    char plus, minus; /* tell if there is a symbol of '+' or '-' */
    int firstRegister, secondRegister, thirdRegister, added;
    if (type < 11 || type == 12) /* meaning 'addi','subi','andi','nori' or 'nori' */
        from += 4;
    else if (type == 11) /* meaning ori commands only */
        from += 3;
    else /* meaning 'lb','sb','lw','sw','lh','sh' */
        from += 2;
    command += from;       /* moving the commands to start after the commnds */
    removeSpaces(command); /* removings all the white characters */

    MISSING_DOLLAR(command[0]);                 /* need to be a dollar symbol */
    command += 1;                               /* we don't need the dollar character any more */
    MISSING_NUMMBER(command[0]);                /* need to be a number here */
    RANGE_VALUE(command);                       /* the number need to be in the rage of 0 to 31 */
    firstRegister = atoi(command);              /*the number to enetr the rs filed rs*/
    command += count_dig(atoi(command));        /* no need for the number*/
    MISSING_COMMA(command[0]);                  /*supposed to be a comma*/
    command += 1;                               /* no need for the comma*/
    if (command[0] == '+' || command[0] == '-') /* in case there is '+' or '-' character before the number*/
    {
        if (command[0] == '+')
            plus = '+';
        if (command[0] == '-')
            minus = '-';
        command += 1;
    }
    MISSING_NUMMBER(command[0]); /* need to be a number here */
    if (plus)
        secondRegister = +atoi(command);
    else if (minus)
        secondRegister = -atoi(command);
    else
        secondRegister = atoi(command);
    if (secondRegister > 32767 || secondRegister < -32768) /* the number need to be in this limit */
        return -3;
    command += count_dig(atoi(command));
    MISSING_COMMA(command[0]); /*supposed to be a comma*/
    command += 1;
    MISSING_DOLLAR(command[0]);          /* need to be a dollar symbol */
    command += 1;                        /* we don't need the dollar character any more */
    MISSING_NUMMBER(command[0]);         /* need to be a number here */
    RANGE_VALUE(command);                /* the number need to be in the rage of 0 to 31 */
    thirdRegister = atoi(command);       /*the number to enetr the rs filed rs*/
    command += count_dig(atoi(command)); /* no need for the number*/
    EXTRANEOUS_TEXT(command[0]);         /* in case there is more character in the line that's a mistake*/
    if ((added = addI1(head, firstRegister, secondRegister, thirdRegister, IC, type)) < 0)
        return -7;
    return 1;
}

int checkAndAddI2(char *command, int from, int *IC, optr *head, short type)
{
    int firstRegister, secondRegister, added;
    char *labelI = (char *)calloc(30, sizeof(char)); /* the last operand*/
    int index = 1;
    from += 3; /* the length of the order commands */

    command += from;            /* moving the commands to start after the commnds */
    removeSpaces(command);      /* removings all the white characters */
    MISSING_DOLLAR(command[0]); /* supposed to be a dollar character */
    command += 1;               /*we dont need the character any more*/
    MISSING_NUMMBER(command[0]);
    RANGE_VALUE(command); /* the number need to be in the rage of 0 to 31 */
    firstRegister = atoi(command);
    command += count_dig(atoi(command)); /*we dont need the character any more*/
    MISSING_COMMA(command[0]);           /*supposed to be a comma*/
    command += 1;
    MISSING_DOLLAR(command[0]); /* supposed to be a dollar character */
    command += 1;               /*we dont need the character any more*/

    MISSING_NUMMBER(command[0]);
    RANGE_VALUE(command); /* the number need to be in the rage of 0 to 31 */
    secondRegister = atoi(command);
    command += count_dig(atoi(command));
    MISSING_COMMA(command[0]);  /*supposed to be a comma*/
    command += 1;               /*we dont need the character any more*/
    if (!(isalpha(command[0]))) /* is the first character in a label is'nt a letter tha it's illegal*/
        return -6;

    if (31 < strlen(command))
        return -6;

    labelI[0] = command[0];

    while (command[index] != '\0')
    {
        if (!(isalpha(command[index])) && !isdigit(command[index])) /* the symbol can have only letter and digit */
            return -6;

        labelI[index] = command[index];
        index++;
    }

    index = isOrder(labelI, 0);
    if (index != -1) /* checking if thhe label have the same name as a command word*/
        return -6;

    if ((added = addI2(head, firstRegister, labelI, secondRegister, IC, type)) < 0)
        return -1;

    return 1;
}

int checkAndAddJ(char *command, int from, int *IC, optr *head, short type)
{
    int firstRegister, added, index = 1;
    char *labelJ = (char *)calloc(31, sizeof(char)); /*the string to tenter to the addressx*/

    if (type == 23) /* meaning jmp order */
        from += 3;
    else if (type == 24) /* meaning la order */
        from += 2;
    else
        from += 4; /* meaning call order */

    command += from;       /* moving the commands to start after the commnds */
    removeSpaces(command); /* removings all the white characters */

    if (command[0] == '$') /* the first option for opernad*/
    {
        command += 1; /*we dont need the character any more*/

        MISSING_NUMMBER(command[0]);
        RANGE_VALUE(command); /* the number need to be in the rage of 0 to 31 */
        firstRegister = atoi(command);
        command += count_dig(firstRegister); /*we dont need the character any more*/
        EXTRANEOUS_TEXT(command[0]);         /* in case there is more character in the line that's a mistake*/

        if ((added = addJ(head, firstRegister, labelJ, IC, type)) < 0)
            return -7;
        else
            return 1;
    }
    else if (isalpha(command[0])) /*the second option for operand*/
    {
        if (31 < strlen(command))
            return -6;

        labelJ[0] = command[0];
        while (command[index] != '\0')
        {
            if (!(isalpha(command[index])) && !(isdigit(command[index])))
                return -6;

            labelJ[index] = command[index];
            index++;
        }
        labelJ[index] = '\0'; /* the end of the symbol */
        index = -1;
        if ((added = addJ(head, index, labelJ, IC, type)) < 0)
            return -6;
        return 1;
    }
    else
        return -1; /* in case the first character is'nt '$' or a leter that's a mistake*/
}

int checkAndAddStop(char *command, int from, int *IC, optr *head, short type)
{
    int added;             /* to check if we been able to put the order is the struct */
    from += 4;             /* the length of the stop order */
    command += from;       /* the line remove to after the name of the order */
    removeSpaces(command); /* removings all the white characters */

    if (command[0] == '\0') /* the line need to be empty now */
        EXTRANEOUS_TEXT(command[0]);

    if ((added = addStop(head, IC)) < 0)
        return -6;

    return 1;
}

void removeSpaces(char *str)
{
    int i;
    /* To keep track of non-space character count */
    int count = 0;

    /* Traverse the given string. If current character 
     is not space, then place it at index 'count++' */
    for (i = 0; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
            str[count++] = str[i]; /* here count is incremented */
    }

    str[count] = '\0'; /* finish the string */
}