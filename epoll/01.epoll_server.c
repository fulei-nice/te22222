#include <stdio.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define EPOLLMAX 1000

void setnonblocking(int fd)//非阻塞读
{
	int opts;//记录文件描述符打开方式
	opts = fcntl(fd,F_GETFL);
	if(opts < 0)
	{
		perror("fcntl");
		return ;
	}
	opts = opts | O_NONBLOCK;

	if(fcntl(fd,F_SETFL,opts) < 0)
	{
		perror("fcntl");
		return;
	}

	return;

}

int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("para error1\n");
		return -1;
	}

	int epollFd;

	epollFd = epoll_create(EPOLLMAX);
	if(epollFd < 0)
	{
		perror("epoll_create");
		return -1;
	}

	int listenFd;

	listenFd = socket(AF_INET,SOCK_STREAM,0);
	if(listenFd < 0)
	{
		perror("socket");
		return -1;
	}
	
	struct epoll_event ev;
	ev.data.fd = listenFd;
	ev.events = EPOLLIN | EPOLLET;
	
	epoll_ctl(epollFd,EPOLL_CTL_ADD,listenFd,&ev);

	struct sockaddr_in serverAddr;
	memset(&serverAddr,0,sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret;

	ret = bind(listenFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
	if(ret < 0)
	{
		perror("bind");
		return -1;
	}
	
	ret = listen(listenFd,EPOLLMAX);

	struct epoll_event events[EPOLLMAX];//通知队列
	struct sockaddr_in clientAddr;
	int clilen = sizeof(clientAddr);
	char buff[1024];

	int nfds;//保存通知事件的个数
	int i,j;
	int clientFd;
	int sockFd;
	int cliFd[100] = {0};
	while(1)
	{
		nfds = epoll_wait(epollFd,events,EPOLLMAX,500);
		for(i = 0;i < nfds;i++);//遍历所有的通知事件
		{
			if(events[i].data.fd == listenFd)//客户端第一次连接
			{
				clientFd = accept(listenFd,(struct sockaddr *)&clientAddr,&clilen);
				char ip[48] = {0};
				for(j = 0;j < 100;j++)	
				{
					if(cliFd[j] == 0)
					{
						cliFd[j] = clientFd;//多个客户端数组保存描述符
						break;
					}
				}	
				inet_ntop(AF_INET,&clientAddr.sin_addr,ip,sizeof(ip));
				printf("connect ok:client ip = %s,port =%d\n",ip,ntohs(clientAddr.sin_port));
				setnonblocking(clientFd);
				ev.data.fd = clientFd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollFd,EPOLL_CTL_ADD,clientFd,&ev);
			}
			else if(events[i].events & EPOLLIN)//有数据可读
			{
				sockFd = events[i].data.fd;
				memset(buff,0,1024);
				ret = read(sockFd,buff,1024);
				if(ret < 0)
				{
					perror("read");
					for(j = 0;j < 100;j++)
					{
						if(cliFd[j] == sockFd)
						{
							cliFd[j] = 0;//将描述符变0;
						}
					}
					continue;
				}
				else if(ret == 0)
				{
					perror("read");
					for(j = 0;j < 100;j++)
					{
						if(cliFd[j] == sockFd)
						{
							cliFd[j] = 0;
						}
					}
					continue;
				}
				printf("recv a message:%s\n",buff);
				for(j = 0;j < 100;j++)
				{
					if((cliFd[j] != 0 && cliFd[j] != sockFd))
					{
						ret =write(cliFd[j],buff,ret);
					}
				}
				ev.data.fd = sockFd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollFd,EPOLL_CTL_MOD,sockFd,&ev);
			}
			

		}

	}

close(epollFd);
close(listenFd);
close(sockFd);
return 0;

}

