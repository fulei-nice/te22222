#include <stdio.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define EPOLLMAX 1000

void *handler1(void *arg)
{
	int *p = (int *)arg;
	int fd = *p;
	write(fd,"ok",2);
	printf("send a message:ok!\n");
}
void *handler2(void *arg)
{
	int *p = (int *)arg;
	int fd = *p;
	write(fd,"quit",4);
	printf("send a message:quit!\n");
}

int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("para error\n");
		return -1;
	}

	int listenFd;

	listenFd = socket(AF_INET,SOCK_STREAM,0);
	if(listenFd < 0)
	{
		perror("socket");
		return -1;
	}

	int epollFd;

	epollFd = epoll_create(EPOLLMAX);
	if(epollFd < 0)
	{
		perror("epoll_create");
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
	if(ret < 0)
	{
		perror("listen");
		return -1;
	}

	struct epoll_event events[EPOLLMAX];
	struct sockaddr_in clientAddr;
	int clilen = sizeof(clientAddr);
	char buff[1024];
	int nfds;
	int i;
	int clientFd;
	int sockFd;
	pthread_t pid1;
	pthread_t pid2;
	while(1)
	{
		nfds = epoll_wait(epollFd,events,EPOLLMAX,500);
		for(i = 0;i < nfds;i++)
		{
			if(events[i].data.fd == listenFd )
			{
				clientFd = accept(listenFd,(struct sockaddr *)&clientAddr,&clilen);
				ev.data.fd = clientFd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollFd,EPOLL_CTL_ADD,clientFd,&ev);
			}

			else if(events[i].events & EPOLLIN)
			{
				sockFd = events[i].data.fd;
				memset(buff,0,1024);
				ret = read(sockFd,buff,1024);
				if(ret <= 0)
				{
					perror("read");
					continue;
				}
				printf("recv a message:%s\n",buff);
				if(strcmp("hello",buff) == 0)
				{
					ret = pthread_create(&pid1,NULL,handler1,&sockFd);
				}
				if(strcmp("sleep",buff) == 0)
				{
					ret = pthread_create(&pid2,NULL,handler2,&sockFd);
				}
				ev.data.fd = sockFd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollFd,EPOLL_CTL_MOD,sockFd,&ev);
			}
		}
	}
	colse(epollFd);
	close(listenFd);
	close(sockFd);
	return 0;
}
