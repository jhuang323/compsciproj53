//Justin K Huang
//huangjk2

#include <stdio.h>
#include <string.h>
int main()
{
	char * mynetid = "huangjk2";
	int valclass = 53;

	printf("Hello, %s! Welcome to ICS%d!\n",mynetid,valclass);


	int lenstr = strlen(mynetid);

	printf("len:%d\n",lenstr);

	for(int i = lenstr; i >= 0; i--)
	{
		printf("%d\n",i);
	}

	return 0;
}
