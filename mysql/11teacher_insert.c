#include <stdio.h>
#include <mysql/mysql.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

typedef struct stu
{
	int id;
	char name[20];
	char sex[4];
	int age;
	char tel[13];
}STU_T;

typedef struct node
{
	STU_T stu;
	struct node * next;
}NODE;

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

	NODE * H = NULL;//链表头
	NODE * p = NULL;
	NODE * temp = NULL;

	while(1)
	{
		p = (NODE *)malloc(sizeof(NODE));
		memset(p,0,sizeof(NODE));
		printf("please input a id:\n");
		scanf("%d",&(p->stu.id));
		if(p->stu.id == 0)
		{
			free(p);
			break;
		}
		printf("please input a name:\n");
		scanf("%s",p->stu.name);

		printf("please input a sex:\n");
		scanf("%s",p->stu.sex);

		printf("please input a age:\n");
		scanf("%d",&(p->stu.age));

		printf("please input a tel:\n");
		scanf("%s",p->stu.tel);


		if(H == NULL)
		{
			H = p;
		}
		else//尾插入
		{
			temp = H;
			while(temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = p;
		}


	}
	//组装SQL
	char *buff = "set names utf8;";
	ret = mysql_query(&db,buff);

	char sql[1024] = {0};
	while(H != NULL)
	{
		temp = H;
		H = H->next;
		memset(sql,0,1024);
		sprintf(sql,"insert into students (id,name,sex,age,tel)values(%d,\'%s\',\'%s\',%d,\'%s\');",temp->stu.id,temp->stu.name,temp->stu.sex,temp->stu.age,temp->stu.tel);

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
		free(temp);
	}
	//	char * sql = "insert into students(id,name,sex,age,tel)values(NULL,'李四','男',20,'12345678901');";	


	mysql_close(&db);
	return 0;
}
