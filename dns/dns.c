#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main()
{
	char *url = "www.baidu.com";
	struct hostent *phost;
	char **ppaddr;

	phost = gethostbyname(url);
	if(phost == NULL)
	{
		printf("get host error!\n");
		return -1;
	}

	char buff[48] = {0};
	for(ppaddr = phost->h_addr_list;*ppaddr != NULL;ppaddr++)
	{
		inet_ntop(AF_INET,*ppaddr,buff,sizeof(buff));
		printf("ip = %s\n",buff);
	}
	return 0;
}
