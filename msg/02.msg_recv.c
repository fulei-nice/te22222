#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#define MSGKEY 12345
typedef struct msg{
	long msgtype;
	char buff[48];
}MSG_T;

int main()
{

	int msgid;

	msgid = msgget(MSGKEY,IPC_EXCL);
	if(msgid < 0)
	{
		msgid = msgget(MSGKEY,IPC_CREAT | 0640);
		if(msgid < 0)
		{
			perror("msgget");
			return -1;
		}
	}
		MSG_T msg;
		int ret ;
		while(1)
		{
			memset(&msg,0,sizeof(msg));
			ret = msgrcv(msgid,&msg,sizeof(msg),1,0);//只能接受数据为1的消息
			if(ret < 0)
			{
				perror("msgrcv");
				return -1;
			}
			printf("recv a message :type = %ld,date = %s\n",msg.msgtype,msg.buff);

	}
		return 0;
}
