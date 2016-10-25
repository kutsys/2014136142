#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

const char *path = "/home/user/Lab06/ptest";

pid_t getPid(pid_t pid, char* arg)
{	
	pid = fork();
	if(pid == -1)		// fork failed
		exit(1);
	if(pid == 0)
		execl(path, arg, NULL, NULL);
	return pid;
}

int main(void)
{
	pid_t pid;
	char message;
	
	pid = fork();
	
	if(pid == -1)
	{
		printf("fork failed\n");
		exit(1);
	}
	else if(pid == 0)
		execl(path, "ptest", NULL, NULL);
	
	else{
		while(1){

			if(pid != 0) printf("\nrunning\n");
			else printf("\nnon existed\n");
			
			printf(">>");
			scanf("%c", &message);
			
			switch(message){
			case 'Q' : case 'q':
				printf("Quit pmon program\n");
				if(pid != 0)
					kill(pid, SIGKILL);
				exit(1);
			case 'K' : case 'k':
				printf("Kill ptest program\n");
				if(pid != 0)
					kill(pid, SIGKILL);
				pid = 0;
				break;
			case 'S' : case 's':
				if(pid > 0)
					printf("Still running\n");
				else{
					printf("ptest execute\n");
					pid = getPid(pid, "ptest");
				}
				break;
			case 'R' : case 'r':
				if(pid > 0)
					printf("Re-start\n");
				else{
					kill(pid, SIGKILL);
					pid = -2;
				}
				pid = getPid(pid, "ptest");
				break;
			}//switch
			sleep(5);
		}// while
	}// default
	exit(0);
}
