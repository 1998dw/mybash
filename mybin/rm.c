#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<dirent.h>
#include<limits.h>
#include<string.h>
#include<sys/stat.h>

int isdir(const char*path)
{
    struct stat statbuf;
    if(lstat(path,&statbuf) == 0)
    {
	if(S_ISDIR(statbuf.st_mode) != 0)
	    return 1;
    }
    return 0;
}

int isfile(const char*path)
{
    struct stat statbuf;
    if(lstat(path,&statbuf) == 0)
    {
	 if(S_ISREG(statbuf.st_mode) != 0)
	 return 1;
    }
    return 0;
}

int is_special_dir(const char*path)
{
    if((strcmp(path,".") == 0) || (strcmp(path,"..") ==0) )
    {
	return 1;
    }
    else
	return 0;
}
void get_file_path(const char* path,const char* file_name,char*file_path)
{
    strcpy(file_path,path);
    if(file_path[strlen(path)-1] != '/')
    {
	strcat(file_path,"/");
    }
    strcat(file_path,file_name);
}

void delete_file(const char *path)
{
    DIR *dir;
    struct dirent *dir_info;
    char file_path[PATH_MAX];
    if(isfile(path) == 1)
    {
	remove(path);
	return;
    }
    if(isdir(path) == 1)
    {
	if((dir = opendir(path)) == NULL)
	{
	    return;
	}
	while((dir_info = readdir(dir)) != NULL)
	{
	    get_file_path(path,dir_info->d_name,file_path);
	    if(is_special_dir(dir_info->d_name) == 1)
		continue;
	    delete_file(file_path);
	    rmdir(file_path);
	}
    }
    rmdir(path);
}

int main(int argc,char*argv[])
{
	if(argc < 2)
	{
		printf("参数输入错误！！！\n");
	}
	if(argc == 3 && strcmp(argv[1],"-r") == 0)
	{
		delete_file(argv[2]);
	}
	else
	{
		int i = 0;
		for(i ; i < argc && strcmp(argv[i],"-r") != 0; i++)
		{
			unlink(argv[i]);
		}
	}
}
