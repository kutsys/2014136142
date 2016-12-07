#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_function(void *arg);
sem_t bin_sem;

int a_tid[10];
int min = 1000000;

int main()
{
	int res;
	pthread_t a_thread[10];
	void *thread_result;
	int i;

	res = sem_init(&bin_sem, 0, 0);
	// 세마포어의 초기 값을 0으로 설정한다.
	// sem이 가리키는 세마포어 객체를 초기화하고, 
	// 공유 옵션을 설정하고 세마포어 객체에 초기 정수값을 부여
	if(res != 0){
		perror("Semaphore initialization failed");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<10; i++)
	{
		res = pthread_create(&a_thread[i], NULL, 
					thread_function, (void *)&i);
		a_tid[i] = 0;
		if(res != 0){
			perror("Thread creation failed");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
	// 메인 함수에서 새 쓰레드를 시작
	
	sem_post(&bin_sem);	// 세마포어 값을 1 증가

	for(i=9; i>=0; i--)
	{
		res = pthread_join(a_thread[i], &thread_result);
		// 쓰레드 종료를 기다림
		if(res != 0){
			perror("Thread join failed");
		}
	}

	printf("Finished Successfully\n");
	sem_destroy(&bin_sem);
	// 세마포어 관련된 리소스를 정리 후 삭제.
	// 세마포어 대기상태 쓰레드가 있을 경우, 에러값을 리턴
	exit(EXIT_SUCCESS);
}
int getMin()
{
	int i;
	int min = a_tid[0];
	for(i=1; i<10; i++)
	{
		if(a_tid[i] < min) 
			min = a_tid[i];
	}
	return min;
}
void *thread_function(void *arg)
{
	int id = *((int *)arg);
	struct tm *tm_ptr;
	time_t the_time;
	srand(time(NULL));
	
	while(1)
	{
		sem_wait(&bin_sem);
		// 세마포어 값이 0이면 대기, 0이 아니면 1을 감소한 후 실행
		if(a_tid[id] == getMin())
		{
			int r = rand()%10;
			time(&the_time);
			tm_ptr = gmtime(&the_time);

			printf("TID : %2d | ", id+1);
			//printf("R : %d | ", r);
			printf("TIME : %d:%2d:%2d | ", 
			tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			printf("Count : %d\n", ++a_tid[id]);

			sem_post(&bin_sem);
			sleep(r);
		}
		else	sem_post(&bin_sem);
		if(a_tid[id] == 20)
		{
			printf("Thread %2d is Finished\n", id+1);
			pthread_exit(NULL);
		}
	}
	
}
