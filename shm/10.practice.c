#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define SHMKEY 1234
int main()
{
	pid_t pid;
	int ret;
	int shmid;

	shmid = shmget(SHMKEY,1024,IPC_EXCL);
	if(shmid < 0)
	{
		shmid = shmget(SHMKEY,1024,IPC_CREAT | 0640);
		if(shmid < 0)
		{
			perror("shmget");
			return -1;
		}
	}

	if(!(pid = fork()))
	{
		//child
		char *childAddr;
		childAddr = shmat(shmid,0,0);
		if(childAddr == NULL)
		{
			perror("shmat");
			return -1;
		}

		strcpy(childAddr,"hello parent");
//		printf("child send a massage:%s\n",childAddr);

		sleep(2);
		char buffer[1024] = {0};
		strcpy(buffer,childAddr);
		printf("child recv a message:%s\n",buffer);

		memset(childAddr,0,1024);
		strcpy(childAddr,"sleep");
//		printf("child send a message:%s\n",childAddr);

		sleep(3);
		memset(childAddr,0,1024);
		strcpy(buffer,childAddr);
		printf("child recv a message:%s\n",buffer);

		ret = shmdt(childAddr);


		return 0;
	}

	//parent
	sleep(2);
	char *parentAddr;
	parentAddr = shmat(shmid,0,0);
	if(parentAddr == NULL)
	{
		perror("shmat");
		return -1;
	}
	char buff[1024]={0};
	strcpy(buff,parentAddr);
	printf("parent recv a message:%s\n",buff);

	memset(parentAddr,0,1024);
	strcpy(parentAddr,"hello child");
	//	printf("parent send a message:%s\n",parentAddr);

	sleep(2);
	memset(parentAddr,0,1024);
	strcpy(buff,parentAddr);
	printf("parent recv a message:%s\n",buff);

	memset(parentAddr,0,1024);
	strcpy(parentAddr,"ok");

	ret = shmdt(parentAddr);
	wait(&pid);

	shmctl(shmid,IPC_RMID,NULL);

	return 0;

}
