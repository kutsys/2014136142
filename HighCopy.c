#include<stdio.h>
#include<stdlib.h>
int main()
{
	int nread;
	FILE *in, *out;
	int count = 0;
	int printDot = 0;

	in = fopen("file.in","r");
	out = fopen("file2.out","w");

	while((nread = fgetc(in)) != EOF){
		fputc(nread, out);
		count++;
		if(count % 1024 == 0){
			printf(".");
			printDot++;
		}
	}

	printf("\nCopy is Finished\n");
	printf("DOT : %d", printDot);
	fclose(in);
	fclose(out);
	exit(0);
}
