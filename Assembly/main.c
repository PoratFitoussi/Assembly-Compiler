#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "symbol.h"
#include "data.h"
#include "checkText.h"
#include "convertText.h"
#include "order.h"
#include "addEx.h"
#include "lineOutput.h"
#include "creatFile.h"

#define SymbolMax 30
#define ErrorJump   \
    flagError = ON; \
    free(command);  \
    continue;

sptr SYMBOL_HEAD = NULL;      /* we gonna use this to point to the symbol struct */
dptr HEAD_DATA = NULL;        /* the head of the list for directive commands */
optr ORDER_HEAD = NULL;       /* the head of the list for order commands */
entryptr ENTRY_HEAD = NULL;   /* the head of the list for entry commands */
externptr EXTERN_HEAD = NULL; /* the head of the list for extern commands */
fptr FULL_LIST = NULL;        /* the head of the list finished list for all the rirective and order commands */

int ICF; /* the address of the order */

/*every where there is Discounts: (1), it mean char[81] with 0 in the end of command */
enum Status
{
    OFF,
    ON
};

short flagError;
char syName[commanSize];

/*the steps of the program*/
void firstStep(FILE *);
void secondStep(FILE *);
void releaseLists();

int main(int argc, char const *argv[])
{
    int i;                     /*index for the file name*/
    char *fullFileName = NULL; /* will be the name of the file*/
    FILE *file;                /* pointer to the file*/
    if (argc <= 1)             /* in case there is'nt file*/
    {
        printf("No File where typed.\n");
        exit(0);
    }
    for (i = 1; i < argc; i++) /* a loop in case there is more than one file*/
    {
        fullFileName = NULL;
        fullFileName = concat(argv[i], ".as\0"); /* put the name of the file in the variabel*/
        if (fullFileName)                        /*in case the is a file*/
        {

            printf(" -- File: %s -- \n", fullFileName);
            file = fopen(fullFileName, "r"); /* open the file*/
            if (file)
            {
                firstStep(file);
                if (flagError == ON) /* in case we discavord an error*/
                {
                    flagError =OFF;
                    printf(" -- File: %s couldn't be created becuse there are error to fix in it. --\n", fullFileName);
                    fclose(file);
                    free(fullFileName);
                    releaseLists();
                    continue;
                }
                rewind(file);     /* read again the file for the second step of the prog*/
                secondStep(file); /* move to the second step*/
                if (flagError == ON)
                {
                    flagError =OFF;
                    printf(" -- File: %s couldn't be created becuse there are error to fix in it. --\n", fullFileName);
                    fclose(file);
                    free(fullFileName);
                    releaseLists();
                    continue;
                }
                if (printFile(argv[i], HEAD_DATA, ENTRY_HEAD, EXTERN_HEAD, FULL_LIST) != 1)
                {
                    printf("Error: could not creat output files to: %s\n", fullFileName);
                }
                else
                {
                    printf(" -- Successfully created all output files for %s -- \n", fullFileName);
                }
                fclose(file);
            }
            else
            {
                printf("Error: The file %s.as dose not exists.\n", argv[i]);
            }
            free(fullFileName);
            releaseLists();
            ICF = 0;
        }
        else
        {
            printf("Dynamic alocation error: couldn't creat file: %s\n", argv[i]);
        }
    }
    return 1;
}

void firstStep(FILE *file)
{
    short flagSymbol = OFF; /*if there is a Symbol the variabel will be ON*/
    int index;              /*the index of every line */
    int endSymbol;          /* in case there is a label that variabel will be the place of the index where the symbol end*/
    int INTresult;          /* that variabel will be the place of the order or directive command in array we make */
    int INTresult2;         /* we gonna use that variabel to know if we succeeded to add the order or directive to the list*/
    int SymbolStart;        /* in case there is a symbol in the line that variabel tell where it start in the line */
    int SymbolLength;       /* the symbol length in case there is */
    int lineNumber = 0;     /* represented the number of the line we in */
    int IC = 100;           /* the address of the order in the line */
    int DC = 0;             /* the address of the directive in the line */
    char *command = NULL;   /* represented the line from the file */

    /*a loop that will run until the last line of the file*/
    while ((command = getLinefromFile(file)))
    {
        index = 0;                                                                /*the index start from zero*/
        flagSymbol = OFF;                                                         /*the symbol start from zero*/
        lineNumber++;                                                             /* when we inside the loop we know that another line starated*/
        if (!isTextLeft(command, index) || command[jumpSpace(command, 0)] == ';') /* check if it's an empty or note line */
            continue;

        index = jumpSpace(command, index);    /* we make the indext to be the place after the space */
        endSymbol = isSymbol(command, index); /* check if there is a symbol */

        if (endSymbol > 0) /* it's mean there is a symbol */
        {
            flagSymbol = ON;
            SymbolStart = index;              /* save the place the symbol */
            SymbolLength = endSymbol - index; /* calculating the length of the symbol */
            if (SymbolLength >= SymbolMax)    /* the length of the symbol can't be bigger than 30*/
            {
                printf("Error: in (line: %d) Syntax error, Symbol name can't be longer than: %d.\n", lineNumber, SymbolMax);
                ErrorJump
            }
            index = jumpSpace(command, endSymbol + 1); /* skip to the place after the space from the symbol */
            if (!isTextLeft(command, index))
            {
                printf("Error: in (line: %d) Syntax error, expected a method name or directive after symbol.\n", lineNumber);
                ErrorJump
            }
        }
        if (command[index] == '.') /* in case we deal with diractive */
        {
            INTresult = isDirective(command, index); /* check if the word is one of the directive word */
            if (INTresult <= 0)
            {
                printf("Error: in (line: %d) Syntax error, unknow directive name.\n", lineNumber);
                ErrorJump
            }
            if (INTresult <= 4) /* in case it's db,dh,dw or asciz directive */
            {
                if (flagSymbol) /* in case there is symbol */
                {
                    flagSymbol = OFF;
                    strncpy(syName, (command + SymbolStart), SymbolLength); /* save the name of the symbol */
                    syName[SymbolLength] = '\0';
                    if (checkExist(SYMBOL_HEAD, syName)) /* the same symbol can't appear twice*/
                    {
                        printf("Error: in (line: %d) Symbol already exist, and have been defined befor.\n", lineNumber);
                        ErrorJump
                    }
                    INTresult2 = SetSymbol(&SYMBOL_HEAD, command, SymbolStart, SymbolLength, DC, 1); /* send the symbol to enter to the list */
                    if (INTresult2 == -2)                                                            /* it's mean we dd'nt succeed to put the symbol in the list*/
                    {
                        printf("Error: in (line: %d) Dynamic alocation error, could not add Symbol.\n", lineNumber);
                        ErrorJump
                    }
                }
                switch (INTresult)
                {
                case 1:
                    INTresult2 = addDx(command, index, &DC, &HEAD_DATA, 1);
                    if (INTresult2 < 0 && INTresult2 != -2)
                    {
                        printf("Error: in (line: %d) Syntax error, after directive name. (expected: number,number..) note: all number must be between: -512:512 .\n", lineNumber);
                        ErrorJump
                    }
                    break;
                case 2:
                    INTresult2 = addDx(command, index, &DC, &HEAD_DATA, 2);
                    if (INTresult2 < 0 && INTresult2 != -2)
                    {
                        printf("Error: in (line: %d) Syntax error, after directive name. (expected: number,number..) note: all number must be between: -32768:32767 .\n", lineNumber);
                        ErrorJump
                    }
                    break;
                case 3:
                    INTresult2 = addDx(command, index, &DC, &HEAD_DATA, 3);
                    if (INTresult2 < 0 && INTresult2 != -2)
                    {
                        printf("Error: in (line: %d,) Syntax error, after directive name. (expected: number,number..) note: all number must be between: -2,147,483,648: 2,147,483,647 .\n", lineNumber);
                        ErrorJump
                    }
                    break;
                case 4:
                    INTresult2 = addAsciz(command, index, &DC, &HEAD_DATA);
                    if (INTresult2 < 0 && INTresult2 != -2)
                    {
                        printf("Error: in (line: %d) Syntax error, after directive name. (expected: \"sum_text\").\n", lineNumber);
                        ErrorJump
                    }
                    break;
                }
                if (INTresult2 == -2)
                {
                    printf("Error: in (line: %d) Dynamic alocation error, could not add data.\n", lineNumber);
                    ErrorJump
                }
                if (isTextLeft(command, INTresult2))
                {
                    printf("Error: in (line: %d) Syntax error, unexpected text after directive order.\n", lineNumber);
                    ErrorJump
                }
            }
            else /* in case the other dircetive was entry or extern */
            {
                if (flagSymbol)
                {
                    flagSymbol = OFF;
                    printf("Warning in (line: %d) unexpected Symbol in this type of directive.\n", lineNumber);
                }
                if (INTresult == 6) /* in case the directive is extern */
                {
                    index += externSize; /* skip on the directive word */
                    if (isTextLeft(command, index))
                    {
                        index = jumpSpace(command, index);                     /* skip on the space */
                        INTresult2 = addSymbols(&SYMBOL_HEAD, command, index); /* send the symbol to be add under extern */
                        if (INTresult2 < 0)
                        {
                            switch (INTresult2)
                            {
                            case -2:
                                printf("Error: in (line: %d) Dynamic alocation error, could not add Symbol.\n", lineNumber);
                                ErrorJump break;
                            case -3:
                                printf("Error: in (line: %d) Syntax error, unexpected text after adding symbols.\n", lineNumber);
                                ErrorJump break;
                            case -4:
                                printf("Error: in (line: %d) Symbol from extern can't apear twice or in eny other type \n", lineNumber);
                                ErrorJump break;
                            default:
                                printf("Error: in (line: %d) Syntax error, expected symbol name.\n", lineNumber);
                                ErrorJump
                            }
                        }
                    }
                    else
                    {
                        printf("Error: in (line: %d) expected symbol name afted directive: extern.\n", lineNumber);
                        ErrorJump
                    }
                }
            }
            free(command);
            continue;
        }
        /* from here it's an order or mistake */
        if (flagSymbol)
        {
            flagSymbol = OFF;
            strncpy(syName, (command + SymbolStart), SymbolLength);
            syName[SymbolLength] = '\0';
            if (checkExist(SYMBOL_HEAD, syName))
            {
                printf("Error: in (line: %d) Syntax error, Symbol already exist, and have been defined befor.\n", lineNumber);
                ErrorJump
            }
            /* send the symbol to be add to the list */
            INTresult2 = SetSymbol(&SYMBOL_HEAD, command, SymbolStart, SymbolLength, IC, 0);
            if (INTresult2 == -2)
            {
                printf("Error: in (line: %d) Dynamic alocation error, could not add Symbol.\n", lineNumber);
                ErrorJump
            }
        }
        INTresult = isOrder(command, index); /* if we got here and the first word is'nt an order than it's mistakes*/
        if (INTresult < 0)
        {
            printf("Error: in (line: %d) Syntax error, unknow order name.\n", lineNumber);
            ErrorJump
        }
        if (0 <= INTresult && INTresult <= 4) /* the first half of the R commands */
        {
            INTresult2 = checkAndAddR1(command, index, &IC, &ORDER_HEAD, INTresult);
        }
        if (5 <= INTresult && INTresult <= 7)
        {
            INTresult2 = checkAndAddR2(command, index, &IC, &ORDER_HEAD, INTresult);
        }
        if ((8 <= INTresult && INTresult <= 12) || (17 <= INTresult && INTresult <= 22)) /*thouse kind of command have the same structure*/
        {
            INTresult2 = checkAndAddI1(command, index, &IC, &ORDER_HEAD, INTresult);
        }
        if (13 <= INTresult && INTresult <= 16)
        {
            INTresult2 = checkAndAddI2(command, index, &IC, &ORDER_HEAD, INTresult);
        }
        if (23 <= INTresult && INTresult <= 25) /* for the jmp,la and call commands */
        {
            INTresult2 = checkAndAddJ(command, index, &IC, &ORDER_HEAD, INTresult);
        }
        if (INTresult == 26)
        {
            INTresult2 = checkAndAddStop(command, index, &IC, &ORDER_HEAD, INTresult);
        }
        switch (INTresult2)
        {
        case -1: /* missing dollar */
        {
            printf("Error: in (line: %d) Syntax error, missing a dollar.\n", lineNumber);
            ErrorJump
        }
        case -2: /* missing number*/
        {
            printf("Error: in (line: %d) Syntax error, missing number.\n", lineNumber);
            ErrorJump
        }
        case -3:
        {
            printf("Error: in (line: %d) Syntax error, number is out of range.\n", lineNumber);
            ErrorJump
        }
        case -4:
        {
            printf("Error: in (line: %d) Syntax error, missing a comma.\n", lineNumber);
            ErrorJump
        }
        case -5:
        {
            printf("Error: in (line: %d) Syntax error, there is text after when the order needed to end.\n", lineNumber);
            ErrorJump
        }
        case -6:
        {
            printf("Error: in (line: %d) Syntax error, the symbol didn't defined good.\n", lineNumber);
            ErrorJump
        }
        case -7:
        {
            printf("Error: in (line: %d) Memory error,cannot allocate memory;.\n", lineNumber);
            ErrorJump
        }
        }
    }
    free(command);
    ICF = IC;
}

void secondStep(FILE *file)
{
    int index, end, INTresult, INTresult2, lineNumber = 0;
    char *command = NULL;
    flagError = OFF;
    while ((command = getLinefromFile(file)))
    {
        index = 0;    /* the start of the line */
        lineNumber++; /* new line */

        if (!isTextLeft(command, index) || command[jumpSpace(command, 0)] == ';') /* in case that's a command or empty line */
            continue;
        if (command[index] == '`')
        {
            printf("Error: in (line: %d) %s\n", lineNumber, (command + index + 1));
            flagError = ON;
            continue;
        }

        index = jumpSpace(command, index); /* skip on the whit characters */
        end = isSymbol(command, index);
        if (end > 0)
            index = jumpSpace(command, end + 1);
        if (command[index] == '.')
        {
            INTresult = isDirective(command, index);
            if (INTresult == 5)
            {
                index += entrySize; /* moving the index after the name of the directive */
                if (isTextLeft(command, index))
                {
                    index = jumpSpace(command, index);
                    end = endOfText(command, index); /* the place of the end of the line */
                    if ((end - index) > SymbolMax)   /* the length of the symbol in entry can't be longer than 30 */
                    {
                        printf("Error: in (line: %d) Syntax error, symbol name is too long. Can't pass: %d chars.\n", lineNumber, SymbolMax);
                        ErrorJump
                    }
                    INTresult2 = checkEntry(command, index, end, &ENTRY_HEAD, SYMBOL_HEAD);
                    if (INTresult2 == -1)
                    {
                        printf("Error: symbol must to be exsit before entry commands\n");
                        ErrorJump
                    }
                }
            }
        }
        free(command);
    }

    if (ORDER_HEAD != NULL && flagError == OFF)
        INTresult = putFullList(SYMBOL_HEAD, ORDER_HEAD, &FULL_LIST, &EXTERN_HEAD);

    
    switch (INTresult)
    {
    case -1:
    {
        printf("Error:  Memory error,cannot allocate memory;.\n");
        flagError = ON;
    }
    case -2:
    {
        printf("Error: extern type can't be in I2 commands type.\n");
        flagError = ON;
    }
    case -3:
    {
        printf("Error: missing symbol with the same name in J commands.\n");
        flagError = ON;
    }
    }
}

void releaseLists()
{
    releaseSymbol(&SYMBOL_HEAD);
    releaseData(&HEAD_DATA);
    releaseORDER(&ORDER_HEAD);
    releaseEntry(&ENTRY_HEAD);
    releaseExtern(&EXTERN_HEAD);
    releaseFullList(&FULL_LIST);
}
