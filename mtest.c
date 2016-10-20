#include<stdio.h>
#include<stdlib.h>
extern int InFoo(), InBoo(), InBar();
int main()
{	
	InFoo();
	InBoo();
	InBar();
	printf("And this is Main Function.\n");
	printf("Hello World!\n");
	exit(0);
}
