#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);
int main(void)
{
	int i;	

	printf("Num : 2014136142\n");
	printf("Name : 최은진\n");

	int res;
	pthread_t a_thread[10];
	void *thread_result;	// 쓰레드 함수의 리턴값을 받기 위한 포인터

	for(i = 0; i<10; i++){
		res = pthread_create(&a_thread[i], NULL, thread_function, (void *)&i);
		// 쓰레드 생성
		if(res != 0)	// 실패시
		{
			perror("Thread creation failed");
			exit(EXIT_FAILURE);	
		}
		sleep(1);
	}
	
	for(i = 9; i>=0; i--)
	{
		res = pthread_join(a_thread[i], &thread_result);
		if(res != 0)		// 조인 실패
		{
			perror("Thread join failed\n");
			exit(EXIT_FAILURE);
		}
	}
	printf("Fisihed (Thread)\n");
	exit(EXIT_SUCCESS);	
}

void *thread_function(void *arg)
{
	time_t the_time;
	int id = *((int *)arg);
	srand(time(NULL));
	int count = 0;
	while(1)
	{
		int r = rand()%10;
		time(&the_time);
		struct tm *tm_ptr;
		tm_ptr = gmtime(&the_time);
		printf("TID : %2d | ", id+1);
		printf("TIME : %d:%d:%2d | ", 
			 tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		printf("Count : %d\n", ++count);
		if(count == 20)
		{
			printf("Thread %2d is Finished\n", id+1);
			break;
		}
		sleep(r);
	}
}		
