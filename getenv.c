#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char *argv[])
{
	char *var, *value;
	
	var = argv[1];
	value = getenv(var);
	if(value)
		printf("Variable %s has value %s\n", var, value);
	else
		printf("Variable %s has no value\n", var);
	exit(0);
}
