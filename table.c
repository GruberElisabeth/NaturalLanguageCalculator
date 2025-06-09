#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "table.h"

struct Node *addToList(struct Node *list, char *addChar, char *type, char *value)
{
    // if it is the first element in the table, a new node is made and the values are inserted
    if (!findDouble(list, addChar))
    {
        if (list == NULL)
        {
            struct Node *node = (struct Node *)malloc(sizeof(struct Node));
            strcpy(node->name, addChar);
            insertVal(node, value, type);
            node->next = NULL;
            return node;
        }
        else
        {
            // otherwise the end of the existing list is searched
            struct Node *node = list;
            if (node->name != NULL)
            {
                while (node->next == NULL)
                {
                    node = node->next;
                }
            }

            // at the end of the list, a new node is added with the provided values
            struct Node *nextNode = (struct Node *)malloc(sizeof(struct Node));
            strcpy(node->name, addChar);
            insertVal(list, value, type);
            node->next = nextNode;
            return list;
        }
    }
    fprintf(stderr, "The element does already exist.\n");
    exit(1);
}

int getInt(struct Node *list, char *name)
{
    if (list != NULL)
    {
        if ((strcmp(list->name, name) == 0) && (strcmp(list->type, "int") == 0))
        {
            return list->valInt;
        }
        struct Node *node = list;
        while (node->next != NULL)
        {
            node = node->next;

            if ((strcmp(node->name, name) == 0))
            {
                if (strcmp(node->type, "int") == 0)
                {
                    return node->valInt;
                }
                fprintf(stderr, "The variable with name:  %s is not of type int", name);
                exit(1);
            }
        }
    }
    fprintf(stderr, "There is no integer with the name %s", name);
    exit(1);
}

double getDouble(struct Node *list, char *name)
{
    if (list != NULL)
    {
        if ((strcmp(list->name, name) == 0) && (strcmp(list->type, "double") == 0))
        {
            return list->valDou;
        }
        struct Node *node = list;
        while (node->next != NULL)
        {
            node = node->next;
            if ((strcmp(node->name, name) == 0))
            {
                if (strcmp(node->type, "double") == 0)
                {
                    return node->valDou;
                }
                fprintf(stderr, "The variable with name:  %s is not of type double", name);
                exit(1);
            }
        }
    }
    fprintf(stderr, "There is no double with the name %s", name);
    exit(1);
}

bool getBoolean(struct Node *list, char *name)
{
    if (list != NULL)
    {
        if ((strcmp(list->name, name) == 0) && (strcmp(list->type, "bool") == 0))
        {
            return list->valBool;
        }
        struct Node *node = list;
        while (node->next != NULL)
        {
            node = node->next;
            if ((strcmp(node->name, name) == 0))
            {
                if (strcmp(node->type, "bool") == 0)
                {
                    return node->valBool;
                }
                fprintf(stderr, "The variable with name:  %s is not of type boolean", name);
                exit(1);
            }
        }
    }
    fprintf(stderr, "There is no boolean with the name %s", name);
    exit(1);
}

struct Node *deleteItem(struct Node *list, char *name)
{
    if (list != NULL)
    {
        if ((strcmp(list->name, name) == 0))
        {
            return list->next;
        }
        struct Node *node = list;
        struct Node *before = NULL;
        while (node->next != NULL)
        {
            before = node;
            node = node->next;
            if ((strcmp(node->name, name) == 0))
            {
                before->next = node->next;
                return list;
            }
        }
    }
    fprintf(stderr, "There is no Value with the name %s", name);
    exit(1);
}

struct Node *updateItem(struct Node *list, char *addChar, char *value)
{
    return NULL;
}

bool findDouble(struct Node *list, char *name)
{
    if (list != NULL)
    {
        if ((strcmp(list->name, name) == 0))
        {
            return list->next;
        }
        struct Node *node = list;
        struct Node *before = NULL;
        while (node->next != NULL)
        {
            before = node;
            node = node->next;
            if ((strcmp(node->name, name) == 0))
            {
                fprintf(stderr, "There is already a variable with this name: %s.", name);
                exit(1);
            }
        }
    }
    return false;
}

void insertVal(struct Node *list, char *value, char *type)
{
    if (strcmp(type, "int") == 0)
    {
        list->valInt = atoi(value);
    }
    else if (strcmp(type, "bool") == 0)
    {
        if (strcmp(value, "true") == 0)
        {
            list->valBool = true;
        }
        else if (strcmp(value, "true") == 0)
        {
            list->valBool = false;
        }
    }
    else if (strcmp(type, "double") == 0)
    {
        list->valDou = atof(value);
    }
    else
    {
        fprintf(stderr, "The type did not match any allowed type.");
        exit(1);
    }
}
