#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	if(argc < 3)
	{
		printf("error");
		exit(0);
	}
	int fd1 = open(argv[1],O_RDONLY);
	assert(fd1 != -1);
	int fd2 = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0664);
	assert(fd2 != -1);

	char buff[128] = {0};
	while(1)
	{
		int n = read(fd1,buff,127);
		if(n <= 0)
		{
			//exit(0);
			break;
		}
		write(fd2,buff,n);
	}
	close(fd1);
	close(fd2);
}
