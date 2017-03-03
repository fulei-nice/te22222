#include <stdio.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>


int main(int argc,char **argv)
{
	if(argc != 3)
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
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret;
	ret = connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
	if(ret < 0)
	{
		perror("connect");
		return -1;
	}
	char buff[1024] = {0};	
	while(1)
	{
		memset(buff,0,1024);
		ret = read(0,buff,1024);
	ret = write(sockfd,buff,ret -1);
	if(ret < 0)
	{
		perror("write");
		return -1;
	}

	printf("send a message:%s\n",buff);
	}
	close(sockfd);
	return 0;

}
