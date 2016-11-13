#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
bool isRunning;
pid_t pid = -1;
void checkFunc(char* pname){
	FILE *fp;
	char buf[1024];
	memset(buf, '\0', 50);
	fp = popen("pgrep -x ptest", "r");
	if(fp == NULL)
		printf("Failed!\n");
	fgets(buf, 10, fp);
	if(buf[0] != '\0'){
		pid = atoi(buf);
		printf("running\n");
	}
	else{
		pid = -1;
		printf("not existed\n");
	}
	pclose(fp);
}
void Alarm(void)
{
	checkFunc("ptest");
	fflush(stdout);
	alarm(5);
}
int main(void)
{
	char message;
	(void)signal(SIGALRM, (void*)Alarm);
	alarm(5);
	
	while(1){
		scanf("%c", &message);
		switch(message){
		case 'Q' : case 'q':
			checkFunc("ptest");
			if(pid != -1){
				printf("Quit ptest program\n");
				kill(pid, SIGKILL);
				pid = -1;
			}
			printf("Quit pmon program\n");
			exit(1);
		case 'K' : case 'k':
			checkFunc("ptest");
			if(pid != -1){
				printf("Kill ptest program\n");
				kill(pid, SIGKILL);
				pid = -1;
			}
			else
				printf("ptest is not existed\n");	
			break;
		case 'S' : case 's':
			checkFunc("ptest");
			if(pid != -1)
				printf("Still running\n");
			else{
				printf("Now ptest is running\n");
				system("gnome-terminal --command \"./ptest\"");
			}	
			break;
		case 'R' : case 'r':
			checkFunc("ptest");
			if(pid != -1){
				printf("Re-start\n");
				kill(pid, SIGKILL);
				system("gnome-terminal --command \"./ptest\"");
			}
			else{
				printf("Now ptest is running\n");
				system("gnome-terminal --command \"./ptest\"");
			}
			break;
		}//switch
	}// while
	exit(0);
}
