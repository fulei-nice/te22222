#include <stdio.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t lock_i;
int i =0;


void *handlerA(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&lock_i);
		i++;
		printf("handlerA:%d\n",i);
		pthread_mutex_unlock(&lock_i);
		sleep(1);
	}
}
void *handlerB(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&lock_i);
		i++;
		printf("handlerB:%d\n",i);
		pthread_mutex_unlock(&lock_i);
		sleep(1);
	}
}


int main()
{
	pthread_t pidA;
	pthread_t pidB;

	int ret;
	pthread_mutex_init(&lock_i,NULL);

		ret = pthread_create(&pidA,NULL,handlerA,NULL);
	if(ret < 0)
	{
		perror("pthread create");
		return -1;
	}
	ret = pthread_create(&pidB,NULL,handlerB,NULL);
	if(ret < 0)
	{
		perror("pthread create");
		return -1;
	}

	pthread_join(pidA,NULL);
	pthread_join(pidB,NULL);
	return 0;
}
