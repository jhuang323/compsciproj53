// Define all helper functions for hw2 in this file
#include "helpers2.h"
#include <stdio.h>

char* myStrChar(char* str, int achar)
{
    //find ptr location of char else NULL

    //check if first char is null
    // if(*(str) == '\0')
    // {
    //     return NULL;
    // }

    char * mvcharptr = str;

    while(*mvcharptr != '\0')
    {
        // printf("Current char: %c\n",*mvcharptr);

        if(*mvcharptr == achar)
        {
            return mvcharptr;
        }

        //increment ptr
        mvcharptr++;
    }



    //edge case where it match end
    if(*mvcharptr == achar)
    {
        return mvcharptr;
    }
    

    //return NUlL
    return NULL;

    
}


int myStrLen(char* str)
{
    //use pointer arithmetic
    char* tempptr = str;

    while(*tempptr != '\0')
    {
        tempptr++;
    }

    //calc
    return tempptr - str;
}


//strcmp
int myStrCmp(const char* str1,const char* str2)
{
    const char * tempstr1ptr = str1;
    const char * tempstr2ptr = str2;


    while(*tempstr1ptr != '\0' && *tempstr2ptr != '\0')
    {
        // printf("comparing %c,%c\n",*tempstr1ptr,*tempstr2ptr);

        if(*tempstr1ptr != *tempstr2ptr)
        {
            // printf("diff found\n");
            return *tempstr1ptr - *tempstr2ptr;
        }

        //update pointers
        tempstr1ptr++;
        tempstr2ptr++;
    }

    // printf("final comparing %d,%d\n",*tempstr1ptr,*tempstr2ptr);

    //return str1 -str2 ??
    return *tempstr1ptr - *tempstr2ptr;

}

//insertinrevorder function test
void InsertInReverseOrder(list_t* list, void* val_ref) {
    if(list == NULL || val_ref == NULL)
        return;
    if (list->length == 0) {
        InsertAtHead(list, val_ref);
        return;
    }

    node_t** head = &(list->head);
    node_t* new_node;
    new_node = malloc(sizeof(node_t));
    new_node->data = val_ref;
    new_node->next = NULL;

    if (list->comparator(new_node->data, (*head)->data) > 0) {
        new_node->next = *head;
        *head = new_node;
    } else if ((*head)->next == NULL) {
        (*head)->next = new_node;
    } else {
        node_t* prev = *head;
        node_t* current = prev->next;
        while (current != NULL) {
            if (list->comparator(new_node->data, current->data) < 0) {
                if (current->next != NULL) {
                    prev = current;
                    current = current->next;
                } else {
                    current->next = new_node;
                    break;
                }
            } else {
                prev->next = new_node;
                new_node->next = current;
                break;
            }
        }
    }
    list->length++;
}
