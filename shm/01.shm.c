#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define SHMKEY 1234

int main()
{
	int shmid;
	pid_t pid;
	int ret;
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
		strcpy(childAddr,"hello");
		printf("child send a message:%s\n",childAddr);

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
	printf("in parent recv a message:%s\n",buff);
	ret = shmdt(parentAddr);

	wait(&pid);

	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}
