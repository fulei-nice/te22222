#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("para error\n");
		return -1;
	}
	char fileName[48] = {0};
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

	int ret;

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
	if(ret < 0)
	{
		perror("accept");
		return -1;
	}
	
	ret = read(clientFd,fileName,48);
	int filefd;
	filefd = open(fileName,O_WRONLY | O_TRUNC | O_CREAT,0640);
	char buff[1024] = {0};
	while(1)
	{
		memset(buff,0,1024);
		ret = read(clientFd,buff,1024);
		if(ret < 0)
		{
			printf("ret < 0\n");
			break;
		}
		if(ret == 0)
		{
			printf("ret = 0\n");
			break;
		}

		ret = write(filefd,buff,ret);

	}

	close(filefd);
	close(sockfd);
	close(clientFd);
	return 0;
}
