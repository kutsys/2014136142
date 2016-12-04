#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

const char *path = "/home/user/2014136142/subproc";

int main(void)
{
	int i;	
	pid_t pid;
	int pArr[10];

	printf("Num : 2014136142\n");
	printf("Name : 최은진\n");

	for(i=0; i<10; i++)
	{
		pid = fork();
		if(pid == -1){
			printf("fork failed\n");
			exit(1);
		}
		else if(pid == 0){
			execl(path, "subproc", NULL, (char *)0);  
		}
		else{	
			pArr[i] = pid;	
		}
	}
	// 호출 완료
	
	int q;
	for(i=0; i<10; i++)
	{
		wait(&pArr[q]);
	}	
	printf("Finished (proces)\n");
	exit(0);
}

