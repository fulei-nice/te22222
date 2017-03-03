#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

int sockFd ;

void *handler(void *arg)
{
	struct sockaddr *clientAddr = (struct sockaddr *)arg;
	
	char buff[1024];
	int ret;

	while(1)
	{
		memset(buff,0,1024);
		ret = read(0, buff, 1024);
		ret = sendto(sockFd, buff, ret - 1, 0, clientAddr, sizeof(struct sockaddr));
		printf("send a msg:%s\n",buff);
	}

	return NULL;
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("para error\n");
		return -1;
	}

	sockFd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockFd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret ;

	ret = bind(sockFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
	if(ret < 0)
	{
		perror("bind");
		return -1;
	}

	int flag = 0;
	struct sockaddr_in clientAddr;
	int clilen = sizeof(clientAddr);
	char buff[1024] = {0};
	char buff1[1024] = {0};

	pthread_t pid;

	while(1)
	{
		memset(buff,0,1024);
		memset(buff1,0,1024);
		ret = recvfrom(sockFd, buff, 1024, 0, (struct sockaddr *)&clientAddr,&clilen);
		if(ret < 0)
		{
			perror("recvfrom");
			return -1;
		}
		printf("recv a msg: %s\n",buff);

		if(flag == 0)
		{
			ret = pthread_create(&pid,NULL,handler,&clientAddr);
			flag = 1;
		}
	}

	pthread_join(pid,NULL);
	close(sockFd);
	return 0;
}
