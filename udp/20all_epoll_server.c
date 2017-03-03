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
#include "threadpool.h"

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

typedef struct msg
{
	char buff[1024];
	int fd;
}MSG_T;

#define EPOLLMAX 1000

void send_result1(int fd, int result, int msgtype,char *pbuff)
{
	MSGHEAD msghead;

	int len;
	len = sizeof(msghead) + sizeof(int) + strlen(pbuff);
	msghead.len = len;
	msghead.msgtype = msgtype;//返回消息

	//网络序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	result = htonl(result);

	//组装发送buff
	char buff[1024] = {0};
	memcpy(buff,&msghead,sizeof(msghead));
	memcpy(buff + sizeof(msghead), &result, sizeof(int));
		
	memcpy(buff + sizeof(msghead) + sizeof(int),pbuff,strlen(pbuff));

	int ret;
	ret = write(fd, buff, len);
	if(ret < 0)
	{
		perror("write");
		return;
	}
}
void send_result2(int fd, int result, int msgtype,MYRE_T *pmyRe)
{
	MSGHEAD msghead;

	int len;
	len = sizeof(msghead) + sizeof(int) + sizeof(MYRE_T);
	msghead.len = len;
	msghead.msgtype = msgtype;//返回消息

	//网络序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	result = htonl(result);

	//组装发送buff
	char buff[1024] = {0};
	memcpy(buff,&msghead,sizeof(msghead));
	memcpy(buff + sizeof(msghead), &result, sizeof(int));
		
	memcpy(buff + sizeof(msghead) + sizeof(int),pmyRe,sizeof(MYRE_T));

	int ret;
	ret = write(fd, buff, len);
	if(ret < 0)
	{
		perror("write");
		return;
	}
}

void send_result3(int fd, int result, int msgtype,int num,MYRE_T *myRe)
{
	MSGHEAD msghead;

	int len;
	len = sizeof(msghead) + sizeof(int) + num * sizeof(myRe);
	msghead.len = len;
	msghead.msgtype = msgtype;//返回消息

	//网络序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	result = htonl(result);
	num = htonl(result);
	
	//组装发送buff
	char buff[1024] = {0};
	memcpy(buff,&msghead,sizeof(msghead));
	memcpy(buff + sizeof(msghead), &result, sizeof(int));
	memcpy(buff + sizeof(msghead) + sizeof(int),myRe,num * sizeof(myRe));

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
	msghead.msgtype = msgtype;//返回消息

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

void login(char *buff,int fd)
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
		send_result(fd, 1, 4);
		return;
	}
	else
	{
		printf("connect ok\n");
	}

	int ret ; 	
	char *buffer = "set names utf8;";
	ret = mysql_query(&db,buffer);
	char sql[1024] = {0};

	sprintf(sql,"select * from account where name = \'%s\' and passwd = \'%s\';",myRe.name,myRe.passwd);

	ret = mysql_query(pdb,sql);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		perror("select fail");
		printf("sql = %s\n",sql);
		send_result(fd,1,4);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int row;
	row = mysql_num_rows(res);

	if(row == 0)
	{
		send_result(fd,1,4);
	}
	else
	{
		send_result(fd,0,4);
	}

	mysql_free_result(res);
	mysql_close(pdb);
	return;
}

void sele2(char *buff,int fd)//用户查询
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
		send_result(fd, 1, 8);
		return;
	}
	else
	{
		printf("connect ok\n");
	}

	int ret ; 	
	char *buffer = "set names utf8;";
	ret = mysql_query(&db,buffer);
	char sql[1024] = {0};

	sprintf(sql,"select * from account where name = \'%s\';",myRe.name);

	ret = mysql_query(pdb,sql);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		perror("select fail");
		printf("sql = %s\n",sql);
		send_result(fd,1,8);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int row;
	row = mysql_num_rows(res);

	if(row == 0)
	{
		send_result(fd,1,8);
		mysql_free_result(res);
		mysql_close(pdb);
		return;
	}

	int column;
	column = mysql_num_fields(res);

	MYSQL_ROW result;

	result = mysql_fetch_row(res);

	MYRE_T temp;
	memset(&temp,0,sizeof(temp));

	strcpy(temp.name,result[0]);
	strcpy(temp.passwd,result[1]);

	send_result2(fd,0,8,&temp);

	mysql_free_result(res);
	mysql_close(pdb);
	return;
}

void sele(int fd)//buff查询
{
	MYSQL db;
	MYSQL *pdb;

	mysql_init(&db);

	pdb = mysql_real_connect(&db,"127.0.0.1","root","123","class306",3306,NULL,0);
	if(pdb == NULL)
	{
		perror("connect fail");
		send_result(fd, 1, 6);
		return;
	}
	else
	{
		printf("connect ok\n");
	}

	int ret ; 	
	char *buffer = "set names utf8;";
	ret = mysql_query(&db,buffer);
	char *sql = "select * from account;";

	ret = mysql_query(pdb,sql);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		perror("select fail");
		printf("sql = %s\n",sql);
		send_result(fd,1,6);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);

	int row;

	row = mysql_num_rows(res);
	if(row == 0)
	{
		send_result(fd,1,6);
		mysql_free_result(res);
		mysql_close(pdb);
		return ;
	}
		
	int column;//列数
	column = mysql_num_fields(res);
	char buff[1000] = {0};

	MYSQL_ROW result;
	int i;
	int j;
	for(i = 0; i < row; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0; j < column; j++)
		{
			printf("%10.10s",result[j]);
			sprintf(buff,"%s%10.10s",buff,result[j]);
		}
		printf("\n");
		sprintf(buff,"%s%s",buff,"\n");
	}
	
	send_result1(fd,0,6,buff);

	mysql_free_result(res);
	mysql_close(pdb);
	return;
}

void sele3(int fd)//结构体查询
{
	MYSQL db;
	MYSQL *pdb;
	
	mysql_init(&db);

	pdb = mysql_real_connect(&db,"127.0.0.1","root","123","class306",3306,NULL,0);
	if(pdb == NULL)
	{
		perror("connect fail");
		send_result(fd, 1, 10);
		return;
	}
	else
	{
		printf("connect ok\n");
	}

	int ret ; 	
	char *buffer = "set names utf8;";
	ret = mysql_query(&db,buffer);

	char *sql = "select * from account;";

	ret = mysql_query(pdb,sql);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		perror("select fail");
		printf("sql = %s\n",sql);
		send_result(fd,1,10);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);

	int row;

	row = mysql_num_rows(res);
	if(row == 0)
	{
		send_result(fd,1,10);
		mysql_free_result(res);
		mysql_close(pdb);
		return ;
	}
	
	MYRE_T *myRe = (MYRE_T *)malloc(sizeof(MYRE_T) * row);

	int column;//列数
	column = mysql_num_fields(res);

	MYSQL_ROW result;
	int i;
	int j;
	for(i = 0; i < row; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0; j < column; j++)
		{
			printf("%10.10s",result[j]);
			if(j == 0)
			{
			strcpy(myRe[i].name,result[0]);
			}
			if(j == 1)
			{
			strcpy(myRe[i].passwd,result[1]);
			}
		}
		printf("\n");
	}
/*	for(i = 0 ; i < row ; i++)
	{
		printf("%s  %s\n",myRe[i].name,myRe[i].passwd);
	}
*/	
	int num;
	num = row;	
	send_result3(fd,0,10,num,myRe);

	mysql_free_result(res);
	mysql_close(pdb);
	free(myRe);
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

	int ret ; 	
	char *buffer = "set names utf8;";
	ret = mysql_query(&db,buffer);

	char sql[1024] = {0};

	sprintf(sql,"insert into account values(\'%s\',\'%s\');",myRe.name,myRe.passwd);

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
void *workhandler(void *arg)
{
	MSGHEAD msghead;
	MSG_T *pmsg = (MSG_T *)arg;
	//解析buff数据

	memcpy(&msghead,pmsg->buff,sizeof(msghead));

	//网络转换

	msghead.len = ntohl(msghead.len);
	msghead.msgtype = ntohl(msghead.msgtype);

	if(msghead.msgtype == 1)//注册消息
	{
		myRegister(pmsg->buff,pmsg->fd);
	}
	else if(msghead.msgtype == 3)//登陆消息
	{
		login(pmsg->buff,pmsg->fd);
	}
	else if(msghead.msgtype == 5)//buff查询消息
	{
		sele(pmsg->fd);
	}
	else if(msghead.msgtype == 7)//用户查询消息
	{
		sele2(pmsg->buff,pmsg->fd);
	}
	else if(msghead.msgtype == 9)//结构体查询消息
	{
		sele3(pmsg->fd);
	}

	free(pmsg);
	pmsg = NULL;
	return NULL;
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
	MSGHEAD msghead;
	MYRE_T myRe;

	MSG_T *pmsg;

	pool_init(10);
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

				pmsg = (MSG_T *)malloc(sizeof(MSG_T));
				memset(pmsg,0,sizeof(MSG_T));
				memcpy(pmsg->buff,buff,ret);

				pmsg->fd = sockFd;

				pool_add_worker(workhandler,pmsg);

				ev.data.fd = sockFd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollFd,EPOLL_CTL_MOD,sockFd,&ev);

			}
		}
	}
				sleep(5);
				pool_destroy();
	colse(epollFd);
	close(listenFd);
	close(sockFd);
	return 0;
}
