#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

struct CTNode {
	unsigned char data;
	struct CTNode *lchild, *rchild, *parent;
};

int InitBiTree(struct CTNode *T)
{
	T = (struct CTNode *)malloc(sizeof(struct CTNode));
	if (T == NULL)
		return -1;
	T->lchild = NULL;
	T->rchild = NULL;
	T->data = 0;
	return 0;
}

void DestroyBiTree(struct CTNode *T)
{
	if (T != NULL)
		free(T);
}

int CreateBiTree(struct CTNode *T, unsigned char Data)
{
	if (T == NULL)
		return -1;
	T->data = Data;
	InitBiTree(T->rchild);
	InitBiTree(T->rchild);
}

void ClearBiTree(void)
{
	;
}

bool BiTreeEmpty(void)
{
	return 0;
}

int BiTreeDepth(void)
{
	int depth = 0;
	return depth;
}


int main()
{
	printf("test\n");
	return 0;
}
