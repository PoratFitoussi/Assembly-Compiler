#include <stdio.h>

#include "symbol.h"
#include "data.h"
#include "order.h"
#include "addEx.h"
#include "lineOutput.h"
#include "convertText.h"

extern int ICF; /* the address of the order */

int printFile(const char *fileName, dptr HEAD_DATA, entryptr ENTRY_HEAD, externptr EXTERN_HEAD, fptr FULL_LIST)
{
    int place = 0, value = 0, lineNumber;
    char *OrderDataFIle = NULL;
    char *EnrtyFile = NULL;
    char *ExternFile = NULL;
    FILE *fp1, *fp2, *fp3;
    lineNumber = numLine(&HEAD_DATA, ICF);

    if (FULL_LIST || HEAD_DATA)
    {
        OrderDataFIle = concat(fileName, ".ob\0");
        if (!OrderDataFIle || !(fp1 = fopen(OrderDataFIle, "w")))
            return -1;
        fprintf(fp1, "     %d %d\n", ICF - 100, lineNumber);
        if (FULL_LIST)
        {
            while (FULL_LIST) /*print to the file all the order */
            {
                fprintf(fp1, "0%d ", FULL_LIST->address);
                fprintf(fp1, "%X", FULL_LIST->A2);
                fprintf(fp1, "%X ", FULL_LIST->A1);
                fprintf(fp1, "%X", FULL_LIST->B2);
                fprintf(fp1, "%X ", FULL_LIST->B1);
                fprintf(fp1, "%X", FULL_LIST->C2);
                fprintf(fp1, "%X ", FULL_LIST->C1);
                fprintf(fp1, "%X", FULL_LIST->D2);
                fprintf(fp1, "%X\n", FULL_LIST->D1);

                FULL_LIST = FULL_LIST->next;
            }
        }
        if (HEAD_DATA)
        {
            while (HEAD_DATA)
            {   /* all the options for data - type commands */
                if (HEAD_DATA->type == 1)
                {
                    if (place == 0)
                    {
                        fprintf(fp1, "0%d %X%X ", value + ICF, HEAD_DATA->A2, HEAD_DATA->A1);
                        place = 2;
                        value += 4;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 1)
                    {
                        fprintf(fp1, "%X %X", HEAD_DATA->A2, HEAD_DATA->A1);
                        place = 3;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 2)
                    {
                        fprintf(fp1, "%X%X ", HEAD_DATA->A2, HEAD_DATA->A1);
                        place = 4;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 3)
                    {
                        fprintf(fp1, "%X %X", HEAD_DATA->A2, HEAD_DATA->A1);
                        place = 5;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 4)
                    {
                        fprintf(fp1, "%X%X ", HEAD_DATA->A2, HEAD_DATA->A1);
                        place = 6;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 5)
                    {
                        fprintf(fp1, "%X %X", HEAD_DATA->A2, HEAD_DATA->A1);
                        place = 7;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 6)
                    {
                        fprintf(fp1, "%X%X\n", HEAD_DATA->A2, HEAD_DATA->A1);
                        place = 0;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 7)
                    {
                        fprintf(fp1, "%X \n0%d %X", HEAD_DATA->A2, value + ICF, HEAD_DATA->A1);
                        place = 1;
                        value += 4;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                }
                if (HEAD_DATA->type == 2)
                {
                    if (place == 0)
                    {
                        fprintf(fp1, "0%d %X%X %X%X ", value + ICF, HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1);
                        place = 4;
                        value += 4;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 1)
                    {
                        fprintf(fp1, "%X %X%X %X", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1);
                        place = 5;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 2)
                    {
                        fprintf(fp1, "%X%X %X%X ", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1);
                        place = 6;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 3)
                    {
                        fprintf(fp1, "%X %X%X %X", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1);
                        place = 7;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 4)
                    {
                        fprintf(fp1, "%X%X %X%X\n", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1);
                        place = 0;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 5)
                    {
                        fprintf(fp1, "%X %X%X \n0%d %X", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, value + ICF, HEAD_DATA->B1);
                        place = 1;
                        value += 4;
                        continue;
                    }
                    if (place == 6)
                    {
                        fprintf(fp1, "%X%X \n0%d %X%X ", HEAD_DATA->A2, HEAD_DATA->A1, value + ICF, HEAD_DATA->B2, HEAD_DATA->B1);
                        place = 2;
                        value += 4;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                    if (place == 7)
                    {
                        fprintf(fp1, "%X \n0%d %X%X %X", HEAD_DATA->A2, value + ICF, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1);
                        place = 3;
                        value += 4;
                        HEAD_DATA = HEAD_DATA->next;
                        continue;
                    }
                }
                if (HEAD_DATA->type == 3)
                {
                    if (place == 0)
                    {
                        fprintf(fp1, "0%d %X%X %X%X ", value + ICF, HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1);
                        fprintf(fp1, "%X%X %X%X\n", HEAD_DATA->C2, HEAD_DATA->C1, HEAD_DATA->D2, HEAD_DATA->D1);
                        HEAD_DATA = HEAD_DATA->next;
                        value += 4;
                        continue;
                    }
                    if (place == 1)
                    {
                        fprintf(fp1, "%X %X%X %X%X ", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1, HEAD_DATA->C2);
                        fprintf(fp1, "%X%X \n0%d %X", HEAD_DATA->C1, HEAD_DATA->D2, value + ICF, HEAD_DATA->D1);
                        HEAD_DATA = HEAD_DATA->next;
                        value += 4;
                        continue;
                    }
                    if (place == 2)
                    {
                        fprintf(fp1, "%X%X %X%X ", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1);
                        fprintf(fp1, "%X%X \n0%d %X%X ", HEAD_DATA->C2, HEAD_DATA->C1, value + ICF, HEAD_DATA->D2, HEAD_DATA->D1);
                        HEAD_DATA = HEAD_DATA->next;
                        value += 4;
                        continue;
                    }
                    if (place == 3)
                    {
                        fprintf(fp1, "%X %X%X %X%X\n", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1, HEAD_DATA->C2);
                        fprintf(fp1, "0%d %X%X %X", value + ICF, HEAD_DATA->C1, HEAD_DATA->D2, HEAD_DATA->D1);
                        HEAD_DATA = HEAD_DATA->next;
                        value += 4;
                        continue;
                    }
                    if (place == 4)
                    {
                        fprintf(fp1, "%X%X %X%X \n", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1);
                        fprintf(fp1, "0%d %X%X %X%X ", value + ICF, HEAD_DATA->C2, HEAD_DATA->C1, HEAD_DATA->D2, HEAD_DATA->D1);
                        HEAD_DATA = HEAD_DATA->next;
                        value += 4;
                        continue;
                    }
                    if (place == 5)
                    {
                        fprintf(fp1, "%X %X%X \n0%d ", HEAD_DATA->A2, HEAD_DATA->A1, HEAD_DATA->B2, value + ICF);
                        fprintf(fp1, "%X%X %X%X %X", HEAD_DATA->B1, HEAD_DATA->C2, HEAD_DATA->C1, HEAD_DATA->D2, HEAD_DATA->D1);
                        HEAD_DATA = HEAD_DATA->next;
                        value += 4;
                        continue;
                    }
                    if (place == 6)
                    {
                        fprintf(fp1, "%X%X \n0%d %X%X ", HEAD_DATA->A2, HEAD_DATA->A1, value + ICF, HEAD_DATA->B2, HEAD_DATA->B1);
                        fprintf(fp1, "%X%X %X%X ", HEAD_DATA->C2, HEAD_DATA->C1, HEAD_DATA->D2, HEAD_DATA->D1);
                        HEAD_DATA = HEAD_DATA->next;
                        value += 4;
                        continue;
                    }
                    if (place == 7)
                    {
                        fprintf(fp1, "%X \n0%d %X%X %X%X ", HEAD_DATA->A2, value + ICF, HEAD_DATA->A1, HEAD_DATA->B2, HEAD_DATA->B1, HEAD_DATA->C2);
                        fprintf(fp1, "%X%X %X", HEAD_DATA->C1, HEAD_DATA->D2, HEAD_DATA->D1);
                        HEAD_DATA = HEAD_DATA->next;
                        value += 4;
                        continue;
                    }
                }
            }
        }
        fclose(fp1);
    }

    if (ENTRY_HEAD) /* make sure there is entry commands*/
    {
        EnrtyFile = concat(fileName, ".ent\0");
        if (!EnrtyFile || !(fp2 = fopen(EnrtyFile, "w")))
            return -1;

        while (ENTRY_HEAD)
        {
            fprintf(fp2, "0%d %s\n", ENTRY_HEAD->address, ENTRY_HEAD->name);
            ENTRY_HEAD = ENTRY_HEAD->next;
        }
        fclose(fp2);
    }
    if (EXTERN_HEAD) /* make sure there is extern commands */
    {
        ExternFile = concat(fileName, ".ext\0");
        if (!ExternFile || !(fp3 = fopen(ExternFile, "w")))
            return -1;
        while (EXTERN_HEAD)
        {
            fprintf(fp3, "0%d %s\n", EXTERN_HEAD->address, EXTERN_HEAD->name);
            EXTERN_HEAD = EXTERN_HEAD->next;
        }
        fclose(fp3);
    }
    return 1;
}
