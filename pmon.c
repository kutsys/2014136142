#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

const char *path = "/home/cej/2014136142/ptest";

pid_t getPid(pid_t pid, char* arg)
{	
	pid = fork();
	if(pid == -1)		// fork failed
		exit(1);
	if(pid == 0)
		execl(path, arg, NULL, (char *)0);
	return pid;
}
void printFunc(int pid)
{
	if(pid != 0) printf("running\n");
	else printf("not existed\n");
	alarm(5);			
}
int main(void)
{
	pid_t pid;
	char message;
	pid = fork();
	if(pid == -1){
		printf("fork failed\n");
		exit(1);
	}
	else if(pid == 0)
		execl(path, "ptest", NULL, (char *)0);
	
	else{
		(void) signal(SIGALRM,printFunc);
		alarm(5);
	
		while(1){
			//printf(">>");
			scanf("%c", &message);
			
			
			switch(message){
			case 'Q' : case 'q':
				if(pid != 0){
					printf("Quit ptest program\n");
					kill(pid, SIGKILL);
				}
				printf("Quit pmon program\n");
				exit(1);
			case 'K' : case 'k':
				if(pid != 0){
					printf("Kill ptest program\n");
					kill(pid, SIGKILL);
				}
				pid = 0;
				break;
			case 'S' : case 's':
				if(pid > 0)
					printf("Still running\n");
				else{
					printf("Now ptest is running\n");
					pid = getPid(pid, "ptest");
				}
				break;
			case 'R' : case 'r':
				if(pid <= 0)
					printf("Re-start\n");
				else{
					kill(pid, SIGKILL);
					pid = -2;
				}
				pid = getPid(pid, "ptest");
				break;
			}//switch
			printf("%d\n", pid);
			//sleep(5);
		}// while
	}// default
	exit(0);
}
