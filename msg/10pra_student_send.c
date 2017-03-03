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
		printf("please input a ID:num = (0 is exit)\n");
		scanf("%d",&(da.num));
		if(da.num == 0)
		{
			break;
		}
		printf("please input a  name: name = \n");
		scanf("%s",da.name);

		ret = msgsnd(msgid,&da,sizeof(da),0);
		if(ret < 0)
		{
			perror("msgsnd");
			return -1;
		}
		printf("发送学生的学号%d,姓名%s\n",da.num,da.name);
	}
//	msgctl(msgid,IPC_RMID,NULL);
	return 0;
}
