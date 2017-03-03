#include "threadpool.h"

void *workhandler(void * arg)
{
	int *p = (int *)arg;
	printf("workhandler is runing:%d\n", *p);

	return NULL;
}

int main()
{
	//1.初始化建立线程池
	pool_init(5);

	//2.往线程池添加任务
	int ret;
	int num[5];
	int i;

	sleep(3);
	for(i = 0; i < 5; i++)
	{
		num[i] = i;
	    ret = pool_add_worker(workhandler, &num[i]);
	}

	sleep(5);
	pool_destroy();

	return 0;
}
