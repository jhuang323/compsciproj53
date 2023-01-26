#ifndef HELPERS_2_H
#define HELPERS_2_H
#include "linkedlist.h" //ok to include??
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hw2.h"

// Declare all helper functions for hw2 in this file

//strchar
char* myStrChar(char* str, int achar);

//strlen
int myStrLen(char* str);

//strcmp
int myStrCmp(const char* str1,const char* str2);

//my isdigit
int myIsDigit(int achar);

//my check if entire string is digit
int myCheckStrIsDigit(const char * str);

//mycheckdate
int myCheckDate(char * givstr,int * monnum,int * daynum, int * yearnum);

//destroy listnode chain
void recurdestroylnchain(node_t * anode,list_t * alist);

// //test insertinreverse order for LL
void InsertInReverseOrder(list_t* list, void* val_ref);

//date conversion to unix
long int myConvertToUnix(int amon,int aday,int ayear);



#endif