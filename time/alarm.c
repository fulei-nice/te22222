#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

void handler(int num)
{
	printf("time is comming\n");

	return ;
}

int main()
{
	signal(SIGALRM,handler);

	struct itimerval value;
	//定时器第一次到来的时间
	value.it_value.tv_sec = 3;//秒
	value.it_value.tv_usec = 0;//微妙
	//除了第一次后续定时器到来的间隔时间
//	value.it_interval.tv_sec = 3;
//	value.it_interval.tv_usec = 0;

	value.it_interval = value.it_value;

	setitimer(ITIMER_REAL,&value,NULL);

	while(1)
	{
	}

	return 0;
}

