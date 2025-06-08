#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "table.h"

Node* addToList(Node* list, char addChar[], char type[], char value[]){
    if(list == NULL){
        if(!findDouble(list, addChar)){
            Node* node = (Node*)malloc(sizeof(Node));
            //node->name = addChar;
            //node->valInt = value;
            node->next = NULL;
            return node;
        }
    }else{
        Node* node = list;
        if(node->name != NULL){
            while(node->next == NULL){
                node = node->next;
            }
        }

        Node* nextNode = (Node*)malloc(sizeof(Node));
        //nextNode->name = addChar;
        //nextNode->val = value;
        node->next = nextNode;
    
        return list;
    }
    return list;
}

int getInt(Node* list, char name[]){
    char type [] = {'I','N','T'};
    if(list != NULL){
        if((strcmp(list->name, name) == 0) && (strcmp(list->type, type) == 0)){
            return list->valInt;
        }
        Node* node = list;
        while(node->next != NULL){
            node = node->next;
            if((strcmp(node->name, name) == 0) && (strcmp(node->type, type) == 0)){
                return node->valInt;
            }
        }
    }
    printf("We found no integer with the name %s", name);
    return -10000;
}

double getDouble(Node* list, char name[]){
    char type [] = {'D','O','U','B','L','E'};
    if(list != NULL){
        if((strcmp(list->name, name) == 0) && (strcmp(list->type, type) == 0)){
            return list->valDou;
        }
        Node* node = list;
        while(node->next != NULL){
            node = node->next;
            if((strcmp(node->name, name) == 0) && (strcmp(node->type, type) == 0)){
                return node->valDou;
            }
        }
    }
    printf("We found no double with the name %s", name);
    return -10000;
}

bool getBoolean(Node* list, char name[]){
    char type [] = {'B','O','O','L'};
    if(list != NULL){
        if((strcmp(list->name, name) == 0) && (strcmp(list->type, type) == 0)){
            return list->valBool;
        }
        Node* node = list;
        while(node->next != NULL){
            node = node->next;
            if((strcmp(node->name, name) == 0) && (strcmp(node->type, type) == 0)){
                return node->valBool;
            }
        }
    }
    printf("We found no boolean with the name %s", name);
    return false;
}

Node* deleteItem(Node* list, char name[]){
    if(list != NULL){
        if((strcmp(list->name, name) == 0)){
            return list->next;
        }
        Node* node = list;
        Node* before = NULL;
        while(node->next != NULL){
            before = node;
            node = node->next;
            if((strcmp(node->name, name) == 0)){
                before->next = node->next;
                return list;
            }
        }
    }
    printf("There was no Value with the name %s", name);
    return NULL;
}

bool findDouble(Node* list,char name[]){
    if(list != NULL){
        if((strcmp(list->name, name) == 0)){
            return list->next;
        }
        Node* node = list;
        Node* before = NULL;
        while(node->next != NULL){
            before = node;
            node = node->next;
            if((strcmp(node->name, name) == 0)){
                return true;
            }
        }
    }
    return false;
}








