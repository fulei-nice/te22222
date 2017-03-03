#include <stdio.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct msghead
{
	int len;
	int msgtype;//消息类型1.注册；3.登陆
}MSGHEAD;

typedef struct myRegister
{
	char name[20];
	char passwd[20];
}MYRE_T;

void sele_result(char *buff)
{
	int result;

	memcpy(&result, buff + sizeof(MSGHEAD), sizeof(int));

	result = ntohl(result);

	if(result == 0)
	{
		printf("查询成功\n");
		printf("%s",buff + sizeof(MSGHEAD) + sizeof(int));
	}
	else
	{
		printf("查询失败\n");
	}
}
void login_result(char *buff)
{
	int result;

	memcpy(&result, buff + sizeof(MSGHEAD), sizeof(int));

	result = ntohl(result);
	if(result == 0)
	{
		printf("登陆成功\n");
	}
	else
	{
		printf("登陆失败\n");
	}

	return;
}

void register_result(char *buff)
{
	int result;

	memcpy(&result, buff + sizeof(MSGHEAD), sizeof(int));

	result = ntohl(result);
	if(result == 0)
	{
		printf("注册成功\n");
	}
	else
	{
		printf("注册失败\n");
	}

	return;
}

void *handler(void *arg)
{
	int *p = (int *)arg;
	int fd = *p;
	int ret ;
	char buff[1024];
	MSGHEAD msghead;

	while(1)
	{
		memset(buff,0,1024);
		ret = read(fd,buff,1024);
		printf("recv a msg:%d\n",ret);
		memset(&msghead,0,sizeof(msghead));
		memcpy(&msghead,buff,sizeof(msghead));

		//网络序转换

		msghead.len = ntohl(msghead.len);
		msghead.msgtype = ntohl(msghead.msgtype);
		if(msghead.msgtype == 2)//注册的返回消息
		{
			register_result(buff);
		}
		else if(msghead.msgtype == 4)//登陆返回消息
		{
			login_result(buff);
		}
		else if(msghead.msgtype == 6)//查询返回消息
		{
			sele_result(buff);
		}
	}
}

void menu1()
{
	printf("please input your chice:\n");
	printf("1.register\n");
	printf("2.longin\n");
	printf("3.select\n");

	return ;
}

void sele(int fd)
{
	
	MSGHEAD msghead;//消息头

	memset(&msghead,0,sizeof(msghead));

	int len;//保存发送数据的总长度

	len = sizeof(MSGHEAD);

	msghead.len = len;
	msghead.msgtype = 5;//查询

	//网络序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//发送数据
	int ret;
	ret = write(fd,&msghead,len);

	if(ret < 0)
	{
		perror("write");
	}

	return;
}

void login(int fd)
{
	MYRE_T myRe;//消息体
	MSGHEAD msghead;//消息头

	memset(&myRe,0,sizeof(myRe));
	memset(&msghead,0,sizeof(msghead));
	printf("please input a name:\n");
	scanf("%s",myRe.name);
	printf("please input a passwd:\n");
	scanf("%s",myRe.passwd);

	int len;//保存发送数据的总长度

	len = sizeof(MSGHEAD) + sizeof(myRe);

	msghead.len = len;
	msghead.msgtype = 3;//登陆

	//网络序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送buff
	char buff[1024] = {0};
	memcpy(buff,&msghead,sizeof(msghead));
	memcpy(buff + sizeof(msghead),&myRe,sizeof(myRe));

	//发送数据
	int ret;
	ret = write(fd,buff,len);

	if(ret < 0)
	{
		perror("write");
	}

	return ;
}
void myRegister(int fd)
{
	MYRE_T myRe;//消息体
	MSGHEAD msghead;//消息头

	memset(&myRe,0,sizeof(myRe));
	memset(&msghead,0,sizeof(msghead));
	printf("please input a name:\n");
	scanf("%s",myRe.name);
	printf("please input a passwd:\n");
	scanf("%s",myRe.passwd);

	int len;//保存发送数据的总长度

	len = sizeof(MSGHEAD) + sizeof(myRe);

	msghead.len = len;
	msghead.msgtype = 1;//注册

	//网络序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送buff
	char buff[1024] = {0};
	memcpy(buff,&msghead,sizeof(msghead));
	memcpy(buff + sizeof(msghead),&myRe,sizeof(myRe));

	//发送数据
	int ret;
	ret = write(fd,buff,len);

	if(ret < 0)
	{
		perror("write");
	}

	return ;
}

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
	int num;
	pthread_t pid;
	ret = pthread_create(&pid,NULL,handler,&sockfd);
		
		while(1)
		{
			menu1();
			scanf("%d",&num);
			if(num == 1)//z注册
			{
				myRegister(sockfd);
			}
			else if(num == 2)//登陆
			{
				login(sockfd);
			}
			else if(num == 3)//查询
			{
				sele(sockfd);
			}
		}
	pthread_join(pid,NULL);
	close(sockfd);
	return 0;

}
