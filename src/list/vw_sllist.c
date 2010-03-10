#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vw_sllist.h"
#include "err_code.h"

vw_sllist vw_sllist_new(error_ptr *err)
{
	vw_sllist l;

	l = malloc(sizeof(struct _vw_sllist));
	if(l == NULL){
		error_set(err, ERR_MALLOC, "Memory allocation error");
		return NULL;
	}
	l->first = NULL;
	l->last = NULL;
	l->curr = NULL;
	
	error_free(err);
	return l;
}

/* TODO */
s8 vw_sllist_add(vw_sllist l, llist_pos_t pos, generic_ptr data, error_ptr *err)
{
	struct vw_sllist_node *tmp, *tmp2;
	llist_pos_t i;

	if(l == NULL || pos < -1 || data == NULL){
		error_set(err, ERR_PARAM, "Bad parameter value");
		return ERR_PARAM;
	}
	/* Création d'une nouvelle liste */
	if(l->first == NULL){
		l->first = malloc(sizeof(struct vw_sllist_node));
		if(l->first == NULL){
			error_set(err, ERR_MALLOC, "Memory allocation error");
			return ERR_MALLOC;
		}
		l->first->data = data;
		l->first->next = NULL;
		l->last = l->first;
		l->curr = l->first;
	/* Ajout en première position */
	}else if(pos == 0){
		tmp = malloc(sizeof(struct vw_sllist_node));
		if(tmp == NULL){
			error_set(err, ERR_MALLOC, "Memory allocation error");
			return ERR_MALLOC;
		}
		tmp->data = data;
		tmp->next = l->first;
		l->first = tmp;
	}else{
		tmp = l->first;
		i = 1;
		while(tmp->next != NULL && i != pos){
			tmp = tmp->next;
			i++;
		}
		tmp2 = malloc(sizeof(struct vw_sllist_node));
		if(tmp2 == NULL){
			error_set(err, ERR_MALLOC, "Memory allocation error");
			return ERR_MALLOC;
		}
		tmp2->data = data;
		tmp2->next = tmp->next;
		tmp->next = tmp2;
		if(tmp2->next == NULL) l->last = tmp2;
	}

	error_free(err);
	return OK;
}

/* TODO the rest of the world */
s8 vw_sllist_del(vw_sllist *l, llist_pos_t pos)
{
	struct vw_sllist_node *tmp, *tmp2;
	llist_pos_t i;
	
	if(l == NULL || *l == NULL || pos < -1)
		return ERR_PARAM;
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

generic_ptr vw_sllist_get(vw_sllist l, llist_pos_t pos)
{
	struct vw_sllist_node *tmp;
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

llist_pos_t vw_sllist_chk(vw_sllist l, generic_ptr data)
{
	struct vw_sllist_node *tmp;
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

void vw_sllist_print(vw_sllist l)
{
	struct vw_sllist_node *tmp;

	tmp = l;
	while(tmp != NULL){
		printf("%p -> ", tmp->d);
		tmp = tmp->next;
	}
	printf("NULL\n");
}

s8 vw_sllist_free(vw_sllist *l)
{
	struct vw_sllist_node *tmp, *tmp2;

	if(l == NULL || *l == NULL)
		return ERR_PARAM;
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
