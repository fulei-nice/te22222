#include <stdio.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>

void create_pid(pid_t *pid,int num)
{
	int i;
	for(i = 0;i < num; i++)
	{
		switch(pid[i] = fork())
		{
			case 0:
				{
					//in child
					while(1)
					{
						printf("in child pid = %d\n",getpid());
						sleep(2);
					}
				}
			case -1:
				return;
			default:
				{
					//in parent
					printf("in parent creat a chile pid = %d\n",pid[i]);
					break;
				}
		}
	}
return;
}

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
	
	pid_t pid[1];
	char buff[1024] = {0};
	char *buff1 = "CREAT";
	char *buff2 = "KILL";
	int i;

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
			create_pid(pid,3);
			printf("create ok!\n");
		}
		
		if(strcmp(buff2,buff) == 0)
		{
			for(i = 0;i < 3;i++)
			{
				kill(pid[i],9);
			}
		}
	}

	wait(NULL);
	close(sockfd);
	close(clientFd);
	return 0;
}


















