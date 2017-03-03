#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

typedef struct student{
	int id;
	char name[20];
}STU_T;
void *handler1(void *arg)
{
	int *num = (int *) arg;
	while(1)
	{
		printf("handler1 is running:%d\n",*num);
		sleep(1);
	}
	return NULL;
}

void *handler2(void *arg)
{
	STU_T *p = (STU_T *) arg;
	while(1)
	{
		printf("handler2 is running: id =%d,name = %s\n",p->id,p->name);
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t pid1;
	pthread_t pid2;
	int ret;
	int num = 10;

	STU_T stu;
	stu.id = 1001;
	strcpy(stu.name,"zhangshan");

	ret = pthread_create(&pid1,NULL,handler1,&num);
	if(ret < 0)
	{
		perror("pthread creat");
		return -1;
	}

	ret = pthread_create(&pid2,NULL,handler2,&stu);
	if(ret < 0)
	{
		perror("pthread creat");
		return -1;
	}
	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	return 0;
}
