#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int ret;
int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("para error1\n");
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
	int clientFd;
	int clilen;
	clilen = sizeof(clientAddr);
	memset(&clientAddr,0,sizeof(clientAddr));
	
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(atoi(argv[1]));
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	clientFd = accept(sockfd,(struct sockaddr *)&clientAddr,&clilen);
	if(ret < 0)
	{
		perror("clientFd");
		return -1;
	}

	int fd;
	char buffer[1024] = {0};
	char buff[1024] = {0};
while(1)
{
	memset(buff,0,1024);
	memset(buffer,0,1024);
	ret = read(clientFd,buffer,sizeof(buffer));
	printf("recv a file :%s\n",buffer);

	fd = open(buffer,O_RDONLY | O_CREAT,0640);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	ret = read(fd,buff,1023);
	printf("in recv file :\n%s\n",buff);
}

	close(fd);
	close(sockfd);
	close(clientFd);
	return 0;
}
