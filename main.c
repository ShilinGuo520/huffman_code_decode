#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


#define true 1
#define false 0

#define TRUE 1
#define FALSE 0

struct CTNode {
	unsigned char data;
	struct CTNode *lchild, *rchild;
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

	DestroyBiTree(T->lchild);
	ClearBiTree(T->rchild);
	DestroyBiTree(T);

	return 0;
}

int TraversalBiTree(struct CTNode *T, unsigned char *buff)
{
	if ((T->rchild == NULL) && (T->lchild == NULL)) {
		return 0;
	}

	*buff = T->lchild->data;
	TraversalBiTree(T->rchild, ++buff);

	return 0;
}

bool BiTreeEmpty(struct CTNode *T)
{
	if ((T->rchild == NULL) && (T->lchild == NULL)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

int BiTreeDepth(struct CTNode *T)
{
	int depth = 0;
	while (1)
	if ((T->rchild == NULL) && (T->lchild == NULL)) {
		return depth;
	} else {
		depth += 1;
		T = T->rchild;
	}
}



unsigned char find_max(int totile[], int num)
{
	int i = 0, j = 0;
	int p = totile[0];

	while (i < num) {
		if (p < totile[i]) {
			p = totile[i];
			j = i;
		}
		i++;
	}

	totile[j] = 0;
	return j;
}

void ascii_count(const char *path, unsigned char buff[])
{
	int q_file;
	int i = 0;
	unsigned char temp;
	int totile[256] = {0};

	q_file = open(path, O_RDONLY);

	while(read(q_file, &temp, 1)) {
		totile[temp] += 1;
	}

	for (i = 0; i < 256; i++) {
		buff[i] = find_max(totile, 256);
	}
}


int out_file;
unsigned char byte;
unsigned char bit_num;

void add_bit_2_file_init(const char *dec, unsigned char *header)
{
	out_file = open(dec, O_WRONLY | O_TRUNC | O_CREAT, 0x1FFF);
	write(out_file, header, 256);
	byte = 0;
	bit_num = 0;
}

void add_bit_2_file(unsigned char bit)
{
	byte |= (bit << bit_num);

	bit_num++;

	if (bit_num >= 8) {

		write(out_file, &byte, 1);
		byte = 0;
		bit_num = 0;
	}
}

void add_bit_2_file_exit(void)
{
	if (bit_num != 0) {
		write(out_file, &byte, 1);
		byte = 0;
		bit_num = 0;
	}

	close(out_file);
}

int in_file;
unsigned char in_byte;
unsigned char in_bit_num;



void read_bit_4_file_init(const char *src, unsigned char *node_buff)
{
	in_file = open(src, O_RDONLY);

	read(in_file, node_buff, 256);

	read(in_file, &in_byte, 1);
	in_bit_num = 0;
}


unsigned char read_bit_4_file(void)
{
	unsigned char ret;

	ret = (in_byte >> in_bit_num) & 0x01;

	in_bit_num++;

	if (in_bit_num >= 8) {
		if (read(in_file, &in_byte, 1)) {
			in_bit_num = 0;
		} else {
			ret = 0xFE;
		}
	}

	return ret;
}

void read_bit_4_file_exit(void)
{
	close(in_file);
}

void huffman_code(struct CTNode *root, unsigned char data)
{
	if (root->lchild->data == data) {
		//printf("%d", 1);
		add_bit_2_file(1);
		return;
	} else {
		//printf("%d", 0);
		add_bit_2_file(0);
		huffman_code(root->rchild, data);
	}
	
}

unsigned char huffman_decode(struct CTNode *root)
{
	unsigned char bit_temp;
	unsigned char ret;

	bit_temp = read_bit_4_file();
	while (1) {
		if (bit_temp) {
			ret = root->lchild->data;

			if (bit_temp > 1)
				return bit_temp;

			return ret;
		} else {
			if (root->rchild != NULL) {
				bit_temp = read_bit_4_file();
				root = root->rchild;
			}
			else 
				return -1;
		}
	}
	
	return -1;
}

void huffman_decode_str(struct CTNode *root, unsigned char *str)
{
	unsigned char temp = 0;

	temp = huffman_decode(root);
	while (temp != 0xFE) {
		*str = temp;
		str++;
		temp = huffman_decode(root);
	}
}

void huffman_code_str(struct CTNode *root, unsigned char *str)
{
	if (str == NULL)
		return ;

	while (*str) {
		huffman_code(root, *str);
		str++;
	}
}


void code(const char *src, const char *dec)
{
	int in_file;
	unsigned char temp;
        struct CTNode *root;
	unsigned char node_data[256];

        ascii_count(src, node_data);

        InitBiTree(&root);
        CreateBiTree(&root, node_data);

	add_bit_2_file_init(dec, node_data);

	in_file = open(src, O_RDONLY);


	while (1) {
		if (read(in_file, &temp, 1)) {
			huffman_code(root, temp);;
		} else {
			break;
		}
	}

	add_bit_2_file_exit();
}

void decode(const char *src, const char *dec)
{
	int out_file;
	unsigned char temp;
	unsigned char node_data[256];
	struct CTNode *root;


	read_bit_4_file_init(src, node_data);

	InitBiTree(&root);
	CreateBiTree(&root, node_data);

	out_file = open(dec, O_WRONLY | O_TRUNC | O_CREAT, 0x1FFF);

	temp = huffman_decode(root);

	while (temp != 0xFE) {
		write(out_file, &temp, 1);
		temp = huffman_decode(root);
	}

	read_bit_4_file_exit();
}

int main()
{
	code("./memory-barriers.txt", "./codetemp.hm");
	decode("./codetemp.hm", "./decode_out.txt");

	return 0;
}

