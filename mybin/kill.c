#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		printf("未输入进程号");
		exit(0);
	}
	int pid = atoi(argv[1]);
	if(strcmp(argv[1],"-9") == 0)
	{
		int pid = atoi(argv[2]);
		kill(pid,SIGKILL);
	}
	else
	{
  		int pid = atoi(argv[1]);
		kill(pid,SIGINT);
	}
	exit(0);
}
