#include <stdio.h>
#include <pthread.h>
#include <errno.h>

int n = 10;
pthread_mutex_t lock_n;

void *handler1(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&lock_n);
		n++;
		printf("生产商品的数量:%d\n",n);
		pthread_mutex_unlock(&lock_n);
		sleep(2);
	}
}

void *handler2(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&lock_n);
		if(n > 0)
		{
			n--;
			printf("消费商品的数量:%d\n",n);
			pthread_mutex_unlock(&lock_n);
			sleep(1);
		}
		else
		{
			printf("无库存品\n");
			pthread_mutex_unlock(&lock_n);
			sleep(1);
			continue;
		}
	}

}

int main()
{
	pthread_t pid1;
	pthread_t pid2;
	int ret;

	pthread_mutex_init(&lock_n,NULL);

	ret = pthread_create(&pid1,NULL,handler1,&n);
	if(ret < 0)
	{
		perror("pthread create");
		return -1;
	}

	ret = pthread_create(&pid2,NULL,handler2,&n);
	if(ret < 0)
	{
		perror("pthread create");
		return -1;
	}
	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	return 0;
}
