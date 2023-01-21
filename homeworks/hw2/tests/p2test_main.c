#include "linkedlist.h"
#include "hw2.h"

int main()
{   
    //test pretty print
    list_t* testlist = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);



    printf("EMpty List\n");
    PrintLinkedList(testlist,stdout);

    // //insert 1 intolist
    printf("list with 1 in it:\n");
    ModFile* f1 = PutModFile(2,1,"MarsSplashScreen.java", NULL);
    ModFile* f2 = PutModFile(411,0,"tools/ICS51_Minesweeper.java", NULL);
    ModFile* f3 = PutModFile(1,7,"mars/venus/HelpAboutAction.java", NULL);
    InsertInOrder(testlist, (void*) f3);
    InsertInOrder(testlist, (void*) f2);
    InsertInOrder(testlist, (void*) f1);
    PrintLinkedList(testlist,stdout);

    printf("lenght in testlist: %d\n",testlist->length);

    //testing delet LL
    DestroyList(&testlist);

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