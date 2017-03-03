#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main()
{
	int fd;
	int ret;//return
	//1.打开文件
	fd = open("1.log",O_RDWR | O_CREAT,0640);
	if(fd < 0)
	{
	//	printf("open error:\n");
		perror("open");
		return -1;
	}
	//2.读写文件
	//char *buff = "hello";
//	ret = write(fd,buff,strlen(buff));
	char buffer[1024] = {0};
	ret = read(fd,buffer,1024);
	if(ret < 0)
	{
		printf("read error:\n");
		close(fd);
		return -1;
	}
	printf("ret = %d , read:%s\n",ret,buffer);
	
	//3.关闭文件
	close(fd);

	return 0;
}
