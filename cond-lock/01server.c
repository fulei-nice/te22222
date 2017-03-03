#include <stdio.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <mysql/mysql.h>
#include <fcntl.h>
typedef struct msghead
{
	int len;
	int msgtype;
}MSGHEAD;
typedef struct myRe
{
	char name[20];
	char passwd[20];
}MYRE_T;

#define EPOLLMAX 1000

void send_enter_result(int fd, int result, int msgtype)
{
	MSGHEAD msghead;

	int len;
	len = sizeof(msghead) + sizeof(int);
	msghead.len = len;
	msghead.msgtype = 4;//注册的返回消息

	//网络序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	result = htonl(result);

	//组装发送buff
	char buff[1024] = {0};
	memcpy(buff,&msghead,sizeof(msghead));
	memcpy(buff + sizeof(msghead), &result, sizeof(int));

	int ret;
	ret = write(fd, buff, len);
	if(ret < 0)
	{
		perror("write");
		return;
	}
}

void send_result(int fd, int result, int msgtype)
{
	MSGHEAD msghead;

	int len;
	len = sizeof(msghead) + sizeof(int);
	msghead.len = len;
	msghead.msgtype = 2;//注册的返回消息

	//网络序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	result = htonl(result);

	//组装发送buff
	char buff[1024] = {0};
	memcpy(buff,&msghead,sizeof(msghead));
	memcpy(buff + sizeof(msghead), &result, sizeof(int));

	int ret;
	ret = write(fd, buff, len);
	if(ret < 0)
	{
		perror("write");
		return;
	}
}

void enter(char *buff,int fd)
{
	MYRE_T myRe;

	memset(&myRe,0,sizeof(myRe));
	memcpy(&myRe,buff + sizeof(MSGHEAD),sizeof(myRe));//解析消息体，保存在myRe

	MYSQL db;
	MYSQL *pdb;

	mysql_init(&db);

	pdb = mysql_real_connect(&db,"127.0.0.1","root","123","class306",3306,NULL,0);
	if(pdb == NULL)
	{
		perror("connect fail");
		send_enter_result(fd, 1, 4);
		return;
	}
	else
	{
		printf("connect ok\n");
	}

//	char sql[1024] = {0};
	
//	sprintf(sql,"insert into account values(\'%s\',\'%s\');",myRe.name,myRe.passwd);
	char *sql = "select * from account;";

	int ret ;
	ret = mysql_query(pdb,sql);
	if(ret == 0)
	{
		printf("insert ok\n");
		send_enter_result(fd,0,4);
	}
	else
	{
		perror("insert fail");
		printf("sql = %s\n",sql);
		send_enter_result(fd,1,4);
	}

	MYSQL_RES *res;
	res = mysql_store_result(pdb);

	int rows;
	int column;
	int i;
	MYSQL_ROW result;
	rows = mysql_num_rows(res);
	column = mysql_num_fields(res);
	printf("登陆学生在第%d行\n",rows);
	printf("\n");
	mysql_close(pdb);
	return;
}

void myRegister(char *buff,int fd)
{
	MYRE_T myRe;

	memset(&myRe,0,sizeof(myRe));
	memcpy(&myRe,buff + sizeof(MSGHEAD),sizeof(myRe));//解析消息体，保存在myRe

	MYSQL db;
	MYSQL *pdb;

	mysql_init(&db);

	pdb = mysql_real_connect(&db,"127.0.0.1","root","123","class306",3306,NULL,0);
	if(pdb == NULL)
	{
		perror("connect fail");
		send_result(fd, 1, 2);
		return;
	}
	else
	{
		printf("connect ok\n");
	}

	char sql[1024] = {0};
	
	sprintf(sql,"insert into account values(\'%s\',\'%s\');",myRe.name,myRe.passwd);

	int ret ;
	ret = mysql_query(pdb,sql);
	if(ret == 0)
	{
		printf("insert ok\n");
		send_result(fd,0,2);
	}
	else
	{
		perror("insert fail");
		printf("sql = %s\n",sql);
		send_result(fd,1,2);
	}

	mysql_close(pdb);
	return;
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
	int i , j;
	int clientFd;
	int sockFd;
	MSGHEAD msghead;
	MYRE_T myRe;

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
	
				//解析buff数据

				memcpy(&msghead,buff,sizeof(msghead));

				//网络转换

				msghead.len = ntohl(msghead.len);
				msghead.msgtype = ntohl(msghead.msgtype);

				if(msghead.msgtype == 1)//注册消息
				{
					myRegister(buff,sockFd);
				}
				else if(msghead.msgtype == 3)//登陆消息
				{
					enter(buff,sockFd);
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
