#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t lock_flag;
int flag = 1;//竖
int num = 1;//横
int ret;
int fd1;
int fd2;
int fd3;
int fd4;
char buff1[1024]={0};
char buff2[1024]={0};
char buff3[1024]={0};
char buff4[1024]={0};

void *handler1(void *arg)
{
	ret = write(fd1,"A.",2);
	int i = 0;
	while(1)
	{
		if(i >= 20)
		{
			sleep(1);
			ret = read(fd1,buff1,1023);
			printf("%s\n",buff1);	
			printf("fd1 give over\n");
			break;
		}
		pthread_mutex_lock(&lock_flag);
		if(flag == 1)
		{
			flag = 2;
			if(num == 1)
			{
				ret = write(fd1,"1",1);
				num = 2;
			i++;
			}
			if(num == 2)
			{
				ret = write(fd1,"2",1);
				num =3;
			i++;
			}
			if(num == 3)
			{
				ret = write(fd1,"3",1);
				num =4;
			i++;
			}
			if(num == 4)
			{
				ret = write(fd1,"4",1);
				num =1;
			i++;
			}
		}
		pthread_mutex_unlock(&lock_flag);
	}	
}
void *handler2(void *arg)
{
	ret = write(fd2,"B.",2);
	int i = 0;
	while(1)
	{
		if(i >= 20)
		{
			sleep(1);
			ret = read(fd2,buff2,1023);
			printf("%s\n",buff2);	
			printf("fd2 give over\n");
			break;
		}
		pthread_mutex_lock(&lock_flag);
		if(flag == 2)
		{
			flag = 3;
			if(num == 1)
			{
				ret = write(fd2,"2",1);
				num = 2;
			i++;
			}
			if(num == 2)
			{
				ret = write(fd2,"3",1);
				num =3;
			i++;
			}
			if(num == 3)
			{
				ret = write(fd2,"4",1);
				num =4;
			i++;
			}
			if(num == 4)
			{
				ret = write(fd2,"1",1);
				num =1;
			i++;
			}
		}
		pthread_mutex_unlock(&lock_flag);
	}	
}
void *handler3(void *arg)
{
	ret = write(fd3,"C.",2);
	int i = 0;
	while(1)
	{
		if(i >= 20)
		{
			sleep(1);
			ret = read(fd3,buff3,1023);
			printf("%s\n",buff3);	
			printf("fd3 give over\n");
			break;
		}
		pthread_mutex_lock(&lock_flag);
		if(flag == 3)
		{
			flag = 4;
			if(num == 1)
			{
				ret = write(fd3,"3",1);
				num = 2;
			i++;
			}
			if(num == 2)
			{
				ret = write(fd3,"4",1);
				num =3;
			i++;
			}
			if(num == 3)
			{
				ret = write(fd3,"1",1);
				num =4;
			i++;
			}
			if(num == 4)
			{
				ret = write(fd3,"2",1);
				num =1;
			i++;
			}
		}
		pthread_mutex_unlock(&lock_flag);
	}	
}
void *handler4(void *arg)
{
	ret = write(fd4,"D.",2);
	int i = 0;
	while(1)
	{
		if(i >= 20)
		{
			sleep(1);
			ret = read(fd4,buff4,1023);
			printf("%s\n",buff4);	
			printf("fd4 give over\n");
			break;
		}
		pthread_mutex_lock(&lock_flag);
		if(flag == 4)
		{
			flag = 1;
			if(num == 1)
			{
				ret = write(fd4,"4",1);
				num = 2;
			i++;
			}
			if(num == 2)
			{
				ret = write(fd4,"1",1);
				num =3;
			i++;
			}
			if(num == 3)
			{
				ret = write(fd4,"2",1);
				num =4;
			i++;
			}
			if(num == 4)
			{
				ret = write(fd4,"3",1);
				num =1;
			i++;
			}
		}
		pthread_mutex_unlock(&lock_flag);
	}	
}

int main()
{
	pthread_t pid1;
	pthread_t pid2;
	pthread_t pid3;
	pthread_t pid4;

	fd1 = open("A.txt",O_RDWR |O_APPEND | O_TRUNC | O_CREAT,0640);
	fd2 = open("B.txt",O_RDWR |O_APPEND | O_TRUNC | O_CREAT,0640);
	fd3 = open("C.txt",O_RDWR |O_APPEND | O_TRUNC | O_CREAT,0640);
	fd4 = open("D.txt",O_RDWR |O_APPEND | O_TRUNC | O_CREAT,0640);
	pthread_mutex_init(&lock_flag,NULL);

	ret =pthread_create(&pid1,NULL,handler1,NULL);
	if(ret < 0)
	{
		perror("pthread create");
		return -1;
	}
	ret =pthread_create(&pid2,NULL,handler2,NULL);
	if(ret < 0)
	{
		perror("pthread create");
		return -1;
	}
	ret =pthread_create(&pid3,NULL,handler3,NULL);
	if(ret < 0)
	{
		perror("pthread create");
		return -1;
	}
	ret =pthread_create(&pid4,NULL,handler4,NULL);
	if(ret < 0)
	{
		perror("pthread create");
		return -1;
	}
	
	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	pthread_join(pid3,NULL);
	pthread_join(pid4,NULL);

	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);

	return 0;
}
