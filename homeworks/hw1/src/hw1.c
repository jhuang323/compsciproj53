#include "hw1.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
// You may define any helper functions you want. Place them in helpers.c/.h

// Main program
int main(int argc, char *argv[]) {
    // Insert your hw1 code here    

    //Input validation git

    // int retcheckrequired = checkreqoptions(argc,argv);
    // printf("returnval: %d",retcheckrequired);

    // if(retcheckrequired == 0)
    // {
    //     //print usage to stderr


    //     //error with args
    //     return 1;
    // }

    //test -n option
    

    // int retval = functnoption();
    // return retval;

//    return functhoption(argc,argv);
    //if -n option
    if(strcmp(argv[1],"-n") == 0)
    {
        // n option
        return functnoption();
    }
    else if(strcmp(argv[1],"-l") == 0)
    {
        //l option
        return functloption(argc,argv);
    }
    else if(strcmp(argv[1],"-h") == 0)
    {
        //h option
        return functhoption(argc,argv);
    }

    
    
}
