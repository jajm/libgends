#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "types.h"
#include "error.h"

slist_node_t *slist_node_new(void *data)
{
	slist_node_t *newnode;

	if(data == NULL){
		Error("Bad parameters");
		return NULL;
	}
	newnode = malloc(sizeof(slist_node_t));
	if(newnode == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	newnode->data = data;
	newnode->next = NULL;

	return newnode;
}

void slist_node_free(slist_node_t *node, free_func_t free_f)
{
	if(node){
		if(free_f) free_f(node->data);
		else free(node->data);
		free(node);
	}
}


slist_t *slist_new(const char *type_name)
{
	slist_t *l;
	size_t len;

	l = malloc(sizeof(slist_t));
	if(l == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	
	len = strlen(type_name);
	l->type_name = malloc(len+1);
	if(l->type_name == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	strncpy(l->type_name, type_name, len+1);

	l->first = NULL;
	l->last = NULL;
	l->curr = NULL;
	
	return l;
}

void slist_begin(slist_t *l)
{
	if(l != NULL){
		l->curr = l->first;
	}
}

s8 slist_next(slist_t *l)
{
	if(l == NULL || l->curr == NULL){
		Error("Bad parameters");
		return -1;
	}
	
	l->curr = l->curr->next;
	return 0;
}

s8 slist_empty(slist_t *l)
{
	if(l == NULL || l->first == NULL)
		return 1;
	return 0;
}

s8 slist_end(slist_t *l)
{
	if(l == NULL || l->curr == NULL)
		return 1;
	return 0;
}

slist_node_t *slist_add_first(slist_t *l, void *data)
{
	slist_node_t *newnode;

	if(l == NULL || data == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if((newnode = slist_node_new(data)) == NULL){
		ErrorP("Failed to create a new node");
		return NULL;
	}

	newnode->next = l->first;
	l->first = newnode;
	if(l->last == NULL){
		l->last = newnode;
	}
	
	return newnode;
}

slist_node_t *slist_add_last(slist_t *l, void *data)
{
	slist_node_t *newnode;

	if(l == NULL || data == NULL){
		Error("Bad parameters");
		return NULL;
	}
	
	if((newnode = slist_node_new(data)) == NULL){
		ErrorP("Failed to create a new node");
		return NULL;
	}

	if(l->first == NULL) l->first = newnode;
	else l->last->next = newnode;

	l->last = newnode;

	return newnode;
}

slist_node_t *slist_add(slist_t *l, void *data)
{
	slist_node_t *newnode;

	if(l == NULL || data == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->curr == NULL){
		Error("Current position not set or at the end of the list");
		return NULL;
	}
	if((newnode = slist_node_new(data)) == NULL){
		ErrorP("Failed to create a new node");
		return NULL;
	}

	if(l->first == NULL){
		l->first = newnode;
		l->last = newnode;
	}else{
		newnode->next = l->curr->next;
		l->curr->next = newnode;
	}
	
	return newnode;
}

void *slist_pop_first(slist_t *l)
{
	slist_node_t *tmp;
	void *d;

	if(l == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->first == NULL){
		Error("Empty list");
		return NULL;
	}
	
	tmp = l->first->next;
	d = l->first->data;
	free(l->first);

	if(l->curr == l->first) l->curr = tmp;
	if(l->last == l->first) l->last = tmp;
	l->first = tmp;

	return d;
}

void *slist_pop_last(slist_t *l)
{
	void *d;
	slist_node_t *tmp;

	if(l == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->last == NULL){
		Error("Empty list");
		return NULL;
	}
	d = l->last->data;
	if(l->first == l->last){
		free(l->first);
		l->curr = NULL;
		l->last = NULL;
		l->first = NULL;
	}else{
		/* La position courante est en général
		 * plus proche du dernier nœud */
		if(l->curr != NULL && l->curr != l->last)
			tmp = l->curr;
		else
			tmp = l->first;
		while(tmp->next != l->last && tmp->next != NULL)
			tmp = tmp->next;
		if(tmp->next == NULL){
			Error("Last node isn't in the list...");
			return NULL;
		}
		free(l->last);
		tmp->next = NULL;
		if(l->curr == l->last) l->curr = tmp;
		l->last = tmp;
	}
	
	return d;
}


void *slist_pop(slist_t *l)
{
	slist_node_t *tmp;
	void *d;

	if(l == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->curr == NULL){
		Error("Current position not set or at the end of the list");
		return NULL;
	}
	d = l->curr->data;
	if(l->first == l->curr){
		free(l->first);
		if(l->last == l->first) l->last = NULL;
		l->first = l->first->next;
		l->curr = l->curr->next;
	}else{
		tmp = l->first;
		while(tmp->next != l->curr && tmp->next != NULL)
			tmp = tmp->next;
		if(tmp->next == NULL){
			Error("Current node isn't in the list");
			return NULL;
		}
		tmp->next = l->curr->next;
		free(l->curr);
		if(l->last == l->curr){
			l->last = tmp;
			l->curr = tmp;
		}else{
			l->curr = tmp->next;
		}
	}

	return d;
}

s8 slist_free_first(slist_t *l)
{
	slist_node_t *tmp;
	free_func_t free_f;

	if(l == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(l->first == NULL){
		Error("Empty list");
		return -1;
	}
	
	tmp = l->first->next;
	free_f = type_free_func(l->type_name);
	slist_node_free(l->first, free_f);

	if(l->curr == l->first) l->curr = l->curr->next;
	if(l->last == l->first) l->last = NULL;
	l->first = tmp;

	return 0;
}

s8 slist_free_last(slist_t *l)
{
	slist_node_t *tmp;
	free_func_t free_f;

	if(l == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(l->last == NULL){
		Error("Empty list");
		return -1;
	}
	
	free_f = type_free_func(l->type_name);
	if(l->first == l->last){
		slist_node_free(l->last, free_f);
		l->first = NULL;
		l->curr = NULL;
		l->last = NULL;
	}else{
		if(l->curr != NULL && l->curr != l->last)
			tmp = l->curr;
		else
			tmp = l->first;
		while(tmp->next != l->last && tmp->next != NULL)
			tmp = tmp->next;
		if(tmp->next == NULL){
			Error("Last node isn't in the list");
			return -1;
		}
		slist_node_free(l->last, free_f);
		if(l->curr == l->last) l->curr = tmp;
		l->last = tmp;
	}
	
	return 0;
}

s8 slist_free(slist_t *l)
{
	slist_node_t *tmp;
	free_func_t free_f;

	if(l == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(l->curr == NULL){
		Error("Empty list");
		return -1;
	}
	free_f = type_free_func(l->type_name);
	if(l->first == l->curr){
		slist_node_free(l->first, free_f);
		if(l->last == l->first) l->last = NULL;
		l->curr = l->curr->next;
		l->first = l->first->next;
	}else{
		tmp = l->first;
		while(tmp->next != l->curr && tmp->next != NULL)
			tmp = tmp->next;
		if(tmp->next == NULL){
			Error("Current node isn't in the list");
			return -1;
		}
		tmp->next = l->curr->next;
		slist_node_free(l->curr, free_f);
		if(l->last == l->curr){
			l->last = tmp;
			l->curr = tmp;
		}else{
			l->curr = tmp->next;
		}
	}

	return 0;
}

void *slist_get_first(slist_t *l)
{
	if(l == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->first == NULL){
		Error("Empty list");
		return NULL;
	}

	return l->first->data;
}

void *slist_get_last(slist_t *l)
{
	if(l == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->last == NULL){
		Error("Empty list");
		return NULL;
	}

	return l->last->data;
}

void *slist_get(slist_t *l)
{
	if(l == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->curr == NULL){
		Error("Empty list");
		return NULL;
	}

	return l->curr->data;
}


slist_node_t *slist_chk(slist_t *l, void *data)
{
	void *g;
	cmp_func_t cmp_f;
	slist_node_t *node = NULL;
	slist_node_t *tmp;
	u32 type_size;

	if(l == NULL || data == NULL){
		Error("Bad parameters");
		return NULL;
	}
	
	cmp_f = type_cmp_func(l->type_name);
	type_size = type_sizeof(l->type_name);
	tmp = l->first;
	while(tmp){
		if(cmp_f && cmp_f(tmp->data, data) == 0 ||
		!cmp_f && memcmp(tmp->data, data, type_size) == 0){
			node = tmp;
			break;
		}	
		tmp = tmp->next;
	}

	return node;
}

void slist_print(slist_t *l)
{
	slist_node_t *tmp;
	
	if(l != NULL){	
		tmp = l->first;
		while(tmp != NULL){
			printf("%p -> ", tmp->data);
			tmp = tmp->next;
		}
		printf("NULL\n");
	}
}

void slist_free_list(slist_t *l)
{
	slist_node_t *tmp, *tmp2;
	free_func_t free_f;

	if(l != NULL){
		free_f = type_free_func(l->type_name);
		tmp = l->first;
		while(tmp != NULL){
			tmp2 = tmp->next;
			slist_node_free(tmp, free_f);
			tmp = tmp2;
		}
		free(l);
	}
}
