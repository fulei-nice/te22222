#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

	pid_t pid;
	int num;
	int i;
	if(!(pid = fork()))
	{
		printf("child 100 num:");	
		for(num = 2; num < 100;num++)
		{
			int n =1;
			for(i = 2;i < num;i++)
			{
						
				if(num % i == 0 )
				{
					n = 0;
					break;
				}
			}
			if (n == 1)
			{
				printf(" %d ",num);	
			}
		}
		printf("\n");
		return 0;
	}
	printf("parent 50 num:");	
	for(num = 2; num < 50;num++)
	{
		int n = 1;
		for(i = 2;i < num;i++)
		{
			
			if(num % i == 0)
			{
				n = 0;
				break;
			}
		}
		if(n == 1)
		{
			printf(" %d ",num);	
		}
	}
	printf("\n");
	wait(&pid);
	return 0;
}
