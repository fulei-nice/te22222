#include <stdio.h>

typedef struct node
{
	int num;
	char name[20];
	struct node *next;
}NODE;

int main()
{
	NODE *H = NULL;
	NODE *temp = H;
	NODE *p;

	p = (NODE *)malloc(sizeof(NODE));

	if(H == NULL)
	{
		H = p;
	}
	

}
