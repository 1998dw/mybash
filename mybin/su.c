#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <pwd.h>
#include <shadow.h>
#include <termios.h>

int main(int argc,char*argv[])
{
    char *user  = "root";

	if(argc > 1)
	{
		user = argv[1];
	}

	printf("Password: ");
	fflush(stdout);

	struct termios oldtm,newtm;
	tcgetattr(0,&oldtm);
	newtm = oldtm;
	newtm.c_lflag &= ~ECHO;
	newtm.c_lflag &= ~ICANON;
	tcsetattr(0,TCSANOW,&newtm);
	char passwd[128] = {0};
	int j = 0;
	char c = 0;
	while((c = getchar()) != '\n')
	{
		if(c == 127)
		{
			if(j <= 0)
			{
				continue;
			}
			passwd[--j] = 0;
			printf("\033[1D");  //1D光标向左挪一列
			printf("\033[K");  //将光标之后的数据删掉
			continue;
		}
		passwd[j++] = c;
		printf("*");
		fflush(stdout);
	}
		
	tcsetattr(0,TCSANOW ,&oldtm);
	printf("\n");
	struct spwd *sp = getspnam(user);
	assert(sp != NULL);

	//sp->sp_pwdp  $id$salt$miwen
	
	char salt[128] = {0};
	char *p = sp->sp_pwdp;
	int count = 0;
	int i = 0;

	while(*p != 0)
	{
		salt[i++] = *p;
		if(*p == '$')
		{
			count++;
		}
		if(count == 3)
		{
			break;
		}
		p++;
	}
	char *pwdp = crypt(passwd,salt);

	if(strcmp(pwdp,sp->sp_pwdp) != 0)
	{
		printf("password is error\n");
		exit(0);
	}

	pid_t n = fork();
	assert(n != -1);

	if(n == 0)
	{
		struct passwd *pwd = getpwnam(user);
		assert(pwd != NULL);

		setuid(pwd->pw_uid);
		setenv("HOME",pwd->pw_dir,1);
		execl(pwd->pw_shell,pwd->pw_shell,(char *)0);

		printf("su exec fail\n");
		exit(0);
	}
	else
	{
		wait(NULL);
	}
}
