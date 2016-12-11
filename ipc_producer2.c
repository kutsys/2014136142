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
	// 1. Start�� �Է� ������ �����Ѵ�.

	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	// 2. ���� �޸� ����ü�� ũ�⸦ ���� ���� �޸� ���׸�Ʈ ����
	// ���������� ��������� 0 �̻��� ������ ���� �޸� �ĺ��ڸ� ��ȯ.

	if(shmid == -1){
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	// ���� �޸� ������ �����ϸ� -1�� ��ȯ�Ѵ�.


	shared_memory = shmat(shmid ,(void *)0, 0);
	// 3. ���α׷��� ���� �޸𸮿� �׼����� �� �ֵ��� �����.
	// shmid : shmget�� ��ȯ�ϴ� ���� �޸� �ĺ���.
	// shm_addr : ���� �޸𸮰� ���� ���μ����� ������ �ּ�
	// shmflg : ��Ʈ �÷����� ����.
	// ȣ���� �����ϸ� ���� �޸��� ù��° ����Ʈ�� ���� �����͸� ��ȯ

	if(shared_memory == (void *)-1){
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	// ���� �޸� ȣ���� �����ϸ� -1�� ��ȯ�Ѵ�.

	shared_stuff = (struct shared_use_st *)shared_memory;
	shared_stuff->student_num = 2014136142;
	shared_stuff->client_pid = getpid();
	shared_stuff->running = 2;
	// shared_memory ���׸�Ʈ�� shared_stuff�� ��ȯ�ϰ� �й��� PID�� �����Ѵ�.
	
	pid = fork();
	if(pid == -1){
		printf("fork failed\n");
		exit(1);
	}
	else if(pid == 0)
		execl("./ipc_consumer2", "ipc_consumer2", NULL);  
	// 4. �Һ��� ȣ��


	
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
	// 4. ���޹��� �����͸� ����Ѵ�.	




	if(shmdt(shared_memory) == -1){
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
	// ���� �޸� ����, ����
}