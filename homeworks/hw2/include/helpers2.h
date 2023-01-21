#ifndef HELPERS_2_H
#define HELPERS_2_H
#include "linkedlist.h" //ok to include??

// Declare all helper functions for hw2 in this file

//strchar
char* myStrChar(char* str, int achar);

//strlen
int myStrLen(char* str);

//strcmp
int myStrCmp(const char* str1,const char* str2);

//destroy listnode chain
void recurdestroylnchain(node_t * anode,list_t * alist);

#endif