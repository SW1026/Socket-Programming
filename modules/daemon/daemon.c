#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int 
main()
{
	pid_t pid;
	if(pid = fork() < 0)
	{
		exit(1);
	}
	else if(pid > 0)
	{
		exit(0);
	}
	else
	{
		chdir("/");
		setsid();
		while(1)
		{
			printf("Hi my name is Seowoo\n");
		}
	}
}





