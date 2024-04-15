#include "convertText.h"
#include "symbol.h"
#include "data.h"
#include "order.h"
#include "addEx.h"
#include "lineOutput.h"

char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    if (result)
    {
        strcpy(result, s1);
        strcat(result, s2);
        return result; /* return the name of the file */
    }
    else
    {
        return NULL;
    }
}

int textToNum(char *text, int from, int end)
{
    int i = 0;
    int result = 0, start = from; /* the same value as index */
    if (text[start] == '-' || text[start] == '+')
        start++;
    while (end - i >= start)
    {
        result += pow(10, i) * ((int)(text[end - i] - '0'));
        i++;
    }
    return (text[from] == '-') ? -result : result;
}








