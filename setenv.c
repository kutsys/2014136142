#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(){
	setenv("TEST_ENV", "1234", 1);
	while(*environ)
		printf("%s\n",*environ++);
	exit(0);
}
