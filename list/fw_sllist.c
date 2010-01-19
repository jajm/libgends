#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fw_sllist.h"

fw_sllist fw_sllist_new(u32 width)
{
	fw_sllist l;

	l = malloc(sizeof(struct _fw_sllist));
	if(l == NULL)
		return NULL;
	l->head = NULL;
	l->width = width;
	return l;
}

err_code fw_sllist_add(fw_sllist l, llist_pos_t pos, void *data)
{
	struct fw_sllist_node *tmp, *tmp2;
	llist_pos_t i;

	if(l == NULL || pos < -1 || data == NULL)
		return PARAM_VALUE_ERROR;
	
	if(l->head == NULL || pos == 0){
		tmp = malloc(sizeof(struct fw_sllist_node));
		if(tmp == NULL)
			return MEM_ALLOC_ERROR;
		tmp->d = malloc(l->width);
		if(tmp->d == NULL){
			free(tmp);
			return MEM_ALLOC_ERROR;
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
		tmp2 = malloc(sizeof(struct fw_sllist_node));
		if(tmp2 == NULL)
			return MEM_ALLOC_ERROR;
		tmp2->d = malloc(l->width);
		if(tmp2->d == NULL){
			free(tmp2);
			return MEM_ALLOC_ERROR;
		}
		memmove(tmp2->d, data, l->width);
		tmp2->next = tmp->next;
		tmp->next = tmp2;
	}
	return OK;
}

err_code fw_sllist_del(fw_sllist l, llist_pos_t pos)
{
	struct fw_sllist_node *tmp, *tmp2;
	llist_pos_t i;

	if(l == NULL || l->head == NULL || pos < -1)
		return PARAM_VALUE_ERROR;
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
	return OK;
}

void *fw_sllist_get(fw_sllist l, llist_pos_t pos)
{
	struct fw_sllist_node *tmp;
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

llist_pos_t fw_sllist_chk(fw_sllist l, void *data)
{
	struct fw_sllist_node *tmp;
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

void fw_sllist_print(fw_sllist l)
{
	struct fw_sllist_node *tmp;

	if(l == NULL)
		return;
	tmp = l->head;
	while(tmp != NULL){
		printf("%p -> ", tmp->d);
		tmp = tmp->next;
	}
	printf("NULL\n");
}

err_code fw_sllist_free(fw_sllist *l)
{
	struct fw_sllist_node *tmp, *tmp2;

	if(l == NULL || *l == NULL)
		return PARAM_VALUE_ERROR;
	tmp = (*l)->head;
	while(tmp != NULL){
		tmp2 = tmp->next;
		free(tmp->d);
		free(tmp);
		tmp = tmp2;
	}
	free(*l);
	*l = NULL;

	return OK;
}

