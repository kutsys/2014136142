// Producer


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shm_com.h"

int main()
{
	int shmid;
	void *shared_memory = (void *)0;
	pid_t pid;

	struct shared_use_st *shared_stuff;
	char buffer[BUFSIZ];

	char input[10];
	while(1){
		scanf("%s", input);
		if(!strcmp("start", input))	break;
		else				printf("Please input <start>\n");
	}
	// 1. Start를 입력 받으면 시작한다.

	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	// 2. 공유 메모리 구조체의 크기를 가진 공유 메모리 세그먼트 생성
	// 성공적으로 만들어지면 0 이상의 정수인 공유 메모리 식별자를 반환.

	if(shmid == -1){
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	// 공유 메모리 생성을 실패하면 -1을 반환한다.


	shared_memory = shmat(shmid ,(void *)0, 0);
	// 3. 프로그램이 공유 메모리에 액세스할 수 있도록 만든다.
	// shmid : shmget이 반환하는 공유 메모리 식별자.
	// shm_addr : 공유 메모리가 현재 프로세스에 연결할 주소
	// shmflg : 비트 플래그의 집합.
	// 호출이 성공하면 공유 메모리의 첫번째 바이트에 대한 포인터를 반환

	if(shared_memory == (void *)-1){
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	// 공유 메모리 호출을 실패하면 -1을 반환한다.

	shared_stuff = (struct shared_use_st *)shared_memory;
	shared_stuff->student_num = 2014136142;
	shared_stuff->client_pid = getpid();
	shared_stuff->running = 2;
	// shared_memory 세그먼트를 shared_stuff로 변환하고 학번과 PID를 저장한다.
	
	pid = fork();
	if(pid == -1){
		printf("fork failed\n");
		exit(1);
	}
	else if(pid == 0)
		execl("./ipc_consumer2", "ipc_consumer2", NULL);  
	// 4. 소비자 호출


	
	while(1){
		if(shared_stuff->running == 0)	break;
		else	sleep(1);
	}

	printf("Waiting....\n");
	
	printf("This is Producer.\n");
	printf("Producer PID : %d\n", getpid());
	printf("Consumer PID : %d\n", shared_stuff->client_pid);
	printf("Student Number : %d\n", shared_stuff->student_num);
	printf("Student Name : %s\n", shared_stuff->name);
	// 4. 전달받은 데이터를 출력한다.	




	if(shmdt(shared_memory) == -1){
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
	// 공유 메모리 해지, 삭제
}