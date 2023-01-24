#include "hw2.h"
#include "linkedlist.h"
#include "helpers2.h"
#include <string.h> // Not allowed in your HW implemention!!

int main() {

    // char* name1 = "Jennifer Wong-Ma";  	       // Static string
    char  name2[] = "Michael Schindler"; 	   // Array without size, edittable
    // char  email1[20] = "jwongma@uci.edu";      // Array with size, uninitialized bytes set to 0 
    char  email2[5] = "mikes@uci.edu";         // Array with size, truncated initialization (no null terminator!) DON'T USE THIS AS A STRING!!!

    // char * name1 = myStrCpy("Jennifer Wong-Ma",NULL);
    // char * email1 = myStrCpy("jwongma@uci.edu",NULL);

    char* email3 = calloc(5,1);     // 5 bytes of space initialized to 0
    strncpy(email3,email2,4);         // String.h functions are not allowed in your HW implemention!! Create your own versions with pointers

    //test by creating a modfile
    // list_t* jwongmalistptr = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);
    // ModFile* f1 = PutModFile(2,1,"MarsSplashScreen.java", NULL);
    // ModFile* f2 = PutModFile(411,0,"tools/ICS51_Minesweeper.java", NULL);
    // ModFile* f3 = PutModFile(1,7,"mars/venus/HelpAboutAction.java", NULL);
    // InsertAtTail(jwongmalistptr, (void*) f1);
    // InsertAtTail(jwongmalistptr, (void*) f2);
    // InsertAtTail(jwongmalistptr, (void*) f3);

    //test bby creating modfile for schindler
    list_t* schindlerlistptr = CreateList(&ModFileABC_Comparator,&ModFile_Printer,&ModFile_Deleter);
    ModFile* f1s = PutModFile(2,1,"MarsSplashScreen.java", NULL);
    ModFile* f2s = PutModFile(411,0,"tools/ICS51_Minesweeper.java", NULL);
    ModFile* f3s = PutModFile(1,7,"mars/venus/HelpAboutAction.java", NULL);
    InsertAtTail(schindlerlistptr, (void*) f1s);
    InsertAtTail(schindlerlistptr, (void*) f2s);
    InsertAtTail(schindlerlistptr, (void*) f3s);

    // Author jwongma = {name1,email1,1, jwongmalistptr};
    Author* temp = malloc(sizeof(Author));
    temp->fullname = name2;
    temp->email = email2;
    temp->commitCount = 20;
    temp->modFileList = schindlerlistptr;//NULL CAse?

    Author* mikes = temp; 

    // printf("\n******AuthorPrinter Tests******\n");    
    // AuthorPrinter(&jwongma, stdout, 1);
    // printf("\n");    
    // AuthorPrinter(mikes, stderr, 1);

    // printf("\n******AuthorEmailComparator Tests******\n");    
    // int success = AuthorEmailComparator(&jwongma, mikes);

    // if(success < 0)
    //     printf("%s is before %s\n", jwongma.email, mikes->email);
    // else if (success > 0)
    //     printf("%s is after %s\n", jwongma.email, mikes->email);
    // else
    //     printf("%s is equal to %s\n", jwongma.email, mikes->email);
    // // Why is Shindler's email address FUNNY?

    // printf("\n******AuthorCommitComparator Tests******\n");    
    // success = AuthorCommitComparator(&jwongma, mikes);

    // if(success < 0)
    //     printf("%d (%s) is before %d (%s)\n", jwongma.commitCount, jwongma.fullname,  mikes->commitCount, mikes->email);
    // else if (success > 0)
    //     printf("%d (%s) is after %d (%s)\n", jwongma.commitCount, jwongma.fullname, mikes->commitCount, mikes->email);
    // else
    //     printf("%d (%s) is equal to %d (%s)\n", jwongma.commitCount, jwongma.fullname, mikes->commitCount, mikes->email);


    printf("\n\n******AuthorDeleter Test******\n");
    temp->email = email3;
    temp->fullname = malloc(strlen(name2)); // String.h functions are not allowed in your HW implemention!! Create your own versions with pointers
    strcpy(temp->fullname,"Michael Schindler");
    //Leaving ModFileList Empty, but you should delete all memory associated with list too

    AuthorDeleter(mikes);
    if(mikes == NULL)
       printf("How did you do that?!?!");
    else
       mikes = NULL;
    //What about temp? Do you need to delete it?

    // What happens if you try to delete name1 or name2? Why? Does it impact anything?
    // AuthorDeleter(&jwongma);
   //  AuthorDeleter(mikes);
    
    printf("\n\n******createAuthor Tests******\n");
    long int theTimestamp = -1;
    Author* author1 = CreateAuthor("c09765e,1636949442,Jennifer Wong-Ma,jwongma@uci.edu,Fall 2021 Bombsweeper Edition\n", &theTimestamp);
    printf("\n***c09765e,1636949442,Jennifer Wong-Ma,jwongma@uci.edu,Fall 2021 Bombsweeper Editiony***\n");
    AuthorPrinter(author1, stdout, 0);

    return 0;
}
