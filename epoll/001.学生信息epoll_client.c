#include <stdio.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
//客户端链接服务器

typedef struct stu
{
	int id;
	char name[20];
}STU_T;

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

	STU_T Da;

	while(1)
	{
		memset(&Da,0,sizeof(Da));
		printf("please input id(0 is exit):\n");
		scanf("%d",&(Da.id));

		if(Da.id == 0)
		{
			break;
		}

		printf("please input name:\n");
		scanf("%s",Da.name);

		Da.id = htonl(Da.id);
		ret = write(sockfd,&Da,sizeof(Da));

		if(ret < 0)
		{
			perror("write");
			return -1;
		}

		printf("send student message:id = %d, name = %s\n",Da.id,Da.name);
	}
	close(sockfd);
	return 0;

}
