// Declare all helper functions for hw1 in this file
#include "helpers1.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void printtest()
{
    printf("hello!");
}

int alldigitscheck(char * astr)
{
    //returns 1 if contains all digits
    for (int i = 0; i < strlen(astr);i++)
    {
        if(isdigit(astr[i]) == 0)
        {
            return 0;
        }
    }

    return 1;
}

int validargcfgbg(char *argvptr[])
{
    //validate FG and BG 1 = valid

    // printf("in validargfgbg-----------------------\n");

    // printf("current: %s\n",*argvptr);
    //move argvptr 1 to right
    argvptr++;
    // printf("current: %s\n",*argvptr);
    int fgnum = -1;
    if(alldigitscheck(*argvptr))
    {

        fgnum = atoi(*argvptr);
    }
    //move argvptr 1 to right
    argvptr++;
    // printf("current: %s\n",*argvptr);
    int bgnum = -1;
    if(alldigitscheck(*argvptr))
    {
        bgnum = atoi(*argvptr);
    }
    

    // printf("nums %d %d\n",fgnum,bgnum);

    if(fgnum < 30 || fgnum > 37)
    {
        // printf("false detect1\n");
        //return false
        return 0;
    }
    if(bgnum < 40 || bgnum > 47)
    {
        // printf("false detect2\n");
        //return false
        return 0;
    }

    //return true
    return 1;

    // printf("the fg bg: %d %d\n",fgnum,bgnum);

    // printf("current: %s\n",*argvptr);

    
}

int checkreqoptions(int argc,char *argv[])
{
    //ret 1 true so errors git
    //check  argc >=2
    // printf("argc %d\n",argc);

    // for(int i; i < argc; i++)
    // {
    //     printf("arg: %s\n",argv[i]);
    // }

    if(argc < 2)
    {
        //errror with args
        return 0;
    }

    // printf("string at 1,%s\n",argv[1]);

    // printf("val: %d",strcmp(argv[1],"-n") == 0);

    if(strcmp(argv[1],"-n") == 0)
    {
        // -n option
        //check if length is 2
        if(argc == 2)
        {
            return 1;
        }
        
    }
    else if(strcmp(argv[1],"-l") == 0)
    {
        //-l option
        if(argc >= 3)
        {
            //check for -S -I
            if(argc >= 4 && argc <= 5)
            {
                int sargcount = 0;
                int iargcount = 0;

                for(int i = 3;i < argc;i++)
                {

                    if(strcmp(argv[i],"-S") == 0)
                    {
                        sargcount++;
                    }
                    else if(strcmp(argv[i],"-I") == 0)
                    {
                        iargcount++;
                    }
                    else
                    {
                        //not match either
                        return 0;
                    }
                }

                // printf("s %d i %d\n",sargcount,iargcount);

                if(sargcount > 1 || iargcount > 1)
                {
                    return 0;
                }

                if(sargcount == 1 || iargcount == 1)
                {
                    return 1;
                }
            }

            //case where no optionals
            if(argc == 3)
            {
                return 1;
            }
        }

        

    }
    else if(strcmp(argv[1],"-h") == 0)
    {
        // printf("h arg detect: \n");

        //-h option
        if(argc >= 3)
        {
            //check for -S -I -C FG BG
            if(argc >= 4 && argc <= 8)
            {
                int sargcount = 0;
                int iargcount = 0;
                int cargcount = 0;

                int ctesttwo = 0;

                int i = 3;

                while(i < argc)
                {

                    // if(ctesttwo > 0 && ctesttwo < 3)
                    // {
                    //     printf("cdetect val %s\n",argv[i]);
                    //     //test fg
                    //     if (ctesttwo == 1)
                    //     {
                    //         printf("%d less than \n", atoi(argv[i]) > 37);
                    //         //test fg
                    //         if(atoi(argv[i]) < 30 || atoi(argv[i]) > 37)
                    //         {
                    //             return 0;
                    //         }
                    //     }
                    //     //testbg
                    //     if (ctesttwo == 2)
                    //     {
                    //         //test bg
                    //         if(atoi(argv[i]) < 40 || atoi(argv[i]) > 47)
                    //         {
                    //             return 0;
                    //         }
                    //     }
                    //     printf("added 11 %d\n",ctesttwo);
                    //     ctesttwo++;
                    // }

                    // printf("ctest %d",ctesttwo);

                    if(strcmp(argv[i],"-C") == 0 && ctesttwo == 0)
                    {
                        //must have next two args
                        // printf("frst in c\n");
                        // printf("%d;%d",i+3,argc);
                        if(argc < (i+3))
                        {
                            //raise error
                            return 0;
                        }

                        // printf("i num %d\n",i);

                        if(validargcfgbg(argv + i) == 1)
                        {

                            //set cargto 1
                            cargcount++;

                            //increment i by 2
                            i = i + 2;
                        }
                        else
                        {
                            //incorrect FG, BG
                            return 0;
                        }

                        // printf("validargcfgbg return: %d \n",retvalidarggfunct);



                        ctesttwo++;
                    }
                    else if(strcmp(argv[i],"-S") == 0)
                    {
                        sargcount++;
                    }
                    else if(strcmp(argv[i],"-I") == 0)
                    {
                        iargcount++;
                    }
                    else
                    {
                    //    printf("ret error");
                        //not match either
                        return 0;
                    }

                    // if(ctesttwo == 3)
                    // {
                    //     ctesttwo++;
                    // }

                    i++;
                }

                // printf("s %d i %d\n",sargcount,iargcount);

                int totalargcount = 0;
                totalargcount += sargcount;
                totalargcount += iargcount;
                totalargcount += cargcount;

                if(sargcount > 1 || iargcount > 1 || cargcount > 1)
                {
                    return 0;
                }


                if(totalargcount > 0 && totalargcount <=3)
                {
                    return 1;
                }
            }

            //case where no optionals
            if(argc == 3)
            {
                return 1;
            }
        }

    }


    return 0;

}

//-n option
int functnoption()
{
    int cchar;
    int boolcurrentchar = 0; 
    int atleastonce = 0;
    

    while((cchar = getchar()) != EOF)
    {
        // printf("char is ");
        // putchar(cchar);
        // printf(" int is %d, is digit: %d\n",cchar,isdigit(cchar));

        int isdigitbool = isdigit(cchar);

        if(isdigitbool)
        {
            //set atleastonce = 1
            atleastonce = 1;
            //set bool ischar true
            boolcurrentchar = 1;
            //print digit with \n
            putchar(cchar);
            
        }
        else
        {
            if(boolcurrentchar)
            {
                //print newline
                printf("\n");
                //set boolcurrentcar to false
                boolcurrentchar = 0;
            }
            
        }
    }

    //add \n at end
    if(boolcurrentchar)
            {
                //print newline
                printf("\n");
            }
    
    //check if atleast once = 0
    if(atleastonce == 0)
    {
        //no match found
        return 2;
    }

    // fprintf(stderr,"2");

    return 0;

}


//-l option
int functloption(int argc,char *argv[])
{
    //test -l option

    

    int soptiontrue = 0;
    int ioptiontrue = 0;

    for(int i = 3;i < argc; i++)
    {
        //check of s option
        if(strcmp(argv[i],"-S") == 0)
        {
            soptiontrue =1;
        }
        //check i option
        if(strcmp(argv[i],"-I") == 0)
        {
            ioptiontrue =1;
        }
    }


    int cchar;
    char *findwordptr = argv[2];
    int occurancecount = 0;

    // printf("word: %s\n",findwordptr);

    int countline = 1;
    int countword = 1;

    //bool
    int beginchecking = 1;
    int prevcharnotdelimit = 0;

    //set charpointer to first
    char curcheckchar = *findwordptr;
    // printf("current char: %d\n",curcheckchar);

    int charcounter = 0;

    //prevnewline
    int prevnewline = 0;

    //startingcharisdelimit
    int startcharcheckdelim = 1;
    

    while((cchar = getchar()) != EOF)
    {
        //pritn out char for test
        // putchar(cchar);
        // printf("  ");

        //check of space or newline then set 
        if(cchar == '\n' || cchar == ' ')
        {
            //check of \0 for currentchar
            if(curcheckchar == '\0' && (startcharcheckdelim == 1))
            {
                printf("%d:%d\n",countline,countword);
                occurancecount++;
            }

            

            //reset ptr
            findwordptr = argv[2];
            curcheckchar = *findwordptr;
        }
        else
        {
            // printf("    inelse ");
            // putchar(curcheckchar);
            //current char matches targetchar

            if(ioptiontrue)
            {
                //test case insensitive
                cchar = tolower(cchar);
                curcheckchar = tolower(curcheckchar);
            }


            if(cchar == curcheckchar)
            {
                // printf(" matched ");
                //matches increment findwordptr
                findwordptr++;
                //update current char
                curcheckchar = *findwordptr;
                // putchar(curcheckchar);
                // printf("updated char: %d",curcheckchar);

                //if prev char is delim
                if(prevcharnotdelimit == 0)
                {
                    //set starting char delim to 1
                    startcharcheckdelim = 1;

                }
                // else
                // {
                //     //set to 1
                //     startcharcheckdelim = 1;

                // }

            }
            else
            {
                //reset the pointer
                //reset ptr
                findwordptr = argv[2];
                curcheckchar = *findwordptr;

                //reset startingchar delim check
                startcharcheckdelim = 0;
            }

            //test add to word
            // countword++;
            
        }


        
        //count words
        if(cchar == ' ' && prevcharnotdelimit)
        {
            countword++;
        }

        //check prev char is not delimit
        if((cchar != ' ') && (cchar != '\n'))
        {
            prevcharnotdelimit = 1;
        }
        else
        {
            prevcharnotdelimit = 0;
        }

        //count lines
        if(cchar == '\n' )
        {
            countline++;
            //reset countword to 1
            countword = 1;
        }

        //check prevnewline
        if(cchar == '\n')
        {
            prevnewline = 1;
        }
        else
        {
            prevnewline = 0;
        }

        //increment charcounter
        charcounter++;

        // printf("endcurrchar: ");
        // putchar(curcheckchar);
        // printf("\n");
    }
    // case where word appears at end of line
    if(curcheckchar == '\0' && (startcharcheckdelim == 1))
    {
        printf("%d:%d\n",countline,countword);
        occurancecount++;
    }
    // printf("countline %d",countline);

    //case where no match
    if(occurancecount == 0)
    {
        //no occurance found
        return 2;
    }

    if(soptiontrue)
    {
        //s option
        fprintf(stderr,"%d %d %d\n",occurancecount,charcounter,countline);

    }
    else
    {
        //no s option
        fprintf(stderr,"%d\n",occurancecount);
    }

    
    

    return 0;

}


int functhoption(int argc,char *argv[])
{
    //test -h option

    

    int soptiontrue = 0;
    int ioptiontrue = 0;
    char * coptionfg = "37";
    char * coptionbg = "41";

    for(int i = 3;i < argc; i++)
    {
        // printf("args: %s",argv[i]);
        //check of s option
        if(strcmp(argv[i],"-S") == 0)
        {
            soptiontrue =1;
        }
        //check i option
        if(strcmp(argv[i],"-I") == 0)
        {
            ioptiontrue =1;
        }
        //check C option
        if(strcmp(argv[i],"-C") == 0)
        {
            // printf("argsfg: %s",argv[i+1]);
            coptionfg = (argv[i+1]);
            coptionbg = (argv[i+2]);

        }
    }

    // printf("options chosen: %s:%s",coptionbg,coptionfg);


    char cchar;
    char *findwordptr = argv[2];
    int occurancecount = 0;

    // printf("word: %s\n",findwordptr);

    int countline = 1;
    int countword = 1;

    //bool
    int beginchecking = 1;
    int prevcharnotdelimit = 0;

    //set charpointer to first
    char curcheckchar = *findwordptr;
    // printf("current char: %d\n",curcheckchar);

    int charcounter = 0;

    //char * using dynamic mem
    int defaultsizeteststr = 1;
    int tststrsize = 1; //default value, can grow
    char * teststr = (char *)malloc(sizeof(char) * (strlen(findwordptr) + 3));
    int charinteststrcount = 0;
    // printf("%s\n",teststr);

    //init teststr to zero
    for(int i = 0; i < (strlen(findwordptr) + 3);i++)
    {
        teststr[i]= '\0';
    }

    int controladd = 0; // controls whether char is added to dynamic str

    //startingcharisdelimit
    int startcharcheckdelim = 1;



    //temp store the targetword
    // char * targetwordarraytemp = (char *)malloc((strlen(findwordptr)+3) * sizeof(char));
    // int temptargetwordarraycounter = 0;
    

    while((cchar = getchar()) != EOF)
    {
        //pritn out char for test
        // putchar(cchar);
        // printf("\x1B[31;41m"); print red
        //printf("\x1B[39;49m");


        //check of space or newline then set 
        if(cchar == '\n' || cchar == ' ')
        {
            //check of \0 for currentchar
            if(curcheckchar == '\0' && (startcharcheckdelim == 1))
            {
                printf("\x1B[%s;%sm",coptionfg,coptionbg);
                // printf("thesttore detect: \n%s",teststr);
                printf("%s",teststr);
                printf("\x1B[39;49m");
                //occurance detected
                // printf("%d:%d\n",countline,countword);
                occurancecount++;

                //reset the store string
                //reset charinteststrcount
                charinteststrcount = 0;

                teststr[0]= '\0';



                //free
                // free(teststr);
                // //alloc
                // teststr = (char *) malloc(sizeof(char) * defaultsizeteststr);

                
                // // printf("reeeallso\n");

                // //reset tstr
                // tststrsize = defaultsizeteststr;

                // //reset charinteststrcount
                // charinteststrcount = 0;

            }

            //reset ptr
            findwordptr = argv[2];
            curcheckchar = *findwordptr;

            //set control add = 0
            controladd = 0;

            //case where \n and no match
            if(charinteststrcount > 0)
            {
                //print out test str
                printf("%s",teststr);
                //reset charinteststrcount
                charinteststrcount = 0;
                teststr[0]= '\0';


            }
        }
        else
        {
            // printf("    inelse ");
            // putchar(curcheckchar);
            //current char matches targetchar

            char tempcompcchar = cchar;

            if(ioptiontrue)
            {
                //test case insensitive
                tempcompcchar = tolower(tempcompcchar);
                curcheckchar = tolower(curcheckchar);
            }

            // printf("comparing %d %d\n",tempcompcchar,curcheckchar);


            if(tempcompcchar == curcheckchar )
            {
                // printf(" matched ");
                //matches increment findwordptr
                findwordptr++;
                //update current char
                curcheckchar = *findwordptr;
                // putchar(curcheckchar);
                // printf("updated char: %d",curcheckchar);

                //control add = 1
                controladd = 1;


                //if prev char is delim
                if(prevcharnotdelimit == 0)
                {
                    //set starting char delim to 1
                    startcharcheckdelim = 1;

                }

                //print teststr
                // printf("printing: %s",teststr);

                //store the targetword
                // printf("store char in targettemp arraY: \n",cchar);
                // putchar(cchar);
                // targetwordarraytemp[temptargetwordarraycounter] = cchar;
                // printf("stored in targetarray: %s\n",targetwordarraytemp);
                // temptargetwordarraycounter++;

                // printf("the store: [%s]\n",teststr); //test
            }
            else
            {
                //reset the pointer
                //reset ptr
                findwordptr = argv[2];
                curcheckchar = *findwordptr;

                //set control add =0
                controladd = 0;

                // printf("-the store: [%s]\n",teststr);
                printf("%s",teststr);


                
                //free
                // free(teststr);
                //alloc
                // teststr = (char *) malloc(sizeof(char) * defaultsizeteststr);

                
                // printf("reeeallso\n");

                //reset tstr
                tststrsize = defaultsizeteststr;

                //reset charinteststrcount
                charinteststrcount = 0;

                teststr[0]= '\0';


                //reset startingchar delim check
                startcharcheckdelim = 0;

                //put things in the temptargetarrayback
                // for(int i = 0;i<=temptargetwordarraycounter;i++)
                // {
                //     // teststr[i] = targetwordarraytemp[i];


                //     //add char to teststr
                //     if((charinteststrcount+1) <= tststrsize)
                //     {
                //         putchar(cchar);
                //         // char * c = &cchar;
                //         // printf("charr (%s)",c);
                //         printf(" char in test count %d",charinteststrcount);
                //         teststr[charinteststrcount] = targetwordarraytemp[i];
                //         printf("current string: %s\n",teststr);
                //         charinteststrcount++;
                //     }
                //     else
                //     {
                //         tststrsize = 2 * tststrsize;
                //         //realloc

                //         teststr = realloc(teststr,sizeof(char) * tststrsize);
                        
                //         //add current char
                //         teststr[charinteststrcount] = targetwordarraytemp[i];
                //         charinteststrcount++;

                //         printf("after realloc99: %s newsize: %d\n",teststr,tststrsize);
                        
                //     }


                // }

                //resetcounter 
                // temptargetwordarraycounter = 0;
                
                
                
            }

            //test add to word
            // countword++;
            
        }


        
        //count words
        if(cchar == ' ' && prevcharnotdelimit)
        {
            countword++;
        }

        //check prev char is not delimit
        if((cchar != ' ') && (cchar != '\n'))
        {
            prevcharnotdelimit = 1;
        }
        else
        {
            prevcharnotdelimit = 0;
        }


        if(controladd ==  1)
        {
            //test growing string
            //add char to teststr
            // if((charinteststrcount+1) <= tststrsize)
            // {
            //     // putchar(cchar);
            //     // char * c = &cchar;
            //     // printf("charr (%s)",c);
            //     // printf(" char in test count %d",charinteststrcount);
            //     teststr[charinteststrcount] = cchar;
            //     // printf("current string: %s\n",teststr);
            //     charinteststrcount++;
            // }
            // else
            // {
            //     tststrsize = 2 * tststrsize;
            //     //realloc

            //     teststr = realloc(teststr,sizeof(char) * tststrsize);
                
            //     //add current char
            //     teststr[charinteststrcount] = cchar;
            //     charinteststrcount++;

            //     // printf("after realloc99: %s newsize: %d\n",teststr,tststrsize);
                
            // }

            teststr[charinteststrcount] = cchar;
            teststr[charinteststrcount+1] = '\0';
            charinteststrcount++;

        }
        else
        {
            //printoutthechar
            // putchar(cchar); //weird
            printf("%c",cchar);
        }
        
        // putchar(cchar);

        //count lines
        if(cchar == '\n')
        {
            countline++;
            //reset countword to 1
            countword = 1;


            //print the newline
            // printf("---------------------final line: %s\n",teststr);

            
        }

        //increment charcounter
        charcounter++;

        

        // printf("endcurrchar: ");
        // putchar(curcheckchar);
        // printf("\n");

        

    }

    


    // case where word appears at end of line
    if(curcheckchar == '\0' && (startcharcheckdelim == 1))
    {

        printf("\x1B[%s;%sm",coptionfg,coptionbg);
        // printf("thesttore detect: \n%s",teststr);
        printf("%s",teststr);
        printf("\x1B[39;49m");

        // printf("%d:%d\n",countline,countword);
        occurancecount++;

        
    }
    // printf("countline %d",countline);

    //free memory
    //free
    free(teststr);

    //case where no match
    if(occurancecount == 0)
    {
        //no occurance found
        return 2;
    }

    if(soptiontrue)
    {
        //s option
        fprintf(stderr,"%d %d %d\n",occurancecount,charcounter,countline);

    }

    
    

    return 0;
}