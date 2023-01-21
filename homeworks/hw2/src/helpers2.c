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

