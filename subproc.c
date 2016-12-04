
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static int count = 0;
void test_funct(time_t org_time)
{
	pid_t pid;
	struct tm *tm_ptr;
	tm_ptr = gmtime(&org_time);
	printf("PID : %d | ", getpid());
	printf("TIME : %d:%d:%d | ",
		 tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
	printf("Count : %d\n", ++count);
}
int main()
{
	time_t the_time;
	srand(getpid());
	while(1)
	{
		int r = rand()%10;
		
		time(&the_time);
		test_funct(the_time);

		if(count == 20) break;
		sleep(r);
	}
	exit(0);
}
