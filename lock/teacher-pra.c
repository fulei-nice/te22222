#include <stdio.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t lock_flag;
int flag = 1;//1,A;2,B;3,C;

void *handlerA(void *arg)
{
	int count = 0;
	while(1)
	{
		if(count >=10)
		{
			break;
		}
		pthread_mutex_lock(&lock_flag);
		if(flag == 1)
		{
			printf("A");
			flag = 2;
		count++;
		}
		pthread_mutex_unlock(&lock_flag);
	}
	return NULL;
}
void *handlerB(void *arg)
{
	int count = 0;
	while(1)
	{
		if(count >=10)
		{
			break;
		}
		pthread_mutex_lock(&lock_flag);
		if(flag == 2)
		{
			printf("B");
			flag = 3;
		count++;
		}
		pthread_mutex_unlock(&lock_flag);
	}
	return NULL;
}
void *handlerC(void *arg)
{
	int count = 0;
	while(1)
	{
		if(count >=10)
		{
			break;
		}
		pthread_mutex_lock(&lock_flag);
		if(flag == 3)
		{
			printf("C");
			flag = 1;
		count++;
		}
		pthread_mutex_unlock(&lock_flag);
	}
	return NULL;
}

int main()
{
	pthread_t pidA;
	pthread_t pidB;
	pthread_t pidC;
	int ret;
	pthread_mutex_init(&lock_flag,NULL);

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
	ret = pthread_create(&pidC,NULL,handlerC,NULL);
	if(ret < 0)
	{
		perror("pthread create");
		return -1;
	}

	pthread_join(pidA,NULL);
	pthread_join(pidB,NULL);
	pthread_join(pidC,NULL);
	return 0;
}
