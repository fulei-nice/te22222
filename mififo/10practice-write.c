#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#define MYFIFO "./myfifo1"

int main()
{
	int ret ;
	ret = mkfifo(MYFIFO,0640);//
	if((ret < 0) && errno != EEXIST)//
	{
		perror("mkfifo");
		return -1;		
	}

	int fd;
	char buff[1024] = {0};
	fd = open(MYFIFO,O_WRONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	while(1)
	{
		memset(buff,0,1024);
	printf("Input  messages:    ");
	scanf("%s",buff);
	ret = write(fd,buff,strlen(buff));
	if(ret < 0)
	{
		perror("open");
		return -1;
	}
//	printf("send  messages:%s\n",buff);
	printf("信息已发送\n");
	}
	close(fd);
	return 0;
}

