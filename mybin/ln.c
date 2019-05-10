#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
int main(int argc,char*argv[])
{
    if(argc < 2)
    {
	printf("error");
	exit(0);
    }
    if(argc == 4 && strcmp(argv[1],"-s") == 0)
    {
	int a = symlink(argv[2],argv[3]);
	assert(a != -1);
    }
    else if(argc == 3)
    {
	int a = link(argv[1],argv[2]);
	assert(a != -1);
    }
}
