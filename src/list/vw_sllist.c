#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vw_sllist.h"

err_code vw_sllist_add(vw_sllist *l, llist_pos_t pos, generic_t data)
{
	struct vw_sllist_node *tmp, *tmp2;
	llist_pos_t i;

	if(l == NULL || pos < -1 || data == NULL)
		return PARAM_VALUE_ERROR;
	/* Création d'une nouvelle liste ou ajout en première position */
	if(*l == NULL || pos == 0){
		tmp = malloc(sizeof(struct vw_sllist_node));
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
		tmp2 = malloc(sizeof(struct vw_sllist_node));
		if(tmp2 == NULL)
			return MEM_ALLOC_ERROR;
		tmp2->d = data;
		tmp2->next = tmp->next;
		tmp->next = tmp2;
	}
	return OK;
}

err_code vw_sllist_del(vw_sllist *l, llist_pos_t pos)
{
	struct vw_sllist_node *tmp, *tmp2;
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

generic_t vw_sllist_get(vw_sllist l, llist_pos_t pos)
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

llist_pos_t vw_sllist_chk(vw_sllist l, generic_t data)
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

err_code vw_sllist_free(vw_sllist *l)
{
	struct vw_sllist_node *tmp, *tmp2;

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
