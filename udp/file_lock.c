#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
	int pid;
	int fd;
	int ret;
	char buff[1024] = {0};
	int i;

	if(!(pid = fork()))
	{
		//in child
		fd = open("./test.log",O_WRONLY | O_CREAT, 0640);

		flock(fd, LOCK_EX);

		lseek(fd, 0, SEEK_END);
		for(i = 0; i < 10; i++)
		{
			ret = write(fd, "A", 1);
			sleep(1);
		}
		flock(fd, LOCK_UN);
		close(fd);
		return 0;
	}

	//in parent

	fd = open("./test.log", O_WRONLY | O_CREAT,0640);

		flock(fd, LOCK_EX);
		lseek(fd, 0, SEEK_END);
	for(i = 0; i < 10; i++)
	{
		ret = write(fd, "B", 1);
		sleep(1);
	}
		flock(fd, LOCK_UN);

		wait(&pid);
		close(fd);
	return 0;
}
