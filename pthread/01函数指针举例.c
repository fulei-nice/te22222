#include <stdio.h>

int myAdd(int a,int b)
{
	printf("myAdd start\n");
	return a+b;
}

int myAdd1(int a,int b)
{
	printf("myAdd1 start\n");

	if(a < 0 || b < 0)
	{
		return 0;
	}

	return a+b;
}
int main()//函数指针举例
{
	int (* p)(int a,int b);
	p = myAdd;

	int num;
	num = p(1,2);//num = myAdd(1,2)
	printf("num = %d\n",num);
	p = myAdd1;
	num = p(-1,9);
	printf("num = %d\n",num);
	return 0;
}
