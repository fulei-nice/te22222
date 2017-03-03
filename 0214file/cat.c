#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
int main(int argc,char ** argv)
{
	int ret;
	int fd = open(argv[1],O_RDWR | O_CREAT,0640);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	char buff[1024] = {0};
	ret = read(fd,buff,1024);
	if(ret < 0)
	{
		printf("read error:\n");
		close(fd);
		return -1;
	}
	printf("ret = %d\nargc = %d\nread:\n%s\n",ret,argc,buff);
	close(fd);
	return 0;
}
