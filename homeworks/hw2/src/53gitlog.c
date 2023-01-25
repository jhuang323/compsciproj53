#include "constants2.h"
#include "helpers2.h"
#include "hw2.h"

int main(int argc, char* argv[]) {
	int D_flag = 0;
	int A_flag = 0;
    int NUM_arg = 0;
    int LEVEL_arg = 0;
    char ORDER_arg = 'n';   // default no order
    char* DATE_arg = NULL;
    char* OUTFILE = NULL;
    char* INFILE = NULL;

    int smalladcounter = 0; //counts the number of times a or d appears

    // Use basic getopt to parse flags and respective arguments
    int option;
    while ((option = getopt(argc, argv, "HD:A:O:I:n:ad" )) >= 0) {
        switch (option) {
            case 'H':
				fprintf(stdout, USAGE_MSG);
				return EXIT_SUCCESS;
            case 'D':
        	    D_flag = 1;
                DATE_arg = optarg;
                break;
            case 'A':
				A_flag = 1;
                LEVEL_arg = atoi(optarg);
                break;
            case 'n':
				NUM_arg = atoi(optarg);
                break;
            case 'a':
            case 'd':
                smalladcounter += 1;
				ORDER_arg = option;
                break;
            case 'O':
				OUTFILE = optarg;
                break;
            case 'I':
				INFILE = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MSG);
                return EXIT_FAILURE;
        }
    }

    // validate a required option was specified - Does not check for more than 1
    if ( ! (A_flag | D_flag) )
    {
        fprintf(stderr, "ERROR: Search mode was not specified.\n\n" USAGE_MSG);
        return EXIT_FAILURE;
    }
    
    // INSERT YOUR IMPLEMENTATION HERE
    // getopts only stored the arguments and performed basic checks. More error checking is still needed!!!!

    //error checking part

    //check if -a and -d appears at once
    if(smalladcounter > 1)
    {
        //arg error
        fprintf(stderr,USAGE_MSG);
        return 1;
    }
    //check if -D and -A appears both times
    if(D_flag == 1 && A_flag == 1)
    {
        //arg error
        fprintf(stderr,USAGE_MSG);
        return 1;
    }

    //check DATE is correct
    int mnum = 0;
    int dnum = 0;
    int ynum = 0;

    

    if(DATE_arg != NULL)
    {
        if(myCheckDate(DATE_arg,&mnum,&dnum,&ynum) == 0)
        {
            //error arg DATE
            printf("DATTE ERROR!\n");
            fprintf(stderr,USAGE_MSG);
            return 1;
        }
    
    }

    printf("ALLOWED !%d\n",NUM_arg);



    return 0;
}
