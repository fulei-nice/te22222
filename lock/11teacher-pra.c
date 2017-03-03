#include <stdio.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t lock_flag;
int flag = 1;

void *handler(void *arg)
{
	int *p = (int *)arg;
	int count = 0;
	while(1)
	{
		if(count >= 10)
		{
			break;
		}
	switch(*p)
	{
		pthread_mutex_lock(&lock_flag);
		case 0:
		{
			if(flag == 1)
			{
				printf("handler is runnig1111: A\n");
				count++;
				flag = 2;
			}
		}
		break;
		case 1:
		{
			if(flag == 2)
			{
				printf("handler is running2222: B\n");
				count++;
				flag = 3;
			}
		}
		break;
		case 2:
		{
			if(flag == 3)
			{
				printf("handler is running3333: C\n");
				count++;
				flag = 1;
			}
		}
		break;
		pthread_mutex_unlock(&lock_flag);
	}
	}
}

int main()
{
	pthread_t pid[3];
	int ret;
	int i;
	int num[3];

	pthread_mutex_init(&lock_flag,NULL);
	for(i = 0;i < 3;i++)
	{
		num[i] = i;
		ret = pthread_create(&pid[i],NULL,handler,&num[i]);
	}

	for(i = 0;i < 3;i++)
	{
		pthread_join(pid[i],NULL);
	}

	return 0;
}
