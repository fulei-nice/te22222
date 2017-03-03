#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#define MYFIFO "./myfifo"

int main()
{
	int ret ;
	ret = mkfifo(MYFIFO,0640);//创建有名管道
	if((ret < 0) && errno != EEXIST)//EEXIST 文件已经存在的错误
	{
		perror("mkfifo");
		return -1;		
	}

	int fd;
	char buff[1024] = {0};
	fd = open(MYFIFO,O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	ret = read(fd,buff,1023);
	if(ret < 0)
	{
		perror("open");
		return -1;
	}
	printf("recv a message:%s\n",buff);

	close(fd);
	return 0;
}

