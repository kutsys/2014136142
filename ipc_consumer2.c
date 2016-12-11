// Consumer

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
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;
	int shmid;
	pid_t pid;
	
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

	if(shmid == -1){
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shared_memory = shmat(shmid ,(void *)0, 0);
	if(shared_memory == (void *)-1){
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	shared_stuff = (struct shared_use_st *)shared_memory;
	
	int i=0;
	while(1){
		if(shared_stuff->running == 2)	break;
		else	sleep(1);
	}

	printf("This is Consumer.\n");
	printf("Producer PID : %d\n", shared_stuff->client_pid);

	strcpy(shared_stuff->name, "최은진"); 
	shared_stuff->client_pid = getpid();
	shared_stuff->running = 0;

	if(shmdt(shared_memory) == -1){
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	//공유 메모리를 현재 프로세스로부터 분리한다.
	exit(EXIT_SUCCESS);
}