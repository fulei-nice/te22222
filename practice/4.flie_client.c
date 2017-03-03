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
	if(argc != 3)
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
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	ret = connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
	if(ret < 0)
	{
		perror("connect");
		return -1;
	}
	

	int fd;
	char buff[1024] = {0};
	while(1)
	{
		memset(buff,0,1024);
	fd = open("1.log",O_WRONLY | O_APPEND | O_CREAT,0640);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	ret = read(0,buff,1024);
	ret = write(fd,buff,ret -1);
	printf("in send file :%s\n",buff);
	char *p = "1.log";

	ret = write(sockfd,p,sizeof(p));
	printf("send a file :%s\n",p);
	}
	close(fd);
	close(sockfd);
	return 0;
}
