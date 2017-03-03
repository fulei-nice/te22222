#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
	int fd;
	int ret;
	fd = open("test.log",O_WRONLY | O_CREAT,0640);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	char *buff = "abcdefghij";
	ret = write(fd,buff,strlen(buff));

	if(ret < 0)
	{
		printf("write");
		close(fd);
		return -1;
	}
	printf("test:\n%s\n",buff);

	int fd1 = open("test1.log",O_WRONLY | O_CREAT,0640);
	if(fd1 < 0)
	{
		perror("open");
			return -1;
	}
	int ret1;
	ret1 = write(fd1,buff,strlen(buff));
	if(ret1 < 0)
	{
		perror("write");
		close(fd1);
		return -1;
	}
	printf("test1:\n%s\n",buff);
	close(fd);
	close(fd1);
	return 0;
}
