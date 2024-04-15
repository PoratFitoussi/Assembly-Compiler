typedef struct symbolChart *sptr; /* pointer to the symbol chart*/
typedef struct symbolChart
{
    char *name;  /* the name of the label*/
    int address; /*the address of the label */
    short type;  /* 0 for code, 1 for data, 2 for extern command */
    sptr next;   /* pointer to the next struct */
} symbolList;

/* A function that check if the symbol is already on the list */
int checkExist(sptr head, char *name);
/* A function that put the symbol into the struct  */
int SetSymbol(sptr *head, char *command, int from, int length, int adress, short type);
/* A function that add to the smybol struct the directive extern */
int addSymbols(sptr *head, char *command, int from);
/* A function that release the symbol struct */
void releaseSymbol(sptr *);
/* A function that check if the list is empty*/
int isSYEmpty(sptr head);
