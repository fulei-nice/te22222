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
				printf("please input a message type:(0 is exit)\n");
			scanf("%ld",&(msg.msgtype));
			if(msg.msgtype == 0)
			{
				break;
			}
			printf("please input a message date:\n");
			scanf("%s",msg.buff);

			ret = msgsnd(msgid,&msg,sizeof(msg),0);
			if(ret < 0)
			{
				perror("msgsnd");
				return -1;
			}

			printf("send a message :type = %ld,date = %s\n",msg.msgtype,msg.buff);

		}
		msgctl(msgid,IPC_RMID,NULL);
		return 0;
}
