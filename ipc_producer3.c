

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msg_st{
	long int message_type;
	pid_t client_pid;
	int student_num;
	char name[BUFSIZ];
};

int main()
{
	int msgid_s, msgid_r;
	pid_t pid;
	
	struct my_msg_st some_data;
	long int msg_to_receive = 0;

	char input[10];
	while(1){
		scanf("%s", input);
		if(!strcmp("start", input))	break;
		else				printf("Please input <start>\n");
	}
	// 1. Start를 입력 받으면 시작한다.

	msgid_s = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgid_s == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	// 메시지 대기열을 설정


	pid = fork();
	if(pid == -1){
		printf("fork failed\n");
		exit(1);
	}
	else if(pid == 0)
		execl("./ipc_consumer3", "ipc_consumer3", NULL);  
	// 3. 소비자 호출

	some_data.message_type = 1;
	some_data.student_num = 2014136142;
	some_data.client_pid = getpid();

	if(msgsnd(msgid_s, (void *)&some_data, BUFSIZ, 0) == -1){
		fprintf(stderr, "msgsnd error : %d", errno);
		exit(EXIT_FAILURE);
	}
	

	msgid_r = msgget((key_t)1235, 0666 | IPC_CREAT);
	//printf("BBB");
	if(msgid_r == -1){
		fprintf(stderr, "msgget failed with error : %d\n", errno);
		exit(EXIT_FAILURE);
	}
	if(msgrcv(msgid_r, (void *)&some_data, BUFSIZ, msg_to_receive, 0) == -1){
		fprintf(stderr, "msgsnd failed\n");
		exit(EXIT_FAILURE);
	}
	
	printf("This is Producer.\n");
	printf("Producer PID : %d\n", getpid());
	printf("Consumer PID : %d\n", some_data.client_pid);
	printf("Student Number : %d\n", some_data.student_num);
	printf("Student Name : %s\n", some_data.name);
	// 4. 전달받은 데이터를 출력한다.	
	

	if(msgctl(msgid_r, IPC_RMID, 0) == -1){
		fprintf(stderr, "msgrcv failed with error : %d", errno);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}