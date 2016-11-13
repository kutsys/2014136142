#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

bool isRunning;
char gpid[10];

void checkFunc(int sig){
	if(sig == SIGALRM){
		FILE *fp = NULL;
		if((fp = popen("pgrep -x ptest", "r")) == NULL)
			printf("Failed!\n");
		if(fgets(gpid, 10, fp) != NULL){
			printf("PID : %s", gpid);
			isRunning = true;
			printf("running\n");
		}
		else
			printf("not existed\n");
		pclose(fp);
	}
	alarm(5);
}

int main(void)
{
	isRunning = false;
	char message;
	(void)signal(SIGALRM, checkFunc);
	alarm(5);
	
	while(1){
		scanf("%c", &message);
		switch(message){
		case 'Q' : case 'q':
			if(isRunning){
				printf("Kill ptest program\n");
				kill(atoi(gpid), SIGKILL);
				isRunning = false;
			}
			printf("Quit pmon program\n");
			exit(1);
		case 'K' : case 'k':
			if(isRunning){
				printf("Kill ptest program\n");
				kill(atoi(gpid), SIGKILL);
			}
			else
				printf("ptest is not existed\n");	
			isRunning = false;
			break;
		case 'S' : case 's':
			if(isRunning)
				printf("Still running\n");
			else{
				printf("Now ptest is running\n");
				system("gnome-terminal --command \"./ptest\"");
				isRunning = true;
			}	
			break;
		case 'R' : case 'r':
			if(isRunning){
				printf("Re-start\n");
				kill(atoi(gpid), SIGKILL);
				system("gnome-terminal --command \"./ptest\"");
			}
			else{
				printf("Still running\n");
				system("gnome-terminal --command \"./ptest\"");
			}
			isRunning = true;
			break;
		}//switch
	}// while
	exit(0);
}
