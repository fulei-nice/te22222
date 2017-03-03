#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

void *handler(void *arg)
{
	int *p = (int *)arg;
	int ret;
	int fd = *p;
	char buff[1024];
	while(1)
	{
		memset(buff,0,1024);
		ret = read(fd,buff,1024);
		printf("recv message:%s:%s\n",buff,buff +20);
	}
	
}
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
	
	pthread_t pid;
	char buff[1024] = {0};
	char buff1[20] = {0};
	char buff2[1000] = {0};
		printf("please input a name:");
		scanf("%s",buff1);
	while(1)
	{
		memset(buff2,0,1024);
		memset(buff,0,1024);
		strcpy(buff,buff1);
		ret = read(0,buff2,1024);
		strcpy(buff + 20,buff2);
	ret = write(sockfd,buff,20 + ret -1);
//	printf("send message:%s\n",buff);
	ret = pthread_create(&pid,NULL,handler,&sockfd);
	}
	pthread_join(pid,NULL);
	close(sockfd);
	return 0;
}
