#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define IS_A 1
#define IS_L 12
#define IS_I 9

#define IS_OPTION(option,flag) ((option) & (1 << ((flag) -1)))

char CurrentPath[128] = {0};
int Option = 0;
//寻找路径并解析选项
void AnayPara(char *argv[],int argc,char *path[])
{
	int i = 1;
	int count = 0;
	for(; i<argc; ++i)
	{
		if(strncmp(argv[i],"-",1) != 0)
		{
			path[count++] = argv[i];
			continue;
		}
		char *p = argv[i] + 1;//p ---> 第一个有效选项
		while(*p != 0)
		{
			Option |= 1 << (*p - 'a');
			p++;
		}
	}
	if(count == 0) //用户没有给定路径
	{
		getcwd(CurrentPath,127);
		path[0] = CurrentPath;
	}
}

void PrintFileInfo(struct dirent *d,char *path)
{
	//没有-a就不打印.和..
	if(!IS_OPTION(Option,IS_A) && strncmp(d->d_name,".",1) == 0)
	{
		return;
	}
	
	struct stat st;
	char pathname[128] = {0};
	strcpy(pathname,path);
	strcat(pathname,"/");
	strcat(pathname,d->d_name);
	stat(pathname,&st);

	//要是有-i就打印inode
	if(IS_OPTION(Option,IS_I))
	{
		printf("%d  ",d->d_ino);
	}

	if(IS_OPTION(Option,IS_L))
	{
		//
	}

	if(S_ISDIR(st.st_mode))
	{
		printf("\033[1;34m%s\033[0m  ",d->d_name);
		return;
	}

	if(st.st_mode & S_IXUSR ||
			st.st_mode & S_IXGRP ||
			st.st_mode & S_IXOTH)
	{
		printf("\033[1;32m%s\033[0m ",d->d_name);
		return;
	}
	printf("%s  ",d->d_name);
}

int main(int argc ,char*argv[])
{
	char *path[128] = {0};
	AnayPara(argv,argc,path);
	int i = 0;
	while(path[i] != NULL)
	{
		DIR *dirp = opendir(path[i]);
		if(dirp == NULL)
		{
			printf("%s Not found\n",path[i]);
			break;
		}

		struct dirent *d = NULL;
		while((d = readdir(dirp)) != NULL)
		{
			PrintFileInfo(d,path[i]);
		}

		printf("\n");
		closedir(dirp);
		i++;
	}
}
