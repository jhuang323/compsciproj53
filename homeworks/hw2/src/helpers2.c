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
    //check for Null
    if(str1 == NULL && str2 == NULL)
    {
        return 0;
    }

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

//my isdigit
int myIsDigit(int achar)
{
    //check char if it is digit
    if(achar < 48 || achar > 57)
    {
        //return zero due to it being non int
        return 0;
    }

    // is a digit
    return 1;
    
}

//my check if entire string is digit
int myCheckStrIsDigit(const char * str)
{
    //use while loop and myisdigit
    const char * mvcharptr = str;

    while(*mvcharptr != '\0')
    {
        // call isdigit for all chars
        if(myIsDigit(*mvcharptr) == 0)
        {
            //isdigit is not number return 0
            return 0;
        }

        //update mvcharptr
        mvcharptr++;

    }

    //after checking through the str
    return 1;
    

}

//my recurdestroy chain
void recurdestroylnchain(node_t * anode,list_t * alist)
{
    // printf("recursive destroy\n");

    if(anode->next != NULL)
    {
        recurdestroylnchain(anode->next,alist);
        
    }
    // printf("freeing\n");
    //call deleter
    if(anode->data != NULL)
    {
        alist->deleter(anode->data);
        //free current anodeindata whatever struct itself
        free(anode->data);
    }
    //free current node
    free(anode);
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

//mycheckdate
int myCheckDate(char * givstr,int * monnum,int * daynum, int * yearnum)
{
    //use while loop to see if 3 / exists

    char * mvcharptr = givstr;
    int slashnum = 0;
    // printf("%c\n",*mvcharptr);

    while(*mvcharptr != '\0')
    {
        // printf("%c\n",*mvcharptr);
        //check for slash /
        if(*mvcharptr == '/')
        {
            slashnum++;
        }

        //update pointer
        mvcharptr++;
    }

    printf("slashnum: %d\n",slashnum);

    if(slashnum != 2)
    {
        //not correct slash
        return 0;
    }

    char * mvstrptr2 = givstr;
    //get day need to free

    char * thedaystr = myStrCpy(mvstrptr2,"/");

    //make sure len == 2
    if(myStrLen(thedaystr) != 2)
    {
        //free str
        free(thedaystr);
        //error
        return 0;
    }

    mvstrptr2 += myStrLen(thedaystr) + 1;

    //get month needed
    char * themonthstr =  myStrCpy(mvstrptr2,"/");
    //make sure len == 2
    if(myStrLen(themonthstr) != 2)
    {
        //free str
        free(thedaystr);
        free(themonthstr);
        //error
        return 0;
    }
    mvstrptr2 += myStrLen(themonthstr) + 1;

    //get year needed
    char * theyearstr =  myStrCpy(mvstrptr2,"/");
    //make sure len == 4
    if(myStrLen(theyearstr) != 4)
    {
        //free str
        free(thedaystr);
        free(themonthstr);
        free(theyearstr);
        //error
        return 0;
    }
    mvstrptr2 += myStrLen(theyearstr) + 1;

    printf("the day str: %s the month str: %s the year str: %s \n",thedaystr,themonthstr,theyearstr);

    if(myCheckStrIsDigit(thedaystr) == 0 || myCheckStrIsDigit(themonthstr) == 0 || myCheckStrIsDigit(theyearstr) == 0)
    {
        //free the strings
        free(thedaystr);
        free(themonthstr);
        free(theyearstr);
        //return on is not all digits
        return 0;

    }

    //set the digits in arg
    *monnum = atoi(themonthstr);
    *daynum = atoi(thedaystr);
    *yearnum = atoi(theyearstr);


    //free the strings
    free(thedaystr);
    free(themonthstr);
    free(theyearstr);

    //check the dates
    


    //return 1 meaning given date is correct
    return 1;

    
}


//date conversion to unix
long int myConvertToUnix(int amon,int aday,int ayear)
{
    //convert to epoch
    struct tm t;
    time_t t_of_day;

    //year - 1900
    t.tm_year = ayear - 1900;
    t.tm_mon = amon - 1; //0 is janurary
    t.tm_mday = aday;

    //11:59:59 pm
    t.tm_hour = 23;
    t.tm_min = 59;
    t.tm_sec = 59;

    //set dst to unknown
    t.tm_isdst = -1;

    t_of_day = mktime(&t);

    printf("seconds since Epoch: %ld\n",(long) t_of_day);

    return (long) t_of_day;
}


