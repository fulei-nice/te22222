#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>


void handler(int num)
{
	printf("handler is running……\n");
	return ;
}
int main()
{
	pid_t pid1;
	pid_t pid2;
	pid_t pid3;
	
	if(!(pid1 = fork()))
	{
		while(1)
		{
		printf("in child1 :pid1 = %d\n",getpid());
		sleep(1);
		}
		return 0;
	}
	if(!(pid2 = fork()))
	{
		while(1)
		{
		printf("in child2 :pid2 = %d\n",getpid());
		sleep(1);
		}
		return 0;
	}
	if(!(pid3 = fork()))
	{
		while(1)
		{
		printf("in child3 :pid3 = %d\n",getpid());
		sleep(1);
		}
		return 0;
	}

	while(1)
	{
//	printf("in parent child: pid = %d\n",getpid());
	printf("star kill pid1\n");
	sleep(2);
	kill(pid1,9);
	printf("star kill pid2\n");
	sleep(2);
	kill(pid2,9);
	printf("star kill pid3\n");
	sleep(2);
	kill(pid3,9);
	printf("kill pid :ok\n");
	sleep(2);
	break;
	}

	wait(&pid1);
	wait(&pid2);
	wait(&pid3);
	return 0;
}
