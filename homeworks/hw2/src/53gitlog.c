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
    while ((option = getopt(argc, argv, "HD:A:O:I:n:a" )) >= 0) {
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
    // if(smalladcounter > 1)
    // {
    //     //arg error
    //     fprintf(stderr,USAGE_MSG);
    //     return 1;
    // }

    
    //check if -D and -A appears both times fix this
    int combDAFlag = D_flag + A_flag;
    // printf("combDAflag %d\n",combDAFlag);
    if(combDAFlag != 1)
    {
        //arg error
        // printf("arg ad error!\n");
        fprintf(stderr,USAGE_MSG);
        return 1;
    }

    //check if -A that the LEVEL is 0 or 1
    if(A_flag == 1)
    {
        //check if LEVEL is 0 or 1
        if(LEVEL_arg < 0 || LEVEL_arg > 1)
        {
            //return error
            fprintf(stderr,USAGE_MSG);
            return 1;
        }
    }

    //check DATE is correct
    int mnum = 0;
    int dnum = 0;
    int ynum = 0;
    //the DATE epoch calculated
    long int calcepochdate = 0;

    

    if(DATE_arg != NULL)
    {
        if(myCheckDate(DATE_arg,&mnum,&dnum,&ynum) == 0)
        {
            //error arg DATE
            // printf("DATTE ERROR!\n");
            fprintf(stderr,USAGE_MSG);
            return 1;
        }
        else
        {
            //calc the epoch time
            // printf("time test: \n");
            calcepochdate = myConvertToUnix(mnum,dnum,ynum);

        }
    
    }

    //check of num argument is negative
    if(NUM_arg < 0)
    {
        //numarg is negative return 1
        return 1;
    }

    // printf("ALLOWED !%d\n",NUM_arg);

    //check if input file can be opened
    FILE * finputptr = NULL;
    FILE * foutputptr = NULL;


    //check infile
    if(INFILE != NULL)
    {
        //attempt to open infile
        finputptr = fopen(INFILE,"r");

        if(finputptr == NULL)
        {
            //could not open file
            return 2;
        }
    }
    else
    {
        // printf("setting to stdin!!!!!!!!!!!!!!");
        //infile is null default option
        finputptr = stdin;
    }

    //checkoutfile
    if(OUTFILE != NULL)
    {
        //attempt to open infile
        foutputptr = fopen(OUTFILE,"w");

        if(foutputptr == NULL)
        {
            //could not open file for write
            return 2;
        }
    }
    else
    {
        //outfile is null default option
        foutputptr = stdout;
    }


    //the main program
    //create a list
    list_t * themainproglist = NULL;
    // list_t * testlist = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);

    //check for D or A flag
    if(D_flag == 1)
    {
        themainproglist = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);
    }
    else
    {
        //for the A flag
        themainproglist = CreateList(&AuthorEmailComparator,&AuthorPrinter,&AuthorDeleter);
    }

    //init char for the ordering
    char orderargument = 'f';
    if(ORDER_arg == 'a')
    {
        //set orderarg to a
        orderargument = 'a';
    }

    

    //need to dynamically allocate
    char * linechunk = (char *)malloc(sizeof(char) * 201);

    

    while(fgets(linechunk,200,finputptr) != NULL)
    {
        // printf("The main prog line is :\n");
        // printf("%s|*\n",linechunk);

        long int authtimestamp = -1;
        Author * thecommitauthor = CreateAuthor(linechunk,&authtimestamp);

        //error check author and timestamp
        if(authtimestamp == -1 || thecommitauthor == NULL)
        {
            //special case 3
            return 3;
        }



        //for the D flag
        if(D_flag == 1)
        {
            // the main prog list stors modfile structs

            //check if timestamp > DATE
            if(authtimestamp > calcepochdate)
            {
                // printf("NOT SKIPPTING +++++++++++++++++++++++++++++++++++++++++++");
                //run processmodfile
                ProcessModFile(finputptr,themainproglist,orderargument);
                

                //printing
                // printf("PRINTING ++++++++++++++++++++++++++++++++++++++\n");
                // PrintLinkedList(themainproglist,stdout);

            }
            else
            {
                // printf("SKIPPTING +++++++++++++++++++++++++++++++++++++++++++");
                //advance the lines until a \n is found
                while(fgets(linechunk,201,finputptr) != NULL)
                {

                    // printf("Theline is :\n");
                    // printf("%s|*\n",linechunk);

                    if(myStrCmp(linechunk,"\n") == 0)
                    {
                        // printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!n DETECTED!!!!!!!!!!!!!!\n");
                        break;
                    }
                }
                
            }

            //author is not being used so delete it
            AuthorDeleter(thecommitauthor);
            //free author struct
            free(thecommitauthor);

            

        }
        else
        {
            //for the A flag with 0 or 1
            //need to create new listfor process mod files

            //first need to check if author is in list
            node_t * ndeptrinmainlist = FindInList(themainproglist,thecommitauthor);
            

            //if in list
            if(ndeptrinmainlist != NULL)
            {
                //it is in list
                //call procmod given the internal list
                Author * authinptrnde = ndeptrinmainlist->data;
                //update the commitcount
                authinptrnde->commitCount++;

                ProcessModFile(finputptr,authinptrnde->modFileList,'a');


                //destroy the commitauthor
                AuthorDeleter(thecommitauthor);
                //free author struct
                free(thecommitauthor);


            }
            else
            {
                //it is not in the list
                //call processmod on the current commitauthor
                ProcessModFile(finputptr,thecommitauthor->modFileList,'a');

                //depending on -a or not is how to place in mainlist
                if(ORDER_arg == 'a')
                {
                    //place in alphabetical order
                    InsertInOrder(themainproglist,(void*)thecommitauthor);
                }
                else
                {
                    //place in first seen order
                    InsertAtTail(themainproglist,(void*)thecommitauthor);

                }

            }
        }


    }

    //printing
    // printf("FINALL LIST PRINTING ++++++++++++++++++++++++++++++++++++++\n");

    if(A_flag == 1 && LEVEL_arg == 0)
    {
        node_t * mvauthptr = themainproglist->head;

        while(mvauthptr != NULL)
        {
            //print in level 0
            AuthorPrinter((void*)mvauthptr->data,(void*)foutputptr,0);

            //update moving ptr
            mvauthptr = mvauthptr->next;

        }
    }
    else if(A_flag == 1 && LEVEL_arg == 1)
    {
        //-A 1 command print \n after each author

        node_t * mvauthptr = themainproglist->head;

        if(NUM_arg == 0)
        {
            //for when printinf everything in list
            while(mvauthptr != NULL)
            {
                //print in level 1
                AuthorPrinter((void*)mvauthptr->data,(void*)foutputptr,1);

                // printf("\n");//newline after each author

                //update moving ptr
                mvauthptr = mvauthptr->next;

                //check if mvauthptr is not null
                if(mvauthptr != NULL)
                {
                    printf("\n");//newline after each author
                }

            }

        }
        else
        {
            for(int i = 0; i < NUM_arg;i++)
            {
                //check if currentNodeptr is NULL
                if(mvauthptr == NULL)
                {
                    break;
                }

                //print in level 1
                AuthorPrinter((void*)mvauthptr->data,(void*)foutputptr,1);

                

                //update moving ptr
                mvauthptr = mvauthptr->next;

                //check if mvauthptr is not null
                if(mvauthptr != NULL)
                {
                    printf("\n");//newline after each author
                }

            }
        }

        

    }
    else
    {
        //for all other printing cases
        PrintNLinkedList(themainproglist,foutputptr,NUM_arg);
    }

    
    
    

    //destroy linechunk at the end
    free(linechunk);
    //dont for get to delete list at end
    DestroyList(&themainproglist);

    //close the files at the end
    fclose(finputptr);
    fclose(foutputptr);

    



    //success
    return 0;
}
