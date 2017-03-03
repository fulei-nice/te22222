#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

typedef struct student{
	int id;
	char name[20];
}STU_T;
void *handler1(void *arg)
{	
	int *num = (int *)arg;
	printf("handler1 arg = %p\n",arg);
	while(1)
	{
	if(*num >= 10)
	{
		break;
	}
	(*num)++;
	printf("hangdler1 num = %d\n",*num);
	sleep(1);
	}

	int *date = (int *)malloc(sizeof(int));
	*date = 666;
	pthread_exit((void *)date);
	return NULL; 
}

void *handler2(void *arg)
{
	int *num = (int *)arg;
	printf("handler2 arg = %p\n",arg);
	while(1)
	{
		if(*num >= 10)
		{
			break;
		}
	(*num)--;
	printf("hangdler2 num = %d\n",*num);
	sleep(2);
	}
	

	STU_T *p = (STU_T *)malloc(sizeof(STU_T));
	p->id = 1006;
	strcpy(p->name,"lisi");
	pthread_exit((void *)p);
	return NULL;
}

int main()
{
	pthread_t pid1;
	pthread_t pid2;
	int ret1;
	int ret2;
	int i = 0;
	

	printf("i = %p\n",&i);
	ret1 = pthread_create(&pid1,NULL,handler1,&i);
	if(ret1 < 0)
	{
		perror("pthread create");
		return -1;
	}

	ret2 = pthread_create(&pid2,NULL,handler2,&i);
	if(ret2 < 0)
	{
		perror("pthread create");
		return -1;
	}
	int *date;
	STU_T *p;
	pthread_join(pid1,(void **)&date);
	printf("in main date = %d\n",*date);
	pthread_join(pid2,(void **)&p);
	printf("in main STU_T id = %d,name =%s\n",p->id,p->name);
	return 0;
}
