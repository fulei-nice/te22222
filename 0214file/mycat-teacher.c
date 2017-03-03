#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main(int argc,char ** argv)//./cat filename
{
	if(argc != 2)//入参的判断
	{
		printf("para error\n");
		return -1;
	}
	int fd;

	fd = open(argv[1],O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	char buff[1024] ={0};
	int ret;
	
	while(ret = read(fd,buff,1023))
	{
		printf("%s",buff);
		memset(buff,0,1024);//清空buff为0
	}
	close(fd);
	return 0;
}
