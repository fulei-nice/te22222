#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int getNUM(int *date,int num)
{
	int i,j;
	int count = 0;
	if(num < 2)
	{
		return 0;
	}
	date[0] = 2;
	count++;
	for(i = 3;i < num;i+=2)
	{
		int flag = 0;
		for(j = 2;j < i;j++)
		{
			if(i % j == 0)
			{
				flag = 1;//表示不是素数
				break;
			}
		}
		if(flag = 0)
		{
			date[count++] = i;
		}
	}
	return count;
}

int main()
{

	pid_t pid;
	int count;
	int i;
	int date[100] = {0};
	if(!(pid = fork()))
	{
		count = getNUM(date,100);
		printf("in child 100 num:");	
		for(i = 0;i < count;i++)
		{
			printf(" %d ",date[i]);	
			sleep(1);
		}

		printf("\n");
		return 0;
	}
	count = getNUM(date,50);
	printf("in parent 50 num:");	
	for(i = 0;i < count;i++)
	{
		printf(" %d ",date[i]);	
		sleep(1);
	}
	printf("\n");
	wait(&pid);
	return 0;
}
