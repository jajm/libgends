#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fw_slist.h"

fw_slist fw_slist_new(u32 width)
{
	fw_slist l;

	l = malloc(sizeof(struct _fw_slist));
	if(l == NULL)
		return NULL;
	l->head = NULL;
	l->width = width;
	return l;
}

s8 fw_slist_add(fw_slist l, llist_pos_t pos, void *data)
{
	struct fw_slist_node *tmp, *tmp2;
	llist_pos_t i;

	if(l == NULL || pos < -1 || data == NULL)
		return -1;
	
	if(l->head == NULL || pos == 0){
		tmp = malloc(sizeof(struct fw_slist_node));
		if(tmp == NULL)
			return -1;
		tmp->d = malloc(l->width);
		if(tmp->d == NULL){
			free(tmp);
			return -1;
		}
		memmove(tmp->d, data, l->width);
		tmp->next = l->head;
		l->head = tmp;
	}else{
		tmp = l->head;
		i = 1;
		while(tmp->next != NULL && i != pos){
			tmp = tmp->next;
			i++;
		}
		tmp2 = malloc(sizeof(struct fw_slist_node));
		if(tmp2 == NULL)
			return -1;
		tmp2->d = malloc(l->width);
		if(tmp2->d == NULL){
			free(tmp2);
			return -1;
		}
		memmove(tmp2->d, data, l->width);
		tmp2->next = tmp->next;
		tmp->next = tmp2;
	}
	return 0;
}

s8 fw_slist_del(fw_slist l, llist_pos_t pos)
{
	struct fw_slist_node *tmp, *tmp2;
	llist_pos_t i;

	if(l == NULL || l->head == NULL || pos < -1)
		return -1;
	if(pos == 0 || l->head->next == NULL){
		tmp = l->head;
		l->head = tmp->next;
		free(tmp->d);
		free(tmp);
	}else{
		tmp = l->head;
		i = 1;
		while(tmp->next != NULL && i != pos){
			tmp2 = tmp;
			tmp = tmp->next;
			i++;
		}
		if(tmp->next != NULL){
			tmp2 = tmp->next;
			tmp->next = tmp2->next;
			free(tmp2->d);
			free(tmp2);
		}else{
			tmp2->next = NULL;
			free(tmp->d);
			free(tmp);
		}
	}
	return 0;
}

void *fw_slist_get(fw_slist l, llist_pos_t pos)
{
	struct fw_slist_node *tmp;
	llist_pos_t i;

	if(l == NULL || l->head == NULL)
		return NULL;
	tmp = l->head;
	i = 0;
	while(tmp != NULL && i != pos)
		tmp = tmp->next;
	if(tmp == NULL)
		return NULL;
	return tmp->d;
}

llist_pos_t fw_slist_chk(fw_slist l, void *data)
{
	struct fw_slist_node *tmp;
	llist_pos_t pos;

	if(l == NULL)
		return -1;
	tmp = l->head;
	pos = 0;
	while(tmp != NULL && memcmp(tmp->d, data, l->width) != 0){
		tmp = tmp->next;
		pos++;
	}
	if(tmp == NULL)
		return -1;
	return pos;
}

void fw_slist_print(fw_slist l)
{
	struct fw_slist_node *tmp;

	if(l == NULL)
		return;
	tmp = l->head;
	while(tmp != NULL){
		printf("%p -> ", tmp->d);
		tmp = tmp->next;
	}
	printf("NULL\n");
}

s8 fw_slist_free(fw_slist *l)
{
	struct fw_slist_node *tmp, *tmp2;

	if(l == NULL || *l == NULL)
		return -1;
	tmp = (*l)->head;
	while(tmp != NULL){
		tmp2 = tmp->next;
		free(tmp->d);
		free(tmp);
		tmp = tmp2;
	}
	free(*l);
	*l = NULL;

	return 0;
}

