#include "linkedlist.h"
#include "hw2.h"

int main()
{   
    //test pretty print
    list_t* testlist = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);



    printf("EMpty List\n");
    PrintLinkedList(testlist,stdout);

    //insert 1 intolist
    printf("list with 1 in it:\n");
    ModFile* f1 = PutModFile(2,1,"MarsSplashScreen.java", NULL);
    InsertAtHead(testlist, (void*) f1);
    PrintLinkedList(testlist,stdout);

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