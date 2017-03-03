#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	pid_t pid;
	int ret;
	int fd[2];

	ret = pipe(fd);
	if(ret < 0)
	{
		perror("pipe");
		return -1;
	}

	if(!(pid  = fork()))
	{
		//child
		close(fd[1]);
		char buff[1024]={0};
		ret = read(fd[0],buff,1023);
		if(ret < 0)
		{
			perror("read");
			return -1;
		}
		printf("child recv a msg:%s\n",buff);
		close(fd[0]);
		return 0;
	}

	//parent
	close(fd[0]);
	ret = write(fd[1],"hello",5);
	printf("parent send a msg:%s\n","hello");
	wait(&pid);
	close(fd[1]);
	return 0;
}
