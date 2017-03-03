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
	if(argc != 3)
	{
		printf("para error\n");
		return -1;
	}
	char fileName[48] = {0};
	printf("please input a file name:\n");
	scanf("%s",fileName);
	
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
	
	ret = write(sockfd,fileName,strlen(fileName));

	sleep(2);
	int filefd;
	filefd = open(fileName,O_RDONLY);
	char buff[1024] = {0};
	while(1)
	{
		memset(buff,0,1024);
		ret = read(filefd,buff,1024);
		if(ret == 0)
		{
			break;
		}
		ret = write(sockfd,buff,ret);
	}

	close(filefd);
	close(sockfd);
	return 0;
}
