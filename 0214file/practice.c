#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{

	int fd;
	fd = open("test.log",O_WRONLY | O_CREAT,0640);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	char buff1[1024] ={0};
	
	printf("Input a string:\n");
//	scanf("%s",buff1);
	
	int ret;
	ret = read(0,buff1,1024);//键盘读取
	if( ret < 0)
	{
		perror("read");
		close(fd);
		return -1;
	}
	printf("ret = %d\n",ret);
	ret = write(1,buff1,ret-1);//显示
	/*
	ret = write(fd,buff1,10);

	if(ret < 0)
	{
		perror("write");
		close(fd);
		return -1;
	}
	int fd1;
	fd1 = open("test1.log",O_WRONLY | O_CREAT,0640);
*/
	close(fd);
	return 0;
}

