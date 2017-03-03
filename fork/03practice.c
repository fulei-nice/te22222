#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

	pid_t a_pid,b_pid;
	if((a_pid = fork()) < 0)
	printf("error!\n");
	else if(a_pid == 0)
	{
		printf("the first child pid = %d\n",getpid());
		printf("a\n");
	}
	else
	{
		printf("the parent pid = %d\n",getpid());
		printf("a\n");
	}

	if((b_pid = fork()) < 0)
		printf("error!\n");
	else if(b_pid == 0)
	{
		printf("c\n");
	}
	else
	{
		printf("e\n");
	}
	wait(&a_pid);
	return 0;
}
