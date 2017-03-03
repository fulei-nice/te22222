#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	int num = 2;

	if(!(pid = fork()))
	{
		num++;
		printf("in child num = %d\n",num);
		return 0;
	}
	
	printf("in parent num = %d\n",num);
	wait(&pid);
	return 0;
}
