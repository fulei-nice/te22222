#include <stdio.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
//客户端链接服务器

int main(int argc,char **argv)//ip port
{
	if(argc != 3)
	{
		printf("para error\n");
		return -1;
	}

	int sockfd;

	sockfd = socket(AF_INET,SOCK_STREAM,0);//TCP
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in serverAddr;

	memset(&serverAddr,0,sizeof(serverAddr));//sizeof(struct sockaddr_in)

	//赋值  服务器的地址信息

	serverAddr.sin_family = AF_INET;//IPv4
	serverAddr.sin_port = htons(atoi(argv[2]));//atoi的作用"8080" -->8080,转成网络字节序
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret;
	//连接 服务器
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
