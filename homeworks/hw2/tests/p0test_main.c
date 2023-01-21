#include <stdio.h>
#include <stdlib.h>
#include <string.h> //string.h functions are not allowed in your assignment

#include "helpers2.h"
#include "hw2.h"
#include "linkedlist.h"

int main(void) {

    printf("\n****Can't modify the arguments*****\n");
    char* sampleStr = "Who?\n What!? Why???";   //reference to static string
    char* delims = "?\n";                       //reference to static string
    
    char* result = myStrCpy(sampleStr, delims);

    printf("sampleStr: %s\nresult: %s, result address: %p\n\n", sampleStr, result, result);   

    printf("\n****Did you modify the arguments???*****\n");
    char sampleStr2[] = "Who?\n What!? Why???";   //reference to static string
    char delims2[] = "?\n";                       //reference to static string
    
    char* result2 = myStrCpy(sampleStr2, delims2);

    printf("sampleStr2: %s\nresult: %s, result address: %p\n\n", sampleStr2, result2, result2);   

    if(strcmp(sampleStr, sampleStr2) != 0) //string.h functions are not allowed in your assignment!!!
        printf("OOPS!!! sampleStr2 was modified - Don't do that!\n");

    if(strcmp(delims, delims2) != 0) //string.h functions are not allowed in your assignment!!!
        printf("OOPS!!! delims2 was modified - Don't do that!\n");

    printf("tests given in examples-------------\n");

    printf("test1\n");
    char * extest1 = "Who?\n What!? Why???";
    char * test1delim = "?\n";
    
    char * test1retstr = myStrCpy(extest1,test1delim);
    printf("returned test1: %s\n",test1retstr);

    //test2
    printf("test2\n");
    char * extest2 = "Who?\n What!? Why???";
    char * test2delim = "!\nt";
    
    char * test2retstr = myStrCpy(extest2,test2delim);
    printf("returned test2: %s\n",test2retstr);


    //test3
    printf("test3\n");
    char * extest3 = "Who?\n What!? Why???";
    char * test3delim = "*!-@#";
    
    char * test3retstr = myStrCpy(extest3,test3delim);
    printf("returned test3: %s\n",test3retstr);

    return 0;
}
