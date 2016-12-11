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
	int times_to_send;
	char client_fifo[256];

	if(access(PIPE_FIFO,F_OK) == -1){
		if(mkfifo(PIPE_FIFO,0777) != 0){
			exit(0);
		}
	}
	client_fifo_fd = open(PIPE_FIFO, O_RDONLY);
	if(client_fifo_fd == -1){
		printf("Sorry, no server\n");
		exit(EXIT_FAILURE);
	}
	read_res = read(client_fifo_fd, &my_data, sizeof(my_data));
	close(client_fifo_fd);
	// 1. 생성자로부터 전달받은 데이터를 읽는다.

	sleep(3);
	printf("This is Consumer.\n");
	printf("Student Number : %d\n", my_data.student_num);
	printf("Producer PID : %d\n", my_data.client_pid); 
	// 2. 전달받은 데이터를 출력한다.

	server_fifo_fd = open(PIPE_FIFO, O_WRONLY);
	strcpy(my_data.name, "최은진");
	my_data.client_pid = getpid();
	write(server_fifo_fd, &my_data, sizeof(my_data));
	close(server_fifo_fd);
	// 3. 생성자에게 데이터를 전달한다.
	exit(EXIT_SUCCESS);
}