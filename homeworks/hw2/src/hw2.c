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
    return NULL;
}

int ModFileABC_Comparator(const void* file1, const void* file2) {
    return -999;
}

int ModFileTotal_Comparator(const void* file1, const void* file2) {
    return -999;
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
