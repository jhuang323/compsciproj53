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

    //setting commit count test
    a1->commitCount = 1;
    a2->commitCount = 3;
    a3->commitCount = 1;

    printf("the a1num is %ld\n",a1num);

    InsertAtTail(testlist,a3);
    InsertAtTail(testlist,a1);
    InsertAtTail(testlist,a2);

    PrintLinkedList(testlist,stdout);

    printf("test find in list\n\n");

    //find in list sort test
    mySortFunctmodauth(testlist,&AuthorCommitComparator,'a');

    printf("\n\n\n\nFInalList\n");

    PrintLinkedList(testlist,stdout);

    //test printnLinkedList
    printf("\n\n\n\n printNLinkedList test ---\n");
    PrintNLinkedList(testlist,stdout,-1);
    printf("\n\n\n");

    

    //destroy list afterwards
    DestroyList(&testlist);


    //test my helper digit
    printf("\n\nMyhelper test:\n");
    printf("IS digit return: %d\n",myIsDigit(':'));
    printf("\n\nMyhelper test:\n");
    printf("IS strdigit return: %d\n",myCheckStrIsDigit("1234455a"));

    //test valid date check
    printf("valid date check\n");
    char * astr = "30/1-2/200-2";
    int mnum = 0;
    int dnum = 0;
    int ynum = 0;
    printf("check date return: %d\n",myCheckDate(astr,&mnum,&dnum,&ynum));
    printf("the nums: %d %d %d\n",mnum,dnum,ynum);


    //test comparator
    Author * a11 = CreateAuthor("c09765e,1636949442,Jennifer Wong-Ma,jwongma@uci.edu,dark_mode,Fall 2021 Bombsweeper Edition\n",&a1num);
    Author * a22 = CreateAuthor("c09765e,1636949442,Jennifer Wong-Ma,jwongma@uci.edu,dark_mode,Fall 2021 Bombsweeper Edition\n",&a1num);

    


    


}