#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<assert.h>

int main(int argc,char*argv[])
{
	if(argc < 3)
	{
		printf("参数输入错误");
		exit(0);
	}
	int fd1 = open(argv[1],O_RDONLY);
	assert(fd1 != -1);
	int fd2 = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0664);
	assert(fd2 != -1);

	while(1)
	{
		char buff[128] = {0};
		int n = read(fd1,buff,127);
		if(n <= 0)
		{
			break;
		}
		write(fd2,buff,n);
	}
	close(fd1);
	close(fd2);
	unlink(argv[1]);
}
