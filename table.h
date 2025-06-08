#include <stdio.h>
#include <stdbool.h>
#include <string.h>


typedef struct Node {
    char name[256];
    char type[256];
    int valInt;
    double valDou;
    bool valBool;
    struct Node* next;
}Node;

//insertion of an item in the symbol table
Node* addToList(Node* list, char addChar[], char type[], char value[]);

//deletion if any item from the symbol table
Node* deleteItem(Node* list, char name[]);

//searching of desired item from symbol table
int getInt(Node* list, char name[]);
double getDouble(Node* list, char name[]);
bool getBoolean(Node* list, char name[]);

bool findDouble(Node* list, char name[]);
