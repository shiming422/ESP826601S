#include "list.h"

void list_init(list_t *l)
{
	l->next=l->prev=l;
}

void list_insert_after(list_t *l,list_t *n)
{
	l->next->prev=n;
	n->next=l->next;
	 
	l->next=n;
	n->prev=l;
}
LAB_list *now_list = NULL; 
LAB_list *last_list = NULL; 
void list_find_before(LAB_list *node)
{
		if (node == NULL) {
        now_list = NULL;
        return;
    }
		
		// 获取下一个 list_t 节点
    list_t *before_list = node->list.prev;  // 用 . 访问 next
    // 通过指针偏移计算外层 LAB_list 地址
    now_list = (before_list != NULL) ? (LAB_list*)((char*)before_list - offsetof(LAB_list, list)) : NULL;
}
void list_find_next(LAB_list *node)
{
	if (node == NULL) {
        now_list = NULL;
        return;
    }
    // 获取下一个 list_t 节点
    list_t *next_list = node->list.next;  // 用 . 访问 next
    // 通过指针偏移计算外层 LAB_list 地址
    now_list = (next_list != NULL) ? (LAB_list*)((char*)next_list - offsetof(LAB_list, list)) : NULL;
}
