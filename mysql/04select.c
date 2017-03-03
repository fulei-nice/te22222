#include <stdio.h>
#include <mysql/mysql.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	MYSQL db;
	MYSQL *pdb;

	mysql_init(&db);

	pdb = mysql_real_connect(&db,"localhost","root","123","class306",3306,NULL,0);
	if(pdb == NULL)
	{
		perror("connect fail");
		return -1;
	}
	else
	{
		printf("connect ok");
		printf("\n");
	}
	
	printf("&db = %p,pdb = %p\n",&db,pdb);
	char *buff = "set names utf8;";
	int ret ;
	ret = mysql_query(pdb,buff);//支持中文

	char *sql = "select * from students;";
	ret = mysql_query(pdb,sql);
	if(ret == 0)
	{
		printf("connect ok");
		printf("\n");
	}
	else
	{
		perror("connect fail");
		return -1;
	}

	MYSQL_RES *res;

	res = mysql_store_result(pdb);

	int rows;//行数
	int column;//列数

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		return 0;
	}

	column = mysql_num_fields(res);

	int i;
	int j;
	MYSQL_ROW result;//保存一行记录的指针
	for(i = 0; i < rows; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0; j < column; j++)
		{
			printf("%s ",result[j]);
		}
		printf("\n");
	}

	mysql_free_result(res);
	mysql_close(&db);

	return 0;
}
