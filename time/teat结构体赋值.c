#include <stdio.h>
#include <string.h>
typedef struct stu
{
	int id;
	char name[20];
}STU_T;

int main()
{
	STU_T st1,st2;
	st1.id = 1001;
	strcpy(st1.name,"zhangsan");

	st2 = st1;

	printf("st2.id = %d,st2.name = %s\n",st2.id,st2.name);
	return 0;
}
