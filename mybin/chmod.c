#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
int main(int argc, char*argv[])
{
    if(argc < 3)
    {
	printf("error");
	exit(0);
    }
    int temp = atoi(argv[1]);
    int a = chmod(argv[2],temp);
    assert(a != -1);
}
