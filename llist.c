#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

err_code vw_llist_add(vw_llist *l, llist_pos_t pos, generic_t data)
{
	struct vw_llist_node *tmp, *tmp2;
	llist_pos_t i;

	if(l == NULL || pos < -1 || data == NULL)
		return PARAM_VALUE_ERROR;
	/* Création d'une nouvelle liste ou ajout en première position */
	if(*l == NULL || pos == 0){
		tmp = malloc(sizeof(struct vw_llist_node));
		if(tmp == NULL)
			return MEM_ALLOC_ERROR;
		tmp->d = data;
		tmp->next = *l;
		*l = tmp;
	}else{
		tmp = *l;
		i = 1;
		while(tmp->next != NULL && i != pos){
			tmp = tmp->next;
			i++;
		}
		tmp2 = malloc(sizeof(struct vw_llist_node));
		if(tmp2 == NULL)
			return MEM_ALLOC_ERROR;
		tmp2->d = data;
		tmp2->next = tmp->next;
		tmp->next = tmp2;
	}
	return OK;
}

err_code vw_llist_del(vw_llist *l, llist_pos_t pos)
{
	struct vw_llist_node *tmp, *tmp2;
	llist_pos_t i;
	
	if(l == NULL || *l == NULL || pos < -1)
		return PARAM_VALUE_ERROR;
	if(pos == 0 || (*l)->next == NULL){
		/* Suppression du premier élément ou du dernier
		 * quand la liste ne contient qu'un seul élément */
		tmp = *l;
		*l = tmp->next;
		generic_free(&(tmp->d));
		free(tmp);
	}else{
		tmp = *l;
		i = 1;
		while(tmp->next != NULL && i != pos){
			tmp2 = tmp;
			tmp = tmp->next;
			i++;
		}
		if(tmp->next != NULL){
			tmp2 = tmp->next;
			tmp->next = tmp2->next;
			generic_free(&(tmp2->d));
			free(tmp2);
		}else{
			tmp2->next = NULL;
			generic_free(&(tmp->d));
			free(tmp);
		}
	}
	return OK;

}

generic_t vw_llist_get(vw_llist l, llist_pos_t pos)
{
	struct vw_llist_node *tmp;
	llist_pos_t i;

	if(l == NULL)
		return NULL;
	tmp = l;
	i = 0;
	while(tmp != NULL && i != pos){
		tmp = tmp->next;
	}
	if(tmp == NULL)
		return NULL;
	return tmp->d;
}

llist_pos_t vw_llist_chk(vw_llist l, generic_t data)
{
	struct vw_llist_node *tmp;
	llist_pos_t pos;

	tmp = l;
	pos = 0;
	while(tmp != NULL && generic_cmp(tmp->d, data) != 0){
		tmp = tmp->next;
		pos++;
	}
	if(tmp == NULL)
		return -1;

	return pos;
}

void vw_llist_print(vw_llist l)
{
	struct vw_llist_node *tmp;

	tmp = l;
	while(tmp != NULL){
		printf("%p -> ", tmp->d);
		tmp = tmp->next;
	}
	printf("NULL\n");
}

err_code vw_llist_free(vw_llist *l)
{
	struct vw_llist_node *tmp, *tmp2;

	if(l == NULL || *l == NULL)
		return PARAM_VALUE_ERROR;
	tmp = *l;
	while(tmp != NULL){
		tmp2 = tmp->next;
		generic_free(&(tmp->d));
		free(tmp);
		tmp = tmp2;
	}
	*l = NULL;

	return OK;
}




llist llist_new(u32 width)
{
	llist l;

	l = malloc(sizeof(struct _llist));
	if(l == NULL)
		return NULL;
	l->head = NULL;
	l->width = width;
	return l;
}

err_code llist_add(llist l, llist_pos_t pos, void *data)
{
	struct llist_node *tmp, *tmp2;
	llist_pos_t i;

	if(l == NULL || pos < -1 || data == NULL)
		return PARAM_VALUE_ERROR;
	
	if(l->head == NULL || pos == 0){
		tmp = malloc(sizeof(struct llist_node));
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
		tmp2 = malloc(sizeof(struct llist_node));
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

err_code llist_del(llist l, llist_pos_t pos)
{
	struct llist_node *tmp, *tmp2;
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

void *llist_get(llist l, llist_pos_t pos)
{
	struct llist_node *tmp;
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

llist_pos_t llist_chk(llist l, void *data)
{
	struct llist_node *tmp;
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

void llist_print(llist l)
{
	struct llist_node *tmp;

	if(l == NULL)
		return;
	tmp = l->head;
	while(tmp != NULL){
		printf("%p -> ", tmp->d);
		tmp = tmp->next;
	}
	printf("NULL\n");
}

err_code llist_free(llist *l)
{
	struct llist_node *tmp, *tmp2;

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

