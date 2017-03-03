#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define MSGKEY 23456
typedef struct stu{
	int num;
	char name[20];
}STU_T;

int main()
{
	int msgid;

	msgid = msgget(MSGKEY,IPC_EXCL);
	if(msgid < 0)
	{
	msgid = msgget(MSGKEY,IPC_CREAT | 0640);
	return -1;
	}

	STU_T da;
	int ret;
	while(1)
	{
		memset(&da,0,sizeof(da));

		ret = msgrcv(msgid,&da,sizeof(da),0,0);
		if(ret < 0)
		{
			perror("msgrcv");
			return -1;
		}
		printf("recv a message: num = %d,name = %s\n",da.num,da.name);
	}
	
	return 0;
}
