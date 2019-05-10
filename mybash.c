#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/utsname.h>

#define STRLEN 128
char OLDPWD[STRLEN] = {0};

void OutPutFlag()
{
	char flag = '$';
	struct passwd *pw = getpwuid(getuid());
	assert(pw != NULL);
	if(pw->pw_uid == 0)  //su用户的uid为0
	{
		flag = '#';
	}
	struct utsname hostname;
	uname(&hostname);
	char node[STRLEN] = {0};
	strcpy(node,hostname.nodename);
	char *name = strtok(node,".");

	char path[STRLEN] = {0};
	getcwd(path,STRLEN-1);

	char *p = path + strlen(path);
	while(*p != '/')
	{
		p--;
	}
	if(strlen(path) != 1)
	{
		p++;
	}
	if(strcmp(path,pw->pw_dir) == 0)
	{
		p="~";
	}
	printf("mybash[%s@%s %s]%c",pw->pw_name,name,p,flag);
	fflush(stdout);

}

void MyCd(char *path)
{
	if(path == NULL)
	{
		return;
	}
	if(strcmp(path,"~") == 0)
	{
		struct passwd *pw = getpwuid(getuid());
		path = pw->pw_dir;
	}
	if(strcmp(path,"-") == 0)
	{
		if(strlen(OLDPWD) == 0)
		{
			printf("mybash: cd: OLDPWD not set\n");
			return;
		}

		path = OLDPWD;
	}

	char oldpwd[STRLEN] = {0};
	getcwd(oldpwd,STRLEN-1);

	if(-1 == chdir(path))
	{
		char err[] = "mybash cd: ";
		strcat(err,path);

		perror(err);
		return;
	}
	strcpy(OLDPWD,oldpwd);
}
void IncisionString(char *cmd, char**argv)
{
	int count = 0;

	char *p = strtok(cmd , " ");
	while(p != NULL)
	{
		argv[count++] = p;
		p = strtok(NULL," ");
	}
}

void Zombie(int sign)
{
	wait(NULL);
}

int main()
{
	signal(SIGCHLD,Zombie);
	while(1)
	{
		OutPutFlag();
		char cmd[STRLEN] = {0};
		fgets(cmd,STRLEN,stdin);
		cmd[strlen(cmd) - 1] = 0;

		int black = 0;
		if(strstr(cmd, "&") != NULL)
		{
			black = 1;
		}

		char *argv[STRLEN] = {0};

		IncisionString(cmd, argv);

		if(strncmp(argv[0],"cd",2) == 0)
		{
			MyCd(argv[1]);
			continue;
		}
		if(strncmp(argv[0],"exit",4) == 0)
		{
			exit(0);
		}   

		pid_t n = fork();
		assert(n != -1);

		if(n == 0)
		{
			char path[STRLEN] = "/home/dw/Desktop/dir/shell/mybin/";
			if(strstr(argv[0],"/") != NULL)
			{
				strcpy(path,argv[0]);
			}
			else
			{
				strcat(path,argv[0]);
			}
			execv(path,argv);
			printf("%s:Command Not Found\n",argv[0]);
			exit(0);//防止子程序继续执行
		}
		else
		{
			if(!black)
			{
				pause();
			}
		}
	}
}
