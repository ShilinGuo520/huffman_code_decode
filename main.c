#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

struct CTNode {
	unsigned char data;
	struct CTNode *lchild, *rchild, *parent;
};

int InitBiTree(struct CTNode **T)
{
	*T = (struct CTNode *)malloc(sizeof(struct CTNode));
	if (T == NULL)
		return -1;
	(*T)->lchild = NULL;
	(*T)->rchild = NULL;
	(*T)->data = 0;
	return 0;
}

void DestroyBiTree(struct CTNode *T)
{
	if (T != NULL)
		free(T);
}

int CreateBiTree(struct CTNode **T, unsigned char *Data)
{
	if (*Data == '\0')
		return -1;

	InitBiTree(&(*T)->lchild);
	InitBiTree(&(*T)->rchild);

	(*T)->lchild->data = *Data;
	
	if (*(++Data) == '\0') {
		(*T)->rchild->data = *Data;
	} else {
		(*T)->rchild->data = 0;
		CreateBiTree(&(*T)->rchild, Data);
	}
	
	return 0;
}

int ClearBiTree(struct CTNode *T)
{
	if ((T->rchild == NULL) && (T->lchild == NULL)) {
		return 0;
	}

	printf("%c", (T->lchild->data));

	DestroyBiTree(T->lchild);
	ClearBiTree(T->rchild);
	DestroyBiTree(T);

	return 0;
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
	int res;
	struct CTNode *root;
	res = InitBiTree(&root);

	res = CreateBiTree(&root, "ASDFGHJKL");

	res = ClearBiTree(root);

//	printf("data:%c\n", root->lchild->data);
//	printf("data:%c\n", root->rchild->lchild->data);
	return 0;
}
