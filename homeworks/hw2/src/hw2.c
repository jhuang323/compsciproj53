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

    // printf("test print filename1: %s\n",((ModFile *)file1)->filename);
    // printf("test print filename2: %s\n",((ModFile *)file2)->filename);

    int comparereslt = myStrCmp(((ModFile *)file1)->filename,((ModFile *)file2)->filename);
    // printf("the comparison: %d\n",comparereslt);

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

    // printf("total file1: %d\n",file1totalinsdel);
    // printf("total file2: %d\n",file2totalinsdel);

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

    //check of data is NUll
    if(data == NULL)
    {
        return;
    }

    ModFile * mdfileptr = data;
    //free string in Modfile
    free(mdfileptr->filename);
    //then free the modfile struct ??? no
    // free(mdfileptr);

}

node_t* FindInList(list_t* list, void* token)  {
    // return NULL;
    // printf("In findin list function\n");

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
        // printf("in findln --pointer: \n");
        ModFile_Printer(mvnodeptr->data,stdout,1);

        //call comparator function
        int comparterfunctretval = list->comparator(mvnodeptr->data,token);

        // printf("**comparatorfunct retval: %d\n",comparterfunctretval);

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

//-----------test my helper functions
// //insertinrevorder function test
// void InsertInReverseOrder(list_t* list, void* val_ref) {
//     if(list == NULL || val_ref == NULL)
//         return;
//     if (list->length == 0) {
//         InsertAtHead(list, val_ref);
//         return;
//     }

//     node_t** head = &(list->head);
//     node_t* new_node;
//     new_node = malloc(sizeof(node_t));
//     new_node->data = val_ref;
//     new_node->next = NULL;

//     if (list->comparator(new_node->data, (*head)->data) > 0) {
//         new_node->next = *head;
//         *head = new_node;
//     } else if ((*head)->next == NULL) {
//         (*head)->next = new_node;
//     } else {
//         node_t* prev = *head;
//         node_t* current = prev->next;
//         while (current != NULL) {
//             if (list->comparator(new_node->data, current->data) < 0) {
//                 if (current->next != NULL) {
//                     prev = current;
//                     current = current->next;
//                 } else {
//                     current->next = new_node;
//                     break;
//                 }
//             } else {
//                 prev->next = new_node;
//                 new_node->next = current;
//                 break;
//             }
//         }
//     }
//     list->length++;
// }

//my function
// void recurdestroylnchain(node_t * anode,list_t * alist)
// {
//     printf("recursive destroy\n");

//     if(anode->next != NULL)
//     {
//         recurdestroylnchain(anode->next,alist);
        
//     }
//     printf("freeing\n");
//     //call deleter
//     if(anode->data != NULL)
//     {
//         alist->deleter(anode->data);
//         //free current anodeindata whatever struct itself
//         free(anode->data);
//     }
//     //free current node
//     free(anode);
// }
//end test ---------------------------

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

    //check if list is null
    if(list  == NULL)
    {
        return;
    }

    //reads from FILE
    printf("IN Process ModFIle: Maxline size: %d\n",MAX_LINESIZE);

    char * linechunk = (char *)malloc(sizeof(char)*(MAX_LINESIZE + 1));

    printf("after init %ld\n",sizeof(char)*(MAX_LINESIZE + 1));

    //test creating new list with ABC comparator
        // list_t * tempABCcomplist = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);

        // //migrate modfiles in list to tempABC
        // tempABCcomplist->head = list->head;
        // //set list head to nullptr
        // list->head = NULL;
        // //set tempABCcomplist length to list length
        // tempABCcomplist->length = list->length;
        // //set listlength to 0
        // list->length = 0;

    //Author pointer
    // Author * curcommitauthor = NULL;

    //the timestamp
    // long int theimestamp;

    while(fgets(linechunk,(MAX_LINESIZE + 1),fp) != NULL)
    {

        // printf("Theline is :\n");
        // printf("%s|*\n",linechunk);

        if(myStrCmp(linechunk,"\n") == 0)
        {
            // printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!n DETECTED!!!!!!!!!!!!!!\n");
            break;
        }

        // //check for author lines
        if(myStrChar(linechunk,'\t') != NULL)
        {
            // printf("test splitting\n");

            //pointer that moves as we split
            char * linecnkptr = linechunk;

            char * numinserts = myStrCpy(linecnkptr,"\t\n");
            // printf("themystrlen %d\n",myStrLen(numinserts)+1);
            linecnkptr += (myStrLen(numinserts)+1);
            // printf("the str after move:%s\n",linecnkptr);
            char * numdeletions = myStrCpy(linecnkptr,"\t\n");
            linecnkptr += (myStrLen(numdeletions)+1);
            char * strfilename = myStrCpy(linecnkptr,"\t\n");//account for newline at end


            // printf("the insertions: %d\n",atoi(numinserts));

            // printf("the deletions: %s\n",numdeletions);
            // printf("the filestr: %s\n",strfilename);
            // printf("\n");

            //insert node depending on odering

            //create a token for findinlist

            ModFile * tokenmodfilecompare =  PutModFile(atoi(numinserts),atoi(numdeletions),strfilename,NULL);

            node_t * findinlreslt = NULL;
            findinlreslt = FindInList(list,tokenmodfilecompare);

            

            // if(ordering == 'a')
            // {

                
                //option a alphabetical and d decreasing
                if(findinlreslt == NULL)
                {
                    if(ordering == 'a')
                    {
                        //insert in alphabet order for a and d options
                        printf("inserting in order--------------------------\n");
                        InsertInOrder(list, (void*)tokenmodfilecompare);
                    }
                    else
                    {
                        //insert in first order
                        printf("inserting in first order--------------------------\n");
                        InsertAtTail(list, (void*)tokenmodfilecompare);
                    }
                }
                else
                {
                    //it exists use putmodfile to update it
                    printf("updating in order--------------------\n");
                    PutModFile(atoi(numinserts),atoi(numdeletions),strfilename,findinlreslt->data);

                    //free tokenmodfilecompare only when we know it exists in LL and it is used for comparison only
                    ModFile_Deleter(tokenmodfilecompare);
                    //delete the struct Modfile itself
                    free(tokenmodfilecompare);

                }

                //debug print
                printf("\n\n\n DEBUG \n");
                PrintLinkedList(list,stdout);
                
                
                
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
        
        

        
    }


    // //after things in the list
    // printf("The AABCcomplist\n");
    // PrintLinkedList(tempABCcomplist,stdout);

    // //try to migrate the modfiles to list
    // node_t * mvnodetptrTempABCList = tempABCcomplist->head;

    // while(mvnodetptrTempABCList != NULL)
    // {
    //     if(ordering == 'a')
    //     {
    //         //insert in order
    //         InsertInOrder(list,(void *)mvnodetptrTempABCList->data);
    //     }
    //     else if(ordering == 'd')
    //     {
    //         InsertInReverseOrder(list,(void *)mvnodetptrTempABCList->data);
    //     }
    //     else if (ordering == 'f')
    //     {
    //         //insert at tail
    //         InsertAtTail(list,(void *)mvnodetptrTempABCList->data);
    //     }
        


    //     //set the data ptr in mvnodeptrtemplist to null
    //     mvnodetptrTempABCList->data = NULL;

    //     //update mvnodeptrtemplist
    //     mvnodetptrTempABCList = mvnodetptrTempABCList->next;

    // }
    

    // //need to destroy afterwards
    // DestroyList(&tempABCcomplist);

    //free linechunk at end
    
    free(linechunk);

}


// Part 3 Functions to implement
void AuthorPrinter(void* data, void *fp, int flag) {
    //check if data is null
    //NOTeE !!! CHECK FOR NULL in Author's modfileList???
    if(data == NULL)
    {
        return;
    }

    //check flag
    if(flag == 0)
    {
        //flag is zero
        fprintf((FILE*)fp,"%s <%s>,%d,%d\n",((Author*)data)->fullname,((Author*)data)->email,((Author*)data)->commitCount,((Author*)data)->modFileList->length);

    }
    else
    {
        //flag is non zero
        //extended print
        fprintf((FILE*)fp,"%s <%s>,%d\n",((Author*)data)->fullname,((Author*)data)->email,((Author*)data)->commitCount);
        //call the printfunction of Modlist

        //need while loop to go through the nodes
        node_t * mvptrinauthormdfilelist = ((Author*)data)->modFileList->head;

        // printf("tesst: %s\n",((Author*)data)->modFileList->head->)

        //the case where the head is null

        while(mvptrinauthormdfilelist != NULL)
        {
            //print tab before
            fprintf((FILE*)fp,"\t");
            //print a node
            ((Author*)data)->modFileList->printer((void *)mvptrinauthormdfilelist->data,fp,1);
            //update ptr
            mvptrinauthormdfilelist = mvptrinauthormdfilelist->next;
        }

        

    }
}

int AuthorEmailComparator(const void* lhs, const void* rhs)  {
    // return -999;

    //check if either arg is NULL
    if(lhs == NULL || rhs == NULL)
    {
        //either arg is NULL
        return 0;
    }

    Author * lhsauthorst = (Author *)lhs;
    Author * rhsauthorst = (Author *)rhs;

    //check for NULL case
    char * lhsauthemail = lhsauthorst->email;
    char * rhsauthemail = rhsauthorst->email;



    if(lhsauthemail == NULL)
    {
        //null case
        return 0;
    }

    if( rhsauthemail == NULL)
    {
        //null case
        return 0;
    }

    //call mystrcmp
    int mystrcmpres = myStrCmp(lhsauthemail,rhsauthemail);
    printf("The comparison value: %d\n",mystrcmpres);

    //determine return val
    if(mystrcmpres < 0)
    {
        //return -1
        return -1;
    }
    else if(mystrcmpres > 0)
    {
        //return 1
        return 1;
    }
    else
    {
        //they are equal
        return 0;
    }

    
}

int AuthorCommitComparator(const void* lhs, const void* rhs) {
    //check if either arg is NULL
    if(lhs == NULL || rhs == NULL)
    {
        //either arg is NULL
        return 0;
    }

    Author * lhsauthorst = (Author *)lhs;
    Author * rhsauthorst = (Author *)rhs;

    int lhscount = lhsauthorst->commitCount;
    int rhscount = rhsauthorst->commitCount;

    //compare in decending order
    if(lhscount > rhscount)
    {
        //return -1
        return -1;
    }
    else if(lhscount < rhscount)
    {
        //return 1
        return 1;
    }
    else
    {
        //equal 
        //using the authoremail comparator
        // return 0 ;
        int retvalauthoremailcomp = AuthorEmailComparator(lhs,rhs);
        printf("the authoremail comp return: %d\n",retvalauthoremailcomp);
        //dummy
        return retvalauthoremailcomp;
    }
}

void AuthorDeleter(void* data)  {
    //if data is null then return
    printf("IN authro deletor\n");
    if(data == NULL)
    {
        return;
    }
    printf("Begin author deletor\n");

    //free dynamically allocated fullname and email
    Author * authorstrptr = (Author *)data;
    //check null
    if(authorstrptr->fullname != NULL)
    {
        free(authorstrptr->fullname);
    }
    if(authorstrptr->email != NULL)
    {
        free(authorstrptr->email);
    }

    //free modfilelist
    if(authorstrptr->modFileList != NULL)
    {
        //free by calling destroy list
        DestroyList(&(authorstrptr->modFileList));
    }

    // finally free the struct it self?? no??
    // free(authorstrptr);
    
}

Author* CreateAuthor(char* line, long int *timestamp)  {
    //check if either is NUll
    if(line == NULL || timestamp == NULL)
    {
        return NULL;
    }

    //get values
    char * mvlineptr = line;
    //get commit hash
    char * commithashstr = myStrCpy(mvlineptr,",\n");
    
    mvlineptr += myStrLen(commithashstr) + 1;

    //get timestamp
    char * timestampstr = myStrCpy(mvlineptr,",\n");
    mvlineptr += myStrLen(timestampstr) + 1;

    //get fullname
    char * fullnamestr = myStrCpy(mvlineptr,",\n");
    mvlineptr += myStrLen(fullnamestr) + 1;

    //get email
    char * emailstr = myStrCpy(mvlineptr,",\n");
    mvlineptr += myStrLen(emailstr) + 1;

    printf("commithash: %s timestamp: %s fullname: %s email: %s\n",commithashstr,timestampstr,fullnamestr,emailstr);

    //check if commithash, email, fullname , timestamp is empty
    if(*commithashstr == '\0' || *timestampstr == '\0' || *fullnamestr == '\0' || *emailstr == '\0')
    {
        //need to fix by deleting
        free(commithashstr);
        free(timestampstr);
        free(fullnamestr);
        free(emailstr);

        printf("commit hash empty;\n");
        return NULL;
    }

    //check timestamp is 0 or positice
    if(atoi(timestampstr) < 0)
    {
        printf("Time stamp error\n");
        return NULL;
    }

    //store timestamp
    *timestamp = atoi(timestampstr);
    //free timestampstr
    free(timestampstr);

    //free commithashstr not used
    free(commithashstr);

    //create author
    Author * retauthorptr = (Author *)malloc(sizeof(Author));

    //set fullname
    retauthorptr->fullname = fullnamestr;
    //set email
    retauthorptr->email = emailstr;
    //set commitcount
    retauthorptr->commitCount = 1;
    
    //createmodfile list to store
    //default abc comparator
    retauthorptr->modFileList = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);

    //return the ptr
    return retauthorptr;
    

}


// Part 4 Functions to implement
void PrintNLinkedList(list_t* list, FILE* fp, int NUM) {
    //check nullptr
    if(list == NULL || fp == NULL)
    {
        return;
    }
    //check num < 0
    if(NUM < 0)
    {
        return;
    }

    node_t * mvnodeptr = list->head;

    if (NUM == 0)
    {
        while(mvnodeptr != NULL)
        {
            //print a node
            list->printer((void *)mvnodeptr->data,(void *)fp,1);

            //update the node pointer
            mvnodeptr = mvnodeptr->next;
        }

    }
    else
    {
        //non zero case
        for(int i = 0;i < NUM;i++)
        {
            if(mvnodeptr == NULL)
            {
                return;
            }
            //after null check print
            list->printer((void *)mvnodeptr->data,(void *)fp,1);

            //update mvnodeptr
            mvnodeptr = mvnodeptr->next;
            
        }
    }

    

}

//my sort function
void mySortFunctmodauth(list_t* givenlist,int (*comparefunct)(const void*, const void*),char modauthoption)
{
    //sorting after wards final list is sorted in 
    //create new list

    list_t * templistholdsort = NULL;

    if(modauthoption == 'm')
    {
        templistholdsort = CreateList(comparefunct,&ModFile_Printer,&ModFile_Deleter);
    }
    else
    {
        templistholdsort = CreateList(comparefunct,&AuthorPrinter,&AuthorDeleter);
    }


    //sort the list with the given comparator by inserting
    node_t * mvnodeptr = givenlist->head;

    while(mvnodeptr != NULL)
    {
        printf("Sorting Node!\n");

        

        InsertInOrder(templistholdsort,(void *)mvnodeptr->data);
        //set the data ptr to null
        mvnodeptr->data = NULL;

        //update mvnodeptr
        mvnodeptr = mvnodeptr->next;
    }

    //print the sorted list
    printf("the sorrted list\n");
    PrintLinkedList(templistholdsort,stdout);

    //swap the heads
    node_t * tempheadstor = givenlist->head;
    //move sortedhead to givenlist
    givenlist->head = templistholdsort->head;
    //set templist head to stored
    templistholdsort->head = tempheadstor;

    //destroy templistholdsort
    DestroyList(&templistholdsort);
    
}

