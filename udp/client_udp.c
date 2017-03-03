#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *handler(void *arg)
{
	int *p = (int *)arg;
	int fd = *p;
	int ret;
	char buff[1024] = {0};
	while(1)
	{
		memset(buff,0,1024);
		ret = read(fd,buff,1024);
		printf("recv a msg:%s\n",buff);
	}
}

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("para errno\n");
		return -1;
	}

	int sockFd;
	sockFd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockFd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));//网络序转换
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret;

	ret = connect(sockFd, (struct sockaddr *)&serverAddr,sizeof(serverAddr));
	if(ret < 0)
	{
		perror("connect");
		return -1;
	}

	pthread_t pid;
	char buff[1024] = {0};
	while(1)
	{
		memset(buff,0,1024);
		ret = read(0,buff,1024);
		ret = write(sockFd,buff,ret - 1);
		printf("send a msg:%s\n",buff);

		ret = pthread_create(&pid,NULL,handler,&sockFd);
	}

	pthread_join(pid,NULL);
	close(sockFd);
	return 0;
}
