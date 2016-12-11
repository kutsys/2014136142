#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>	// File control
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#define PIPE_FIFO "/tmp/fifo"
#define BUFFER_SIZE 10

struct data_to_pass_st{
	pid_t client_pid;
	int student_num;
	char name[BUFFER_SIZE];
};

int main()
{
	int server_fifo_fd, client_fifo_fd;
	struct data_to_pass_st my_data;
	int read_res;
	char *tmp_char_ptr;
	pid_t pid;

	char input[BUFFER_SIZE];
	while(1){
		scanf("%s", input);
		if(!strcmp("start", input))	break;
		else				printf("Please input <start>\n");
	}
	// 1. Start를 입력 받으면 시작한다.
	
	int res =mkfifo(PIPE_FIFO, 0777);
	
	pid = fork();
	if(pid == -1){
		printf("fork failed\n");
		exit(1);
	}
	else if(pid == 0)
		execl("./ipc_consumer", "ipc_consumer", NULL);  
	// 2. 자식 생성

	my_data.student_num = 2014136142;
	my_data.client_pid = getpid();

	server_fifo_fd = open(PIPE_FIFO, O_WRONLY);
	if(server_fifo_fd == -1){
		fprintf(stderr, "Server fifo failure\n");
		exit(EXIT_FAILURE);
	}
	write(server_fifo_fd, &my_data, sizeof(my_data));
	// 3. 구조체에 Consumer에 보낼 PID, 학번을 저장한다.


	printf("Waiting....\n");
	sleep(5);

	client_fifo_fd = open(PIPE_FIFO, O_RDONLY);
	read_res = read(client_fifo_fd, &my_data, sizeof(my_data));
	printf("This is Producer.\n");
	printf("Producer PID : %d\n", getpid());
	printf("Consumer PID : %d\n", my_data.client_pid); 
	printf("Student Number : %d\n", my_data.student_num);
	printf("Student Name : %s\n", my_data.name);
	// 4. 전달받은 데이터를 출력한다.	

	sleep(1);
	close(client_fifo_fd);
	unlink(PIPE_FIFO);
	exit(EXIT_SUCCESS);
}