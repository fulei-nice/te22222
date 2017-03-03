#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main(int argc,char ** argv)
{
	int fd,fd1;
	if(argc != 3 )
	{
		printf("para error\n");
		return -1;
	}
	fd = open(argv[1],O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	int ret,ret1,ret2;
	char buff[1024] = {0};
	ret = read(fd,buff,1023);//读取1.log文件内容到buff
	if(ret < 0)
	{
		perror("read");
		close(fd);
		return -1;
	}
	printf("read-ret:%d\n",ret);
	printf("read-argv[1]:\n%s\n",buff);
//复制文件操作内容
	fd1 = open(argv[2],O_RDWR | O_TRUNC | O_CREAT,0640);
	if(fd1 < 0)
	{
		perror("open");
		return -1;
	}
	
	ret1 = write(fd1,buff,ret);//将buff（1.log）内容保存到test0.log中
	if(ret1 < 0)
	{
		perror("write");
		close(fd1);
		return -1;
	}

	printf("write-argv[2]:\n%s\n",buff);
	
	close(fd);
	close(fd1);
	return 0;
}
