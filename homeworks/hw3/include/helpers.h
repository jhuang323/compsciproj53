// A header file for helpers.c
// Declare any additional functions in this file


//test my includes
#include <signal.h>
#include "linkedList.h"
#include "icssh.h"
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <string.h>


#ifndef MYHELPERS11
#define MYHELPERS11


//my defines for color
#define COLORUSERNAME "\x1B[1;32m"
#define COLORHOSTNAME "\x1B[1;35m"
#define COLORPWD "\x1B[2;31m"
#define COLORRESET "\x1B[0m"

//global var declaration
volatile sig_atomic_t Scflag;

volatile sig_atomic_t Susr2flag;

volatile sig_atomic_t Stermflag;


void sigchild_handler(int signum);

void siguser2_handler(int signum);

void sigterm_handler_pipe(int signum);


int BgentryTimeComparator(void* lhs, void* rhs);

List_t* CreateList(int (*compare)(void*,void*));

void printLList(List_t* list, FILE* fp);

int findinLL(List_t* list,pid_t givenpid);

int checkastrandprocinfosame(char * astr,proc_info * starthead);

int checkerrfilesmatch(proc_info * begprocinfo);

int errorcheckfilesgivenvalid(char * theinfile, char * theoutfile,proc_info * aprocinfohead);

void printascii53();

char * createcustshellprompt(char * auser, char * ahostname, char * apwd);

int findrearindx(List_t* list);

//my isdigit
int myIsDigit(int achar);

//my check if entire string is digit
int myCheckStrIsDigit(const char * str);


#endif