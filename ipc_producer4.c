#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "socket_path"
#define BUFF_SIZE 1024

struct my_msg_st{
	int p_pid;
	int c_pid;
	int student_num;
	char name[BUFSIZ];
};

int main()
{
	int producer_socket;
	int consumer_socket;
	int consumer_size;
	pid_t pid;

	struct sockaddr_un producer_addr;
	struct sockaddr_un consumer_addr;
	struct my_msg_st some_data;

	char buff_snd[BUFF_SIZE];

	char p_pid[20];
	char c_pid[20];
	char name[20] = {0};
	char student_num[11] = {0};
	
	char input[10] = {0};
	while(1){
		scanf("%s", input);
		if(!strcmp("start", input))	break;
		else				printf("Please input <start>\n");
	}
	// 1. Start를 입력 받으면 시작한

	unlink(SOCK_PATH);
	producer_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if(producer_socket == -1){
		fprintf(stderr, "Failed to create producer socket\n");
      		exit(EXIT_FAILURE);
	}
	memset(&producer_addr, 0, sizeof(producer_addr));
	producer_addr.sun_family= AF_UNIX;
	// 1. domain : 주소 패밀리를 지정.
	//    AF_UNIX : 유닉스와 리눅스 파일 시스템으로 구현되는 로컬 소켓을 위해 사용
	// 2. type : 이 소켓으로 사용할 통신의 형식을 지정
	// 3. protocol : 사용할 프로토콜을 지정

	strcpy(producer_addr.sun_path,SOCK_PATH);

	// AF_UNIX 소켓에 대해서 sockaddr_un 구조체로 주소를 설명.
	// sa_family_t	sun_family : AF_UNIX
	// char		sun_path[] : 경로 이름	
	if(bind(producer_socket, (struct sockaddr*)&producer_addr, 
					sizeof(producer_addr)) == -1){
		fprintf(stderr, "bind() 실행 에러\n");
		exit(EXIT_FAILURE);
	}

	if(listen(producer_socket, 5) == -1){
		fprintf(stderr,  "listen() 실행 실패\n");
		exit(EXIT_FAILURE);
   	}
	// 소켓에 들어오는 연결을 허용하기 위해 
	// 서버 프로그램이 보류된 요청을 저장하기 위한 대기열을 생성.

	// 대기열이 취할 수 있는 보류 연결의 최대 개수를 제한하고 (=backlog)
	// 대기열의 길이 제한에 도달할 때까지 들어오는 연결은 소켓에 보류된 상태가 됨
	// 그 이상의 연결은 거부하고 클라이언트의 연결은 실패.

	pid = fork();
	if(pid == -1){
		printf("fork failed\n");
		exit(1);
	}
	else if(pid == 0)
		execl("./ipc_consumer4", "ipc_consumer4", NULL);  
	// 자식 생성

	
	some_data.student_num = 2014136142;
	some_data.p_pid = getpid();
	
	//strcpy(p_pid, getpid()+"");
	//strcpy(student_num, "2014136142");
	
	consumer_size = sizeof(consumer_addr);
	consumer_socket = accept(producer_socket,
		(struct sockaddr*)&consumer_addr, &consumer_size);
	// 소켓의 대기열에서 기다리고 있던 보류 연결.
	// 통신하기 위해 새로운 소켓을 만들고 이것의 설명자를 반환한다.
		
	if (consumer_socket == -1){
		fprintf(stderr, "Failed to connect\n");
		exit(EXIT_FAILURE);
	}

	//strcpy(buff_snd, p_pid);
	write(consumer_socket,&some_data,sizeof(some_data));
	read(consumer_socket,&some_data,sizeof(some_data));
	//write(consumer_socket, buff_snd, BUFF_SIZE);
	//strcpy(buff_snd, student_num);
	//write(consumer_socket, buff_snd, BUFF_SIZE);
	// send to consumer
 
	//read(consumer_socket, c_pid, BUFF_SIZE);
	//read(consumer_socket, name, BUFF_SIZE);
	
	printf("This is Producer.\n");
	printf("Producer PID : %d\n", some_data.p_pid);
	printf("Consumer PID : %d\n", some_data.c_pid); 
	printf("Student Number : %d\n", some_data.student_num);
	printf("Student Name : %s\n", some_data.name);
	// 4. 전달받은 데이터를 출력한다.	      

	close(consumer_socket);
	exit(EXIT_SUCCESS);
}
