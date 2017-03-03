#include <stdio.h>
#include <mysql/mysql.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
	int id;
	char name[24];
	char sex[4];
	int age;
	char tel[13];
}Data;
typedef struct stu
{
	Data va;
	struct stu * next;
}STU_T;

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
	STU_T *h = NULL;//链表头
	STU_T *p = NULL;
	STU_T *s = NULL;	
	STU_T da;
	char buff1[1024] = {0};


	while(1)
	{
		p = (STU_T *)malloc(sizeof(STU_T));
		memset(p,0,sizeof(STU_T));

		printf("input id (0 is exit):\n");
		scanf("%d",&(p->va.id));
		if(p->va.id == 0)
		{
			free(p);
			break;
		}
		printf("input name:\n");
		scanf("%s",p->va.name);
		printf("input sex:\n");
		scanf("%s",p->va.sex);
		printf("input age:\n");
		scanf("%d",&(p->va.age));
		printf("input tel:\n");
		scanf("%s",p->va.tel);

		if(h == NULL)
		{
			h = p;
		}
		else 
		{
			s = h;
			while(s->next != NULL)
			{
				s = s->next;
			}
			s->next = p; 
		}


	}
	//	printf("id = %d,name = %s,sex = %s,age = %d,tel = %s",p->va.id,p->va.name,p->va.sex,p->va.age,p->va.tel);

	char *buff = "set names utf8;";
	ret = mysql_query(&db,buff);

	while(h != NULL)
	{
		s = h;
		h = h->next;
		memset(buff1,0,1024);
		sprintf(buff1,"insert into students(id,name,sex,age,tel)values(%d,\'%s\',\'%s\',%d,\'%s\');",s->va.id,s->va.name,s->va.sex,s->va.age,s->va.tel);


		//	char * sql = "insert into students(id,name,sex,age,tel)values(NULL,'李四','男',20,'12345678901');";	

		ret = mysql_query(&db,buff1);
		if(ret == 0)
		{
			printf("insert ok\n");
		}
		else
		{
			perror("insert fail\n");
			return -1;
		}
		free(s);
	}
	//执行SQL

	mysql_close(&db);
	return 0;
}
