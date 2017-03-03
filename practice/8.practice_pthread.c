#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *handlerA(void *arg)
{
	exit(0);
	while(1)
	{
		printf("handlerA is running:\n");
		sleep(1);
	}
}
void *handlerB(void *arg)
{
	while(1)
	{
		printf("handlerB is running:\n");
		sleep(1);
	}
}
void *handlerC(void *arg)
{
	while(1)
	{
		printf("handlerC is running:\n");
		sleep(1);
	}
}

int main()

{
	pthread_t pid[3];

	int ret;

	ret = pthread_create(&pid[0],NULL,handlerA,NULL);
	ret = pthread_create(&pid[1],NULL,handlerB,NULL);
	ret = pthread_create(&pid[2],NULL,handlerC,NULL);

//	return 0;
	pthread_join(pid[0],NULL);
	pthread_join(pid[1],NULL);
	pthread_join(pid[2],NULL);

	return 0;
}

