#ifndef __LIST_H__
#define __LIST_H__
#include "main.h"

typedef struct list_node{
	struct list_node *next;
	struct list_node *prev;
}list_t;


typedef struct LAB_KEY{
	uint8_t lcd_show_string[50];
	uint8_t show_line;
	int8_t mode;
	int16_t count;
	list_t list;
}LAB_list;

extern LAB_list *now_list;
extern LAB_list *last_list;
void list_init(list_t *l);
void list_insert_after(list_t *l,list_t *n);
void list_find_before(LAB_list *node);
void list_find_next(LAB_list *LAB_list);

#endif
