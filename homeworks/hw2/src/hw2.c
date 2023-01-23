// Your Name
// NetId

#include "hw2.h"
#include "helpers2.h"
#include "linkedlist.h"

// my macros
#define MAX_LINESIZE 200

// Part 0 Function to implement
char* myStrCpy(char* str, char* delimiters) {
    
    //if str is null
    if(str == NULL)
    {
        return NULL;
    }

    //first find min num
    int minstrlen;

    if(delimiters == NULL)
    {
        minstrlen = myStrLen(str);
    }
    else
    {
        minstrlen = myStrLen(str);

        char* mvdelimptr = delimiters;

        while(*mvdelimptr != '\0')
        {
            char selchar = *mvdelimptr;
            // printf("chartest: %c\n",selchar);

            char* retstrchrptr = myStrChar(str,selchar);

            if(retstrchrptr != NULL)
            {
                int calclen = retstrchrptr-str;
                // printf("the diff %d\n",calclen);

                if(calclen < minstrlen)
                {
                    //update
                    minstrlen = calclen;
                }

                

            }

            

            mvdelimptr++;
        }


    }

    // printf("!!!!!!!!!!!!!Final Min strlen: %d\n",minstrlen + 1);

    //malloc to create new str

    char * functretptr = (char *)malloc(minstrlen + 1);

    //error on NULL
    if(functretptr == NULL)
    {
        return NULL;
    }

    //copy into
    char * tempretstrptr = functretptr;
    char * givenstrptr = str;

    for(int i = 0;i<minstrlen;i++)
    {
        // printf("copy %c\n",*givenstrptr);

        *tempretstrptr = *givenstrptr;

        //increment ptrs
        tempretstrptr++;
        givenstrptr++;
    }

    //add \0 to end
    *tempretstrptr = '\0';
    
    // printf("final str: %s\n",functretptr);

    return functretptr;

}

// Part 1 Functions to implement
ModFile* PutModFile(int ins, int dels, char* filename, ModFile* mf) {
    // return NULL;

    //if mf null case
    if(mf == NULL)
    {
        //null case create new struct

        ModFile * retnewlycreatedmf = (ModFile *)malloc(sizeof(ModFile));

        //init ins
        retnewlycreatedmf->inserts = ins;
        //init delets
        retnewlycreatedmf->deletes = dels;
        //init filename uses mystrcopy
        retnewlycreatedmf->filename = myStrCpy(filename,NULL);

        //return the modfile
        return retnewlycreatedmf;

    }
    else
    {
        // for case not null

        //return null if filename given is NULL
        if(filename == NULL)
        {
            return NULL;
        }

        //return NULL if ins or del is negative
        if(ins < 0 || dels < 0)
        {
            return NULL;
        }

        //check if filename matches
        if(myStrCmp(filename,mf->filename) != 0)
        {
            //return null error
            return NULL;
        }

        //update mf struct
        //update ins by adding to it
        mf->inserts += ins;
        //update dels by adding to it
        mf->deletes += dels;


        //return the mf pointer
        return mf;
    }
}

int ModFileABC_Comparator(const void* file1, const void* file2) {
    // return -999;

    printf("test print filename1: %s\n",((ModFile *)file1)->filename);
    printf("test print filename2: %s\n",((ModFile *)file2)->filename);

    int comparereslt = myStrCmp(((ModFile *)file1)->filename,((ModFile *)file2)->filename);
    printf("the comparison: %d\n",comparereslt);

    if(comparereslt < 0)
    {
        //return -1
        return -1;
    }
    else if(comparereslt > 0)
    {
        //return 1
        return 1;

    }
    else
    {
        //return 0
        return 0;
    }
}

int ModFileTotal_Comparator(const void* file1, const void* file2) {
    // return -999;
    unsigned int file1totalinsdel = ((ModFile *)file1)->inserts + ((ModFile *)file1)->deletes;
    unsigned int file2totalinsdel = ((ModFile *)file2)->inserts + ((ModFile *)file2)->deletes;

    printf("total file1: %d\n",file1totalinsdel);
    printf("total file2: %d\n",file2totalinsdel);

    //if file1 ins+del < file2 ins+del
    if(file1totalinsdel < file2totalinsdel)
    {
        //return -1
        return -1;
    }
    else if(file1totalinsdel > file2totalinsdel)
    {
        //return 1
        return 1;
    }
    else
    {
        //return 0
        return 0;
    }
}

// Part 2 Functions to implement
void ModFile_Printer(void* data, void* fp, int flag) {
    // printf("modfile printer test\n");

    //pretty print
    fprintf((FILE*)fp,"Total Inserts:%d\tTotal Deletes:%d\t%s\n",((ModFile *)data)->inserts,((ModFile *)data)->deletes,((ModFile *)data)->filename);

}

void ModFile_Deleter(void* data) {
    //cast data as Modfile*
    // free((ModFile*))
    printf("testmodfile deleter\n");

    ModFile * mdfileptr = data;
    //free string in Modfile
    free(mdfileptr->filename);
    //then free the modfile struct ???
    free(mdfileptr);

}

node_t* FindInList(list_t* list, void* token)  {
    // return NULL;
    printf("In findin list function\n");

    //check if token is NULL
    if(token == NULL)
    {
        //return null
        return NULL;
    }
    
    //using the specific comparator in list

    node_t * mvnodeptr = list->head;
    // ModFile_Printer(mvnodeptr->data,stdout,1);

    while(mvnodeptr != NULL)
    {
        printf("in findln --pointer: \n");
        ModFile_Printer(mvnodeptr->data,stdout,1);

        //call comparator function
        int comparterfunctretval = list->comparator(mvnodeptr->data,token);

        printf("**comparatorfunct retval: %d\n",comparterfunctretval);

        //return if comparevalfunctreturn is 0
        if(comparterfunctretval == 0)
        {
            //return pointer to node in LL
            return mvnodeptr;
        }

        //increment mvnodeptr to next
        mvnodeptr = mvnodeptr->next;
    }

    //end case where mvnodeptr is at the end return
    return mvnodeptr;
}

//my function
void recurdestroylnchain(node_t * anode,list_t * alist)
{
    printf("recursive destroy\n");

    if(anode->next != NULL)
    {
        recurdestroylnchain(anode->next,alist);
        
    }
    printf("freeing\n");
    //call deleter
    if(anode->data != NULL)
    {
        alist->deleter(anode->data);
    }
    //free current node
    free(anode);
}

void DestroyList(list_t** list) {
    //given linked list
    list_t * theliststructptr = *(list);
    printf("in the destroy list\n");

    //for case where the head is NULL
    if(theliststructptr->head != NULL)
    {
        printf("head is not NULL\n");
        recurdestroylnchain(theliststructptr->head,theliststructptr);
    }
    

    //when done free current list_t??
    free(theliststructptr);
    //set the list to be null ptr
    *(list) = NULL;

}

void ProcessModFile(FILE* fp, list_t* list, char ordering) {
    //reads from FILE
    printf("IN Process ModFIle: Maxline size: %d\n",MAX_LINESIZE);

    char * linechunk = (char *)malloc(sizeof(char)*(MAX_LINESIZE + 1));

    printf("after init %ld\n",sizeof(char)*(MAX_LINESIZE + 1));

    //test creating new list with ABC comparator
    list_t * tempABCcomplist = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);

    while(fgets(linechunk,(MAX_LINESIZE + 1),fp) != NULL)
    {
        printf("Theline is :\n");
        printf("%s|*\n",linechunk);

        if(myStrCmp(linechunk,"\n") == 0)
        {
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!n DETECTED!!!!!!!!!!!!!!\n");
            break;
        }

        printf("test splitting\n");

        //pointer that moves as we split
        char * linecnkptr = linechunk;

        char * numinserts = myStrCpy(linecnkptr,"\t\n");
        printf("themystrlen %d\n",myStrLen(numinserts)+1);
        linecnkptr += (myStrLen(numinserts)+1);
        printf("the str after move:%s\n",linecnkptr);
        char * numdeletions = myStrCpy(linecnkptr,"\t\n");
        linecnkptr += (myStrLen(numdeletions)+1);
        char * strfilename = myStrCpy(linecnkptr,"\t\n");//account for newline at end


        printf("the insertions: %d\n",atoi(numinserts));

        printf("the deletions: %s\n",numdeletions);
        printf("the filestr: %s\n",strfilename);
        printf("\n");

        //insert node depending on odering

        //create a token for findinlist

        ModFile * tokenmodfilecompare =  PutModFile(atoi(numinserts),atoi(numdeletions),strfilename,NULL);

        node_t * findinlreslt = FindInList(tempABCcomplist,tokenmodfilecompare);

        

        // if(ordering == 'a')
        // {

            
            //option a alphabetical and d decreasing
            if(findinlreslt == NULL)
            {
                if(ordering == 'a' || ordering == 'd')
                {
                //insert in alphabet order for a and d options
                printf("inserting in order--------------------------\n");
                InsertInOrder(tempABCcomplist, (void*)tokenmodfilecompare);
                }
                else
                {
                    //insert in first order
                    printf("inserting in first order--------------------------\n");
                    InsertAtTail(tempABCcomplist, (void*)tokenmodfilecompare);
                }
            }
            else
            {
                //it exists use putmodfile to update it
                printf("updating in order--------------------\n");
                PutModFile(atoi(numinserts),atoi(numdeletions),strfilename,findinlreslt->data);

                //free tokenmodfilecompare only when we know it exists in LL and it is used for comparison only
                ModFile_Deleter(tokenmodfilecompare);
            }
            
            
            
        // }
        // else if(ordering = 'd')
        // {
        //     //ording is d decreasing total deletesandinserts
        //     if(findinlreslt == NULL)
        //     {
        //         //insert in alphabet order
        //         printf("inserting in revorder--------------------------\n");
        //         InsertInReverseOrder(list, (void*)tokenmodfilecompare);
        //     }
        //     else
        //     {
        //         //it exists use putmodfile to update it
        //         printf("updating in revorder--------------------\n");
        //         PutModFile(atoi(numinserts),atoi(numdeletions),strfilename,findinlreslt->data);

        //         //free tokenmodfilecompare only when we know it exists in LL and it is used for comparison only
        //         ModFile_Deleter(tokenmodfilecompare);
        //     }
        // }


        
        //free the pointers after
        free(numinserts);
        free(numdeletions);
        free(strfilename);
    }


    //after things in the list
    printf("The AABCcomplist\n");
    PrintLinkedList(tempABCcomplist,stdout);

    //try to migrate the modfiles to list
    node_t * mvnodetptrTempABCList = tempABCcomplist->head;

    while(mvnodetptrTempABCList != NULL)
    {
        if(ordering == 'a')
        {
            //insert in order
            InsertInOrder(list,(void *)mvnodetptrTempABCList->data);
        }
        else if(ordering == 'd')
        {
            InsertInReverseOrder(list,(void *)mvnodetptrTempABCList->data);
        }
        else if (ordering == 'f')
        {
            //insert at tail
            InsertAtTail(list,(void *)mvnodetptrTempABCList->data);
        }
        


        //set the data ptr in mvnodeptrtemplist to null
        mvnodetptrTempABCList->data = NULL;

        //update mvnodeptrtemplist
        mvnodetptrTempABCList = mvnodetptrTempABCList->next;

    }
    

    //need to destroy afterwards
    DestroyList(&tempABCcomplist);

    //free linechunk at end
    
    free(linechunk);

}


// Part 3 Functions to implement
void AuthorPrinter(void* data, void *fp, int flag) {

}

int AuthorEmailComparator(const void* lhs, const void* rhs)  {
    return -999;
}

int AuthorCommitComparator(const void* lhs, const void* rhs) {
    return -999;
}

void AuthorDeleter(void* data)  {

}

Author* CreateAuthor(char* line, long int *timestamp)  {
    return NULL;
}


// Part 4 Functions to implement
void PrintNLinkedList(list_t* list, FILE* fp, int NUM) {

}
