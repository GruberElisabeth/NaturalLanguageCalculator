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

/*
Creates a new Node with the input parameters and adds it to the input List at the end
It checks, if the provided list is empty, if a Node with the input name does already exist
and if the declared type matches the value. 
Input: the name, type of the value and the value it self
Output: the first Node of the List with the added Node (which has the input parameters)

 */
struct Node *addToList(struct Node *list, char *addChar, char *type, char *value);

/*
deletes the Node in the input list with the input name
Input: headnode of the list in which the Node should be deleted; name of the node
Output: headnode of the List without the specified node
*/
struct Node *deleteItem(struct Node *list, char *name);

/*
searches for the value of the specified Node and returns it
it gives an error, when the node can't be found or doesn't match the expected type
Input: headnode of the list in which the value should be found, name of the searched Node
Output: double or boolean Value depending on the method
*/
double getDouble(struct Node *list, char *name);
bool getBoolean(struct Node *list, char *name);

/*
updates the value of an already declared variable
Input: headnode of the list in which the value should be found, name of the node, new value to replace the old one
Output: the headnode of the list with the updated value 
*/
struct Node *updateItem(struct Node *list, char *name, char *value);

/*
prints the variable names and values of the list
Input: headnode of the list, which should be printed
Output: none
*/
void printList (struct Node *list);

/*
helpermethod finds duplications of the variable name in a list
Input: the headnode of the list which should be checked, the name of the node
Output: true, iff there was already a node with the input name
*/
bool findDouble(struct Node *list, char *name);

/*
helpermethod inserts a value into a Node
Input: the Node to insert the parameters, the value and type of the node
Output: none
*/
void insertVal(struct Node *list, char *value, char *type);


