#include<stdio.h>
#include<stdlib.h>
int main()
{
	FILE *in, *out;
	int nread;
	int count = 0;
	int printDot = 0;
	printf("BUFSIZ : %d\n", BUFSIZ);

	in = fopen("file.in","r");
	if(in == NULL){
		printf("No File in this Directory.\n");
		exit(1);
	}
	out = fopen("file.out","w");

	while((nread = fgetc(in)) != EOF){
		fputc(nread, out);
		count++;
		if(count%BUFSIZ == 0){ printf("."); printDot++; }
	}
	printf("\nCopy is Finished\n");
	printf("DOT : %d", printDot);
	fclose(in);
	fclose(out);
	exit(0);
}
