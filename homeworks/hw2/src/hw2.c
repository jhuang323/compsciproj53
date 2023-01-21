// Your Name
// NetId

#include "hw2.h"
#include "helpers2.h"
#include "linkedlist.h"

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

    // printf("Final Min strlen: %d\n",minstrlen);

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
        //update ins
        mf->inserts = ins;
        //update dels
        mf->deletes = dels;


        //return the mf pointer
        return mf;
    }
}

int ModFileABC_Comparator(const void* file1, const void* file2) {
    // return -999;

    printf("test print filename1 %s\n",((ModFile *)file1)->filename);
    printf("test print filename2 %s\n",((ModFile *)file2)->filename);

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

    

}

void ModFile_Deleter(void* data) {

}

node_t* FindInList(list_t* list, void* token)  {
    return NULL;
}

void DestroyList(list_t** list) {

}

void ProcessModFile(FILE* fp, list_t* list, char ordering) {

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
