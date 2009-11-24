#include <stdio.h>
#include <stdlib.h>
#include "l_list.h"

err_code llist_add(struct l_list_node **head, s16 pos, generic_t data)
{
	struct l_list_node *tmp, *tmp2;
	s16 i;

	if(head == NULL || pos < -1 || data == NULL)
		return PARAM_VALUE_ERROR;
	/* Création d'une nouvelle liste ou ajout en première position */
	if(*head == NULL || pos == 0){
		tmp = malloc(sizeof(struct l_list_node));
		if(tmp == NULL)
			return MEM_ALLOC_ERROR;
		tmp->d = data;
		tmp->next = *head;
		*head = tmp;
	}else{
		tmp = *head;
		i = 1;
		while(tmp->next != NULL && i != pos){
			tmp = tmp->next;
			i++;
		}
		tmp2 = malloc(sizeof(struct l_list_node));
		if(tmp2 == NULL)
			return MEM_ALLOC_ERROR;
		tmp2->d = data;
		tmp2->next = tmp->next;
		tmp->next = tmp2;
	}
	return OK;
}

err_code llist_del(struct l_list_node **head, s16 pos)
{
	struct l_list_node *tmp, *tmp2;
	s16 i;
	
	if(head == NULL || *head == NULL || pos < -1)
		return PARAM_VALUE_ERROR;
	if(pos == 0){
		tmp = *head;
		*head = tmp->next;
		generic_free(&(tmp->d));
		free(tmp);
	}else{
		tmp = *head;
		i = 1;
		while(tmp->next != NULL && i != pos){
			tmp = tmp->next;
			i++;
		}
		tmp2 = tmp->next;
		tmp->next = tmp2->next;
		generic_free(&(tmp2->d));
		free(tmp2);
	}
	return OK;

}

generic_t llist_get(struct l_list_node *head, s16 pos)
{
	struct l_list_node *tmp;
	s16 i;

	if(head == NULL)
		return NULL;
	tmp = head;
	i = 0;
	while(tmp != NULL && i != pos){
		tmp = tmp->next;
	}
	if(tmp == NULL)
		return NULL;
	return tmp->d;
}

s16 llist_chk(struct l_list_node *head, generic_t data)
{
	struct l_list_node *tmp;
	s16 pos;

	tmp = head;
	pos = 0;
	while(tmp != NULL && generic_cmp(tmp->d, data) != 0){
		tmp = tmp->next;
		pos++;
	}
	if(tmp == NULL)
		return -1;

	return pos;
}

err_code llist_free(struct l_list_node **head)
{
	struct l_list_node *tmp, *tmp2;

	if(head == NULL || *head == NULL)
		return PARAM_VALUE_ERROR;
	tmp = *head;
	while(tmp != NULL){
		tmp2 = tmp->next;
		generic_free(&(tmp->d));
		free(tmp);
		tmp = tmp2;
	}
	*head = NULL;

	return OK;
}
		

