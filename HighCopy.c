#include<stdio.h>
#include<stdlib.h>

int main()
{
	int nread;
	FILE *in, *out;

	int count = 0;
	int printDot = 0;

	in = fopen("file.in","r");
	if(in == -1){
		printf("No File in this Directory.\n");
		exit(1);
	}
	out = fopen("file2.out","w");

	while((nread = fgetc(in)) != EOF){
		fputc(nread, out);
		printf(".");
		printDot++;
	}

	printf("\nCopy is Finished\n");
	printf("DOT : %d", printDot);

	fclose(in);
	fclose(out);
	exit(0);
}
