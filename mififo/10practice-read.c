#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#define MYFIFO "./myfifo1"

int main()
{
	int ret ;
	ret = mkfifo(MYFIFO,0640);//创建有名管道
	if((ret < 0) && errno != EEXIST)//EEXIST 文件已经存在的错误
	{
		perror("mkfifo");
		return -1;		
	}

	int fd,fd1;
	char buff[1024] = {0};
	fd = open(MYFIFO,O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	/*
	fd1 =open("1.log",O_WRONLY | O_CREAT,0640);
	if(fd1 < 0)
	{
		perror("open");
		return -1;
	}
*/	
	while(1)
	{
		memset(buff,0,1024);
	ret = read(fd,buff,1023);
	if(ret < 0)
	{
		perror("open");
		return -1;
	}
	printf("recv messages:    %s\n",buff);
	printf("信息已受到\n");
//	ret = write(fd1,buff,1023);
//	printf("文件中内容:%s\n",buff);
	}
	close(fd);
//	close(fd1);
	return 0;
}

