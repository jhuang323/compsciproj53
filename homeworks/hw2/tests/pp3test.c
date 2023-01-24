#include "hw2.h"
#include "linkedlist.h"
#include "helpers2.h"

int main()
{
    printf("hello\n");

    list_t * testlist = CreateList(&AuthorEmailComparator,&AuthorPrinter,&AuthorDeleter);

    //test add a node
    long int a1num;
    Author * a1 = CreateAuthor("c09765e,1636949442,Jennifer Wong-Ma,jwongma@uci.edu,dark_mode,Fall 2021 Bombsweeper Edition\n",&a1num);
    Author * a2 = CreateAuthor("1cadd09,1582772238,None,jennifer.wong@uci.edu,,PseudoOps.txt edited to remove all load and store instruciton with label\n",&a1num);
    Author * a3 = CreateAuthor("28280af,1602611648,A. Nguyen,an3@uci.edu,,Updated Syscall help message for Service 8 to display 'is less than' rather than '<' to display properly\n",&a1num);

    printf("the a1num is %ld\n",a1num);

    InsertAtTail(testlist,a1);
    InsertAtTail(testlist,a2);
    InsertAtTail(testlist,a3);

    PrintLinkedList(testlist,stdout);

    printf("test find in list\n\n");

    //find in list sort test
    mySortFunctmodauth(testlist,&AuthorEmailComparator,'a');

    printf("\n\n\n\nFInalList\n");

    PrintLinkedList(testlist,stdout);

    

    //destroy list afterwards
    DestroyList(&testlist);
    


}