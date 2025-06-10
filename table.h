#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node
{
    char name[256];
    char type[256];
    int valInt;
    double valDou;
    bool valBool;
    struct Node *next;
} Node;

// insertion of an item in the symbol table
struct Node *addToList(struct Node *list, char *addChar, char *type, char *value);

// deletion if any item from the symbol table
struct Node *deleteItem(struct Node *list, char *name);

// searching of desired item from symbol table
int getInt(struct Node *list, char *name);
double getDouble(struct Node *list, char *name);
bool getBoolean(struct Node *list, char *name);
struct Node *updateItem(struct Node *list, char *name, char *value);
void printList (struct Node *list);

bool findDouble(struct Node *list, char *name);
void insertVal(struct Node *list, char *value, char *type);


