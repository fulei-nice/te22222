#include <stdio.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutex_lock;
pthread_cond_t cond_lock;

int num = 0;

void *handlerA(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex_lock);
		num++;
		printf("handlerA make a product: %d\n",num);
		pthread_mutex_unlock(&mutex_lock);
		if(num > 10)
		{
			pthread_cond_signal(&cond_lock);
		}
		sleep(1);
	}
}

void *handlerB(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex_lock);
		pthread_cond_wait(&cond_lock,&mutex_lock);

		num = num - 10;

		printf("handlerB use 10 product\n");
		pthread_mutex_unlock(&mutex_lock);
	sleep(1);
	}
}

int main()
{
	pthread_mutex_init(&mutex_lock,NULL);
	pthread_cond_init(&cond_lock,NULL);

	pthread_t pid[2];
	int ret;

	ret = pthread_create(&pid[0],NULL,handlerA,NULL);
	ret = pthread_create(&pid[1],NULL,handlerB,NULL);

	pthread_join(pid[0],NULL);
	pthread_join(pid[1],NULL);

}
