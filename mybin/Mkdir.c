#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char *argv[])
{
	if(argv[1][0] == '/')
	{
		printf("mymkdir::cannot create %s \n",argv[1]);
	}
	int i = 0;
	for(i ; argv[i] != '\0' ; i++)
	{
		mkdir(argv[i],0744);
	}
}

