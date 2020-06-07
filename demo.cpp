#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define MAX_ARRAY_NUM 5 
#define CURSOR_NUM 3 


#define list_empty(list) ((list) == (list)->next)
#define node_to_item(node, container, member) \
    (container *) (((char*) (node)) - offsetof(container, member))
#define list_for_each(node, list) \
    for (node = (list)->next; node != (list); node = node->next)


typedef struct listnode
{
    struct listnode *next;
    struct listnode *prev;
}listnode_t;

typedef struct {
	listnode_t node;
	int *pointer;
	int serialno;
	int offset;
} cursor_info;

//  参数 
int array[MAX_ARRAY_NUM];
int offsetMax = 0;
listnode cur_head;

//  list function
void ListInit(listnode_t *node)
{
	if (node == NULL) {
		return;
	}
	node->next = node;
	node->prev = node;
}

void ListAddTail(listnode_t *head, listnode_t *item)
{
	if (head == NULL || item == NULL) {
		return;
	}
	item->next = head;
	item->prev = head->prev;
	head->prev->next = item;
	head->prev = item;
}

//  初始化参数 
int Init(int num)
{
	int i  = 0;
	int sn = 1;	
		
	cursor_info *c_info = NULL;
	c_info = (cursor_info *)malloc(sizeof(cursor_info));
	if (!c_info) {
		printf("malloc failed\n");
		return -1;
	}
	memset((void *)c_info, 0, sizeof(cursor_info));
	ListInit(&cur_head);
	
	for (i = 0; i < MAX_ARRAY_NUM; i++) {
		array[i] = i+1;
		printf("[%d] = %d  ", i + 1, array[i]);
		if (i < num) {
			c_info->serialno = sn;
			c_info->offset   = 0;
			c_info->pointer  = array + i;
			ListAddTail(&cur_head, &c_info->node);
			sn ++;
			if (sn <= num) {
				c_info = (cursor_info *)malloc(sizeof(cursor_info));
				memset((void *)c_info, 0, sizeof(cursor_info));	
				if (!c_info) {
					printf("malloc loop failed\n");
					return -1;
				}
			}
		}
	}
	printf("\n\n");
	
	offsetMax = MAX_ARRAY_NUM - num;
	return 0;
}

//  打印游标 
void DisplayCursor(void)
{
	listnode_t *node = NULL;
	cursor_info *info = NULL;
	
	if (list_empty(&cur_head)) {
		printf("list is empty\n");
		return;
	}
	printf("\n******************************************\n");
	list_for_each(node, &cur_head) {
		info = node_to_item(node, cursor_info, node);
		printf("List info: Serial[%d] Offset[%d] PointNum[%d]\n",info->serialno, info->offset, *(info->pointer));
	}
	printf("******************************************\n");
}

//  根据序号查找游标
cursor_info *FindCursor(int find_num) 
{
	listnode_t *node = NULL;
	cursor_info *info = NULL;

	list_for_each(node, &cur_head) {
		info = node_to_item(node, cursor_info, node);
		if (find_num == info->serialno) 
		//printf("List info: Serial[%d] Offset[%d] PointNum[%d]\n",info->serialno, info->offset, *(info->pointer));
		return info;
	}

	printf("FindCur[%d]\n", find_num);
	
	return NULL;
}

//  指定序号游标向后移动一格 
void MoveCursor(int cur_no)
{
	cursor_info *cur = NULL;
	cur = FindCursor(cur_no);
	cur->offset  += 1;
	cur->pointer += 1;
	
	printf("MoveCur[%d]\n", cur_no);
	DisplayCursor();
}

#if 0 
int GetCurOffset(int cur_no, int *offset)
{
	cursor_info *cur = NULL;
	cur = FindCursor(cur_no);
	*offset = cur->offset;	
	printf("GetCur[%d]offset = %d\n", cur_no, cur->offset);
	return cur->offset;	
}
#endif

int GetCurOffset(int cur_no)
{

	cursor_info *cur = NULL;
	cur = FindCursor(cur_no);
//	printf("GetCur[%d]offset = %d\n", cur_no, cur->offset);
	return cur->offset;	
}

//  从输入序号开始重新排列游标
void Rearrange(int cur_no)
{
	int i = 0;
	cursor_info *info_this = NULL;
	cursor_info *info_prev = NULL;
	
	printf("arrange cur[%d]\n", cur_no);
	for (i = cur_no + 1; i <= CURSOR_NUM; i++) {
		info_this = FindCursor(i);
		info_prev = FindCursor(i - 1);
		info_this->offset  = info_prev->offset;
		info_this->pointer = info_prev->pointer + 1;
	}
	
	DisplayCursor();
}

//  刷新游标
int RenewCursor()
{
	printf("renew\n");

	int i = 0;
	
	for (i = CURSOR_NUM; i >= 2; i--) {
		if (GetCurOffset(i) == offsetMax) {
			MoveCursor(i - 1);
			Rearrange(i - 1);
		}
	}
	
	return 0;	
}

int main()
{
	int cur_num = CURSOR_NUM;
	Init(cur_num);
	DisplayCursor();
	
	while(1) {
		MoveCursor(cur_num);

		if (GetCurOffset(cur_num) == offsetMax)
			RenewCursor();

		if (GetCurOffset(1) == offsetMax)
			break;
	} 
	
	DisplayCursor();
	return 0;
}








