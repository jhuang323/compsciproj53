// Your helper functions need to be here.
#include "helpers.h"

//global var definition
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
    // printf("the sig term was recieved!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

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
    return -1;

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

int checkerrfilesmatch(proc_info * begprocinfo)
{
    //beg procinfo
    
    for(proc_info * outerprocptr = begprocinfo; outerprocptr != NULL ;outerprocptr = outerprocptr->next_proc)
    {
        //outer forloop err file
        char * outererrfile = outerprocptr->err_file;

        //inner mpving for loop
        for(proc_info * innermvprocptr = outerprocptr->next_proc;innermvprocptr != NULL;innermvprocptr = innermvprocptr->next_proc)
        {
            // printf("comparinfg %s %s\n",outererrfile,innermvprocptr->err_file);
            //check the err files
            if(outererrfile != NULL && (innermvprocptr->err_file) != NULL && strcmp(outererrfile,innermvprocptr->err_file) == 0)
            {
                // printf("the are the same\n");
                //they are the same return 1
                return 1;
            }
        }
    }

    //all the err files are diff after checking all
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

    //check the err files are not the same
    if(checkerrfilesmatch(aprocinfohead) == 1)
    {
        //there is a err file that is the same
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

char * createcustshellprompt(char * auser, char * ahostname, char * apwd)
{
    int lencalc = 15; // for the defaule53shell..\0
   
    //add on the colorlen
    lencalc += strlen(COLORUSERNAME);
	
	lencalc += strlen(auser);

    //add on the resetcolorlen
    lencalc += strlen(COLORRESET);

	// getting the hostname
	
    //add on the colorlen
    lencalc += strlen(COLORHOSTNAME);
    
	lencalc += strlen(ahostname);

    //add on the resetcolorlen
    lencalc += strlen(COLORRESET);

   


	// getting the pwd
	
    //add on the colorlen
    lencalc += strlen(COLORPWD);

	lencalc += strlen(apwd);
	
    //add on the resetcolorlen
    lencalc += strlen(COLORRESET);

    // printf("the size of colors %ld\n",strlen("\x1B[1;31m"));

    //allocate mem for str
    // printf("the calclen %ld\n",lencalc);
	char * thedynstr = malloc(sizeof(char)*lencalc);



    //the concat section
	strcpy(thedynstr,"<");
    //username
    strcat(thedynstr,COLORUSERNAME);
	strcat(thedynstr,auser);
    strcat(thedynstr,COLORRESET);

	strcat(thedynstr,"@");
    //hostname
    strcat(thedynstr,COLORHOSTNAME);
	strcat(thedynstr,ahostname);
    strcat(thedynstr,COLORRESET);

	strcat(thedynstr,":");
    //pwd
    strcat(thedynstr,COLORPWD);
	strcat(thedynstr,apwd);
    strcat(thedynstr,COLORRESET);

    //add end prompt
	strcat(thedynstr," 53shell>$ ");
	// printf("the final str: %s\n",thedynstr);

    return thedynstr;
}

int findrearindx(List_t* list)
{
    node_t * mvnodeptr = list->head;

    int counter = 0;

    while((mvnodeptr->next) != NULL)
    {
        //update counter
        counter++;
        //update mvnodeptr
        mvnodeptr = mvnodeptr->next;

    }
    // printf("rear index is %d\n",counter);

    return counter;
}

//my isdigit
int myIsDigit(int achar)
{
    //check char if it is digit
    if(achar < 48 || achar > 57)
    {
        //return zero due to it being non int
        return 0;
    }

    // is a digit
    return 1;
    
}

//my check if entire string is digit
int myCheckStrIsDigit(const char * str)
{
    //use while loop and myisdigit
    const char * mvcharptr = str;

    while(*mvcharptr != '\0')
    {
        // call isdigit for all chars
        if(myIsDigit(*mvcharptr) == 0)
        {
            //isdigit is not number return 0
            return 0;
        }

        //update mvcharptr
        mvcharptr++;

    }

    //after checking through the str
    return 1;
    

}