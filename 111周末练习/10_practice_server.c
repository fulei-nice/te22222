#include <stdio.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>

int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("para error\n");
		return -1;
	}

	int sockfd;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr,0,sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret ;
	ret = bind(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
	if(ret < 0)
	{
		perror("bind");
		return -1;
	}

	ret = listen(sockfd,100);
	if(ret < 0)
	{
		perror("listen");
		return -1;
	}

	struct sockaddr_in clientAddr;
	int clilen = sizeof(clientAddr);

	int clientFd;

	clientFd = accept(sockfd,(struct sockaddr *)&clientAddr,&clilen);
	if(clientFd < 0)
	{
		perror("accept");
		return -1;
	}
	
	pid_t pid1;
	pid_t pid2;
	pid_t pid3;
	char buff[1024] = {0};
	char *buff1 = "CREAT";
	char *buff2 = "KILL";

	while(1)
	{
		memset(buff,0,1024);

		ret = read(clientFd,buff,1023);
		if(ret < 0)
		{
			perror("read");
			return -1;
		}
		printf("recv a message: %s\n",buff);
		if(strcmp(buff1,buff) == 0)
		{
			if(!(pid1 = fork()))
			{
				while(1)
				{
				printf("in child1 creat :pid1 = %d\n",getpid());
				sleep(1);
				}
				return 0;
			}
			if(!(pid2 = fork()))
			{
				while(1)
				{
				printf("in child2 creat :pid2 = %d\n",getpid());
				sleep(1);
				}
				return 0;
			}
			if(!(pid3 = fork()))
			{
				while(1)
				{
				printf("in child3 creat :pid3 = %d\n",getpid());
				sleep(1);
				}
				return 0;
			}
			printf("create ok!\n");
		}
		
		if(strcmp(buff2,buff) == 0)
		{
			printf("star kill pid1……\n");
			sleep(2);
			kill(pid1,9);
			printf("star kill pid2……\n");
			sleep(2);
			kill(pid2,9);
			printf("star kill pid3……\n");
			sleep(2);
			kill(pid3,9);
			sleep(2);
			printf("kill over!\n");
		}
	}

	wait(&pid1);
	wait(&pid2);
	wait(&pid3);
	close(sockfd);
	close(clientFd);
	return 0;
}


















