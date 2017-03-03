#include <stdio.h>
#include <pthread.h>
#include <errno.h>

int num = 0;

void *handler(void *arg)
{
	int j,n;
	while(1)
	{
		if(num == 50)
		{
			break;
		}
	for(j = 0,n = 0;j < 10;j++)
	{
		n++;
		printf("子线程循环%2d次\n",n);
		usleep(1000*10);
	}
		if(j == 10)
		break;

	}
}

int main()
{

	pthread_t pid;
	int ret;
	int i ,m = 0;	
	while(1)
	{
		printf("大循环------第%d次\n",num);
		ret = pthread_create(&pid,NULL,handler,NULL);
		if(ret < 0)
		{
			perror("pthread create");
			return -1;
		}
		pthread_join(pid,NULL);

		if(num == 50)
		{
			break;
		}
		for(i = 0,m = 0;i < 100;i++)
		{
			m++;
			printf("主线程循环%2d次\n",m);
			usleep(1000*10);
			if(i == 100)
			break;
		}
		num++;
	}
	return 0;

}
