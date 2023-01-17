// Declare all helper functions for hw1 in this file git v2
# ifndef HW1_HELPERS
# define HW1_HELPERS

void printtest();

int checkreqoptions(int argc,char *argv[]);

//function for -n option
int functnoption();

//function for -l option
int functloption(int argc,char *argv[]);

//function for -h option
int functhoption(int argc,char *argv[]);

//funct for realloc
// void addandrealloc(char ** teststr,int charinteststrcount,int tststrsize,char thecharr);

# endif