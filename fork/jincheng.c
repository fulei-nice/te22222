#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t pid;

	if(!(pid = fork()))
	{
		printf("in child parent pid = %d,child pid = %d\n",getppid(),getpid());
		return 0;
	}

	printf("in parent pid = %d,child pid= %d\n",getpid(),pid);
	wait(&pid);
	return 0;
}
