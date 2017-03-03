#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc,char ** argv)
{
	int fd1;//拷贝源文件
	int fd2;//拷贝目的文件
	fd1 = open(argv[1],O_RDONLY);
	if(fd1 < 0)
	{
		perror("open");
		return -1;
	}
	fd2 = open(argv[2],O_WRONLY | O_TRUNC |O_CREAT,0640);
	if(fd2 < 0)
	{
		perror("open");
		return -1;
	}

	char buff[1024] ={0};
	int ret;
	while(ret = read(fd1,buff,1024))
	{
		ret = write(fd2,buff,ret);
	//	memset(buff,0,1024);
	}
	close(fd1);
	close(fd2);
	return 0;
}
