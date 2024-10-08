#include "linkedlist.h"
#include "hw2.h"
#include "helpers2.h"

int main()
{   
    //test pretty print
    list_t* testlist = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);



    // printf("EMpty List\n");
    // PrintLinkedList(testlist,stdout);

    // //insert 1 intolist
    // printf("list with 1 in it:\n");
    // ModFile* f1 = PutModFile(3,3,"MarsSplashScreen.java", NULL);
    // PutModFile(222,0,"MarsSplashScreen.java", f1);
    // ModFile* f2 = PutModFile(411,0,"tools/ICS51_Minesweeper.java", NULL);
    // ModFile* f3 = PutModFile(1,7,"mars/venus/HelpAboutAction.java", NULL);
    // ModFile* f4 = PutModFile(1,8,"tools/ICS51_Minesweepedr.java", NULL);
    // InsertInReverseOrder(testlist, (void*) f3);
    // InsertInOrder(testlist, (void*) f1);
    // InsertInReverseOrder(testlist, (void*) f2);
    // InsertInReverseOrder(testlist, (void*) f4);
    // PrintLinkedList(testlist,stdout);

    // printf("lenght in testlist: %d\n",testlist->length);


    //testprocessmodfile
    
    FILE* fptr = fopen("rsrc/mytestfile.txt","r");

    // if(fptr == NULL)
    // {
    //     printf("File error\n");
    // }

    ProcessModFile(fptr,testlist,'d');
    printf("the list:\n");
    PrintLinkedList(testlist,stdout);

    

    // printf("\n\n\nTHE debuglist\n");
    // PrintLinkedList(testlist,stdout);
    // printf("length: %d\n",testlist->length);

    //test if we can print new lines after (yes)
    // printf("\n\ntesting\n");
    // char buf[201];
    // fgets(buf,201,fptr);
    // printf("theline:\n%s\n",buf);


    //close file
    fclose(fptr);



    //test find in list
    // node_t * testfindinlistnode = FindInList(testlist,f4);

    // //testing delet LL
    DestroyList(&testlist);

    // //test if testlist is nullptr
    // if(testlist == NULL)
    // {
    //     printf("The testlist at the end is NULL\n");
    // }
    // else
    // {
    //     printf("Test list is not NULL\n");
    // }

    // free(testlist);

    //test delete modfile
    // ModFile_Deleter(f1);

    // //testing ffree for list with only one node that stores MOdfile with filename
    // //free filename
    // free(((ModFile *)(testlist->head->data))->filename);
    // //free data in head
    // free(testlist->head->data);
    // //free head which points to a LN struct
    // free(testlist->head);
    // //finally free testlist struct
    // free(testlist);


    return 0;
}