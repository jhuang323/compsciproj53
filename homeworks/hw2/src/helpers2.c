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

    // printf("comparing %d,%d\n",*tempstr1ptr,*tempstr2ptr);

    //return str1 -str2 ??
    return *tempstr1ptr - *tempstr2ptr;

}

