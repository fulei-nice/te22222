#include <stdio.h>
#include <mysql/mysql.h>
#include <errno.h>

int main()
{
	MYSQL db;
	MYSQL *pdb;
	int ret;

	//1.初始化
	mysql_init(&db);

	//2.链接
	pdb = mysql_real_connect(&db,"127.0.0.1","root","123","class306",3306,NULL,0);
	
	if(pdb == NULL)	
	{
		printf("connect fail\n");
	}
	else
	{
		printf("connect ok\n");
	}

	//组装SQL
	char *buff = "set names utf8;";
	ret = mysql_query(&db,buff);
	char * sql = "insert into students(id,name,sex,age,tel)values(NULL,'李四','男',20,'12345678901');";	

	//执行SQL
	ret = mysql_query(&db,sql);
	if(ret == 0)
	{
		printf("insert ok\n");
	}
	else
	{
		perror("insert fail\n");
		return -1;
	}

	mysql_close(&db);
	return 0;
}
