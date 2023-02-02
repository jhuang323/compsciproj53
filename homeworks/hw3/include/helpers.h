// A header file for helpers.c
// Declare any additional functions in this file

//test my includes
#include <signal.h>
#include "linkedList.h"

void sigkill_handler(int signum)
{
    //
    printf("In side the sigkill handler\n");

    pid_t pid;


}

int AuthorCommitComparator(void* lhs, void* rhs);

List_t* CreateList(int (*compare)(void*,void*))
{
    //creating a new list
    printf("creating new list\n");

    List_t* list = malloc(sizeof(List_t));
    list->comparator = NULL;
    list->length = 0;
    list->head = NULL;
    return list;

}

void printLList(List_t* list, FILE* fp)
{
    if(list == NULL)
        return;

    printf("in the printing function \n");


    int counter = 0;
    node_t* head = list->head;
    while (head != NULL) {
        // list->printer(head->data, fp, 0);
        //call the debug_print_job
        counter++;
        printf("Debug In LL NUM: %d\n",counter);
        debug_print_job((job_info *) head->value);
        fprintf(fp, "\n");
        head = head->next;
    }
}
