// A header file for helpers.c
// Declare any additional functions in this file

//test my includes
#include <signal.h>
#include "linkedList.h"
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
// #include <csapp.h>

#ifndef MYHELPERS11
#define MYHELPERS11

//global var
volatile sig_atomic_t Scflag = false;

volatile sig_atomic_t Susr2flag = false;

volatile sig_atomic_t Stermflag = false;

void sigchild_handler(int signum)
{
    //store errono
    int olderrno = errno;
    // printf("In side the sigkill handler\n");

    //set sigchild flag to true
    Scflag = true;

    // pid_t pidd;

    // while(waitpid((pid_t)(-1),0,WNOHANG) > 0) {}

    // printf("pid : %d\n",pidd);

    //restore errno
    errno = olderrno;


}

void siguser2_handler(int signum)
{
    //store errono
    int olderrno = errno;
    // write("In the siguser2 %d\n");
    // Susr2flag = true;

    int thepid = getpid();
    
    char buffer[100];

    snprintf(buffer,100,"Hi User! I am process %d\n",thepid);

    // Convert 123 to string [buf]
    // itoa(num, snum, 10);
    
    // printf("printing in usguser2\n");
     
    // char * achar = NULL;
    // strcpy(achar, "a test string");
    // strcat(achar,"123");

    // printf("the strlen %d\n",strlen(buffer));

    write(STDOUT_FILENO,buffer,strlen(buffer));
    // char x = 55;
    // write(1, &x, 1);


    // printf("Hi User! I am process %d\n",getpid());

    // write(STDOUT_FILENO,"Child tererm\n",13);
    // Sio_puts("hello\n");
    //restore errno
    errno = olderrno;
}

void sigterm_handler_pipe(int signum)
{
    //print test
    printf("the sig term was recieved!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

    Stermflag = true;

    //kill the process
    
}

int BgentryTimeComparator(void* lhs, void* rhs)
{
    // printf("in bg time compare\n");
    time_t lhstime = ((bgentry_t * )lhs)->seconds;
    time_t rhstime = ((bgentry_t * )rhs)->seconds;

    // printf("lhstime %ld rhstime %ld\n",lhstime,rhstime);

    if(lhstime < rhstime)
    {
        return -1;
    }
    else if(lhstime > rhstime)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

List_t* CreateList(int (*compare)(void*,void*))
{
    //creating a new list
    // printf("creating new list\n");

    List_t* list = malloc(sizeof(List_t));
    list->comparator = compare;
    list->length = 0;
    list->head = NULL;
    return list;

}

void printLList(List_t* list, FILE* fp)
{
    if(list == NULL)
        return;

    // printf("in the printing function \n");


    int counter = 0;
    node_t* head = list->head;
    while (head != NULL) {
        // list->printer(head->data, fp, 0);
        //call the debug_print_job
        counter++;
        // printf("Debug In LL NUM: %d\n",counter);
        print_bgentry((bgentry_t *) head->value);
        fprintf(fp, "\n");
        head = head->next;
    }
}

int findinLL(List_t* list,pid_t givenpid)
{
    
    node_t * mvnodeptr = list->head;

    if(mvnodeptr == NULL)
    {
        return -1;
    }

    int counter = 0;
    while(mvnodeptr != NULL)
    {
        //check the pid
        bgentry_t * thebgentryptr = (bgentry_t *)mvnodeptr->value;
        // printf("checking %d\n",thebgentryptr->pid);

        if((thebgentryptr->pid) == givenpid)
        {
            return counter;
        }

        //update ptr
        mvnodeptr = mvnodeptr->next;
        //update counter
        counter++;
    }

}

int checkastrandprocinfosame(char * astr,proc_info * starthead)
{
    //check if astr is null
    if(astr == NULL)
    {
        return 0;
    }

    //for loop to go through the proc_info
    proc_info * mvprocptr = starthead;

    while(mvprocptr != NULL)
    {
        //check if the same
        if((mvprocptr->err_file) != NULL && strcmp(astr,mvprocptr->err_file) == 0)
        {
            //they are the same
            return 1;
        }

        //update ptr
        mvprocptr = mvprocptr->next_proc;
    }

    //after checking all of it not same
    return 0;
}

int errorcheckfilesgivenvalid(char * theinfile, char * theoutfile,proc_info * aprocinfohead)
{
    //check infile against outfile
    if(theinfile != NULL && theoutfile != NULL)
    {
        if(strcmp(theinfile,theoutfile) == 0)
        {
            //error they are the same
            return 0;

        }
    }

    //check outfile and procinfo
    if(checkastrandprocinfosame(theoutfile,aprocinfohead) == 1)
    {
        //there is a same
        return 0;
    }

    //check infile and procinfo
    if(checkastrandprocinfosame(theinfile,aprocinfohead) == 1)
    {
        //there is a same
        return 0;
    }


    //after all checks
    return 1;



}

void printascii53()
{
    //print acscii in ten lines
    printf("##==##    ####     ####\n"); //1
    printf("##==##   ##==##    =##=\n"); //2
    printf("##==##   ##====    =##=\n");//3
    printf("##==##   ##====    =##=\n");//4
    printf("##==##   ##====    =##=\n");//5
    printf("##==##   ##====    =##=\n");//6
    printf("##==##   ##====    =##=\n");//7
    printf("##==##   ##====    =##=\n");//8
    printf("##==##   ##==##    =##=\n");//9
    printf(" ####    =####=    ####\n");//10
}



#endif