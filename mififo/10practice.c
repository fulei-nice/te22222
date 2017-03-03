#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
	int ret ;
	pid_t pid;
	int fd[2];

	ret =  pipe(fd);
	if(ret < 0)
	{
		perror("pipe");
		return -1;
	}

	if(!(pid = fork()))
	{
		close(fd[0]);
		char buff[1024] = {0};
		printf("Input a message:\n");
		scanf("%s",buff);
		ret = write(fd[1],buff,strlen(buff));
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
		printf("child : %s\n",buff);
		close(fd[1]);
		return 0;
	}
	close(fd[1]);
	char buffer[1024]={0};
	ret = read(fd[0],buffer,1023);
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
		printf("parent : %s\n",buffer);
		close(fd[0]);
		wait(&pid);
		return 0;
	
}
