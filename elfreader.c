#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUF_SIZE 11

void print_elf_h(char* argv)
{
	char temp[BUF_SIZE];
	sprintf(temp,"readelf -h %s",argv);
	system(temp);

}

void print_program_h(char* argv)
{
	char temp[BUF_SIZE];
	sprintf(temp,"readelf -l %s",argv);
	system(temp);
}

void print_section_h(char* argv)
{
	char temp[BUF_SIZE];
	sprintf(temp,"readelf -S %s",argv);
	system(temp);
}

void checkFile(char* argv)
{
	if(access(argv,F_OK)==-1)
	{
		printf("file is not existed\n");
		exit(1);
    	}
}

void checkOption(char* argv1,char* argv2)
{
	if(strstr(argv1,"e") != NULL)
   	{
		print_elf_h(argv2);
	}

   	if(strstr(argv1,"p") != NULL)
	{
		print_program_h(argv2);
 	}

    	if(strstr(argv1,"s") != NULL)
    	{
		print_section_h(argv2);
    	}

}

int main(int argc,char* argv[])
{

	switch(argc)
	{
	case 2:
		checkFile(argv[1]);
		print_elf_h(argv[1]);		
		break;
	case 3:
		checkFile(argv[2]);
		checkOption(argv[1],argv[2]);
		break;
	default:
		printf("please name the elffile\n");
		break;
	}
	return 0;
}