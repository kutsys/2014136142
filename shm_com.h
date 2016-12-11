// shm_com.h
// 소비자와 공급자가 사용하는 구조체

#define TEXT_SZ 2048

struct shared_use_st{
	pid_t client_pid;
	int student_num;
	char name[10];
	int running;
};