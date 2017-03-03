#include <stdio.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define EPOLLMAX 1000

pthread_mutex_t lock;

typedef struct stu
{
	int id;
	char name[20];
}STU_T;
typedef struct node
{
	STU_T st;
	struct stu * next;
}NODE;

NODE * H = NULL;//链表头

void *handler(void *arg)
{
	NODE *p = NULL;
	while(1)
	{
		pthread_mutex_lock(&lock);
		if(H != NULL)
		{
			//摘链
			p = H;
			H = H->next;
			printf("recv student message: id = %d, name = %s\n",ntohl(p->st.id),p->st.name);
		}
		free(p);//释放节点
	}
		pthread_mutex_unlock(&lock);
	sleep(1);
}

int main(int argc,char **argv)
{
	int ret;
	if(argc != 2)
	{
		printf("para error1\n");
		return -1;
	}

	int epollFd;
	pthread_mutex_init(&lock,NULL);
	pthread_t pid;

	ret = pthread_create(&pid,NULL,handler,NULL);

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
	char buff[1024] = {0};
	int nfds;//保存通知事件的个数
	int i;
	int clientFd;
	int sockFd;
	NODE *p = NULL;
	STU_T st;
	while(1)
	{
		nfds = epoll_wait(epollFd,events,EPOLLMAX,500);
		for(i = 0;i < nfds;i++);//遍历所有的通知事件
		{
			if(events[i].data.fd == listenFd)//客户端第一次连接
			{
				clientFd = accept(listenFd,(struct sockaddr *)&clientAddr,&clilen);
				ev.data.fd = clientFd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollFd,EPOLL_CTL_ADD,clientFd,&ev);
			}
			else if(events[i].events & EPOLLIN)//有数据可读
			{
				sockFd = events[i].data.fd;
				memset(&st,0,sizeof(st));

				//buff
				ret = read(sockFd,buff,sizeof(buff));
				if(ret <= 0)
				{
					perror("read");
					continue;
				}
				//memcpy(&st,buff,sizeof(st));

				//1.创建节点 保存数据
				p = (NODE *)malloc(sizeof(NODE));
				memset(p,0,sizeof(NODE));
				//2.赋值
				memcpy(&(p->st),buff,sizeof(STU_T));
				//3.添加i
				pthread_mutex_lock(&lock);
				if(H == NULL)//空表
				{
					H = p;
				}
				else//非空
				{
					p->next = H;
					H = p;
				}
				pthread_mutex_unlock(&lock);

				ev.data.fd = sockFd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollFd,EPOLL_CTL_MOD,sockFd,&ev);
			}


		}

	}

	pthread_join(pid,NULL);
	close(epollFd);
	close(listenFd);
	close(sockFd);
	return 0;

}

