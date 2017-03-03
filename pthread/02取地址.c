#include <stdio.h>

int func(int *a)

{
	*a = 10;
	return 0;
}

int main()
{

	int a = 0;

	func(&a);

	printf("a = %d\n",a);
	return 0;
}
