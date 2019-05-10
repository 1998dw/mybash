#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
int main(int argc,char*argv[])
{
	if(argc < 2)
	{
		printf("输入命令错误\n");
		exit(0);
	}
	int i = 0;
	for(i ;argv[i]!='\0' ; i++)
	{
		creat(argv[i],0664);
	}
}
