// Justin K Huang
// huangjk2



#include "hw0.h"

int main (int argc, char *argv[])
{

	//Comment/Delete this print statement and insert your hw0 code here 
//	printf("Hello ICS53 student!\n"); 
	
//	printf("%s \n",argv[0]);

/*	shar * tetstr = "hello"; test
	int num = 3;
*/
	for(int i = argc-1;i>=0;i--)
	{
//		printf("val %d\n",i);
		printArg(argv[i],i);
	}

//	printArg(tetstr,num);

	return 0;
}

//Function to print out a single arugment to the screen
void printArg(char * arg_str, int pos){

	//Insert your code here
	
	printf("Argument[%d]: %s\n",pos,arg_str);

	return;
}
