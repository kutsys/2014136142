#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
int main()
{
	char c;
	int in, out;
	int printDot = 0;

	in = open("file.in",O_RDONLY);
	if(in == -1){
		printf("No File in this Directory.\n");
		exit(1);
	}
	out = open("file.out",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);

	while(read(in, &c, 1) == 1){
		write(out,&c,1);
		printf(".");
		printDot++;
	}
	printf("\nCopy is Finished.\n");
	printf("\nDOT : %d", printDot);
	exit(0);
}
