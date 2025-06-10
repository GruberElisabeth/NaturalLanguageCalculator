#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "table.h"

struct Node *addToList(struct Node *list, char *addChar, char *type, char *value)
{
    if (!findDouble(list, addChar))
    {
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        strcpy(newNode->name, addChar);
        strcpy(newNode->type, type);
        insertVal(newNode, value, type);
        newNode->next = NULL;

        if (list == NULL)
        {
            return newNode;
        }
        else
        {
            struct Node *node = list;
            while (node->next != NULL)
            {
                node = node->next;
            }
            node->next = newNode;
            return list;
        }
    }

    fprintf(stderr, "The element already exists.\n");
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
                if (strcmp(node->type, "double") == 0)
                {
                    return node->valDou;
                }
                
                if (strcmp(node->type, "int") == 0)
                {
                    return node->valInt;
                }

                fprintf(stderr, "The variable with name:  %s is not a number", name);
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

struct Node *updateItem(struct Node *list, char *name, char *value)
{
    if (list != NULL)
    {
        if ((strcmp(list->name, name) == 0))
        {
            insertVal(list, value, list->type);
            return list;
        }
        struct Node *node = list;
        while (node->next != NULL)
        {
            node = node->next;
            if ((strcmp(node->name, name) == 0))
            {
                insertVal(list, value, list->type);
                return list;
            }
        }
    }
    return NULL;
}

void printList (struct Node *list){
    printf("Here you can see all the symbols in your table.");
     if (list != NULL)
    {
        if((strcmp(list->type, "int") == 0)){
            printf("Identifier: %s \tValue: %d \n", list->name, list->valInt);
        }else if((strcmp(list->type, "double") == 0)){
            printf("Identifier: %s \tValue: %lf \n", list->name, list->valDou);
        }else{
            printf("Identifier: %s \tValue: %d \n", list->name, list->valBool);
        }
        
        struct Node *node = list;
        while (node != NULL)
        {
            
            if((strcmp(node->type, "int") == 0)){
                printf("Identifier: %s \tValue: %d \n", node->name, node->valInt);
            }else if((strcmp(node->type, "double") == 0)){
                printf("Identifier: %s \tValue: %lf \n", node->name, node->valDou);
            }else{
                printf("Identifier: %s \tValue: %d \n", node->name, node->valBool);
            }

            node = node->next;
        }
    }

}

bool findDouble(struct Node *list, char *name)
{
    if (list != NULL)
    {
        if ((strcmp(list->name, name) == 0))
        {
            fprintf(stderr, "There is already a variable with this name: %s.", name);
            exit(1);
        }
        struct Node *node = list;
        while (node->next != NULL)
        {
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
        else if (strcmp(value, "false") == 0)
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
