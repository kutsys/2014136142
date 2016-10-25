
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void swaptime(time_t, char *);
int main()
{
	char buffer[255];
	time_t the_time;
	
	while(1){
		time(&the_time);
		memset(buffer, 0x00, 255);
		swaptime(the_time, buffer);
 
	  	printf("%s\n", buffer);
		sleep(2);
	}
	exit(0);

}

void swaptime(time_t org_time, char *time_str)
{
    struct tm *tm_ptr;
    tm_ptr = gmtime(&org_time);
    sprintf(time_str, "%d-%d-%d %d:%d:%d", tm_ptr->tm_year+1900,
                    tm_ptr->tm_mon+1,
                    tm_ptr->tm_mday,
                    tm_ptr->tm_hour,
                    tm_ptr->tm_min,
                    tm_ptr->tm_sec);
}



