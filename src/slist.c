#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "slist.h"
#include "types.h"
#include "error.h"

slist_node_t *slist_node_add_after(slist_node_t *node, void *data)
{
	slist_node_t *newnode;
	
	assert(data != NULL);
	newnode = malloc(sizeof(slist_node_t));
	if(newnode == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	newnode->data = data;
	if(node){
		newnode->next = node->next;
		node->next = newnode;
	}else{
		newnode->next = NULL;
	}

	return newnode;
}

void *slist_node_get(slist_node_t *node)
{
	assert(node != NULL);
	return node->data;
}

slist_node_t *slist_node_pop(slist_node_t **head, slist_node_t *node,
	void **data)
{
	slist_node_t *tmp, *ptmp = NULL;

	assert(head != NULL);
	assert(node != NULL);
	
	*data = NULL;
	tmp = *head;
	while(tmp != NULL && tmp != node){
		ptmp = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL){
		Error("Node not found");
		return NULL;
	}

	*data = tmp->data;
	if(ptmp == NULL) *head = tmp->next;
	else ptmp->next = tmp->next;
	free(tmp);

	return ptmp;
}

slist_node_t *slist_node_del(slist_node_t **head, slist_node_t *node,
	func_ptr_t free_f)
{
	void *data;
	slist_node_t *prev;
	
	prev = slist_node_pop(head, node, &data);
	if(data == NULL){
		ErrorP("Failed to delete node");
		return NULL;
	}
	if(free_f) free_f(data);
	else free(data);

	return prev;
}



slist_t *slist_new(const char *type_name)
{
	slist_t *l;
	size_t len;

	assert(type_name != NULL);

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
	
	return l;
}

s8 slist_empty(slist_t *l)
{
	assert(l != NULL);
	
	if(l->first == NULL)
		return 1;
	return 0;
}

slist_node_t *slist_first(slist_t *l)
{
	assert(l != NULL);

	return l->first;
}

slist_node_t *slist_next(slist_t *l, slist_node_t *node)
{
	assert(l != NULL);
	
	if(node == NULL)
		return NULL;

	return node->next;
}

s8 slist_end(slist_t *l, slist_node_t *node)
{
	assert(l != NULL);

	if(node == NULL)
		return 1;
	
	return 0;
}

slist_node_t *slist_add_first(slist_t *l, void *data)
{
	slist_node_t *newnode;
	
	assert(l != NULL);
	assert(data != NULL);
	
	if((newnode = slist_node_add_after(NULL, data)) == NULL){
		ErrorP("Failed to create a new node");
		return NULL;
	}

	newnode->next = l->first;
	l->first = newnode;
	if(l->last == NULL)
		l->last = newnode;
	
	return newnode;
}

slist_node_t *slist_add_last(slist_t *l, void *data)
{
	slist_node_t *newnode;

	assert(l != NULL);
	assert(data != NULL);
	
	if((newnode = slist_node_add_after(l->last, data)) == NULL){
		ErrorP("Failed to create a new node");
		return NULL;
	}

	if(l->first == NULL) l->first = newnode;
	l->last = newnode;

	return newnode;
}

slist_node_t *slist_add(iterator_t *it, void *data)
{
	slist_node_t *newnode;
	slist_t *l;

	assert(it != NULL);
	assert(it->container != NULL);
	assert(it->pointer != NULL);
	assert(data != NULL);
	
	if((newnode = slist_node_add_after(it->pointer, data)) == NULL){
		ErrorP("Failed to create a new node");
		return NULL;
	}
	l = (slist_t *)it->container;
	if(it->pointer == l->last) l->last = newnode;

	return newnode;
}

void *slist_pop_first(slist_t *l)
{
	void *data;
	slist_node_t *first, *next;
	
	assert(l != NULL);
	
	if(l->first == NULL){
		Error("Empty list");
		return NULL;
	}
	
	first = l->first;
	next = first->next;
	slist_node_pop(&first, l->first, &data);
	if(data == NULL){
		ErrorP("Failed to pop the node");
		return NULL;
	}
	if(l->last == l->first) l->last = NULL;
	l->first = first;

	return data;
}

void *slist_pop_last(slist_t *l)
{
	void *data;
	slist_node_t *first;
	slist_node_t *prev;

	assert(l != NULL);
	
	if(l->last == NULL){
		Error("Empty list");
		return NULL;
	}
	
	first = l->first;
	prev = slist_node_pop(&first, l->last, &data);
	if(data == NULL){
		ErrorP("Failed to pop the node");
		return NULL;
	}
	l->last = prev;
	l->first = first;

	return data;
}

void *slist_pop(iterator_t *it)
{
	void *data;
	slist_t *l;
	slist_node_t *first, *prev, *node;

	assert(it != NULL);
	assert(it->container != NULL);
	assert(it->pointer != NULL);
	
	l = (slist_t *)it->container;
	first = l->first;
	node = it->pointer;
	iterator_next(it);
	prev = slist_node_pop(&first, node, &data);
	if(data == NULL){
		ErrorP("Failed to pop the node");
		return NULL;
	}
	if(node == l->last) l->last = prev;
	l->first = first;

	return data;
}

s8 slist_del_first(slist_t *l)
{
	slist_node_t *next, *first;
	func_ptr_t free_f;

	assert(l != NULL);
	
	if(l->first == NULL){
		Error("Empty list");
		return -1;
	}
	
	first = l->first;
	next = first->next;
	free_f = type_get_func(l->type_name, "free");
	slist_node_del(&first, l->first, free_f);

	if(l->last == l->first) l->last = NULL;
	l->first = first;

	return 0;
}

s8 slist_del_last(slist_t *l)
{
	slist_node_t *prev, *first;
	func_ptr_t free_f;

	assert(l != NULL);
	
	if(l->last == NULL){
		Error("Empty list");
		return -1;
	}
	
	free_f = type_get_func(l->type_name, "free");
	first = l->first;
	prev = slist_node_del(&first, l->last, free_f);
	l->last = prev;
	l->first = first;

	return 0;
}

s8 slist_del(iterator_t *it)
{
	slist_t *l;
	slist_node_t *prev, *first, *node;
	func_ptr_t free_f;

	assert(it != NULL);
	assert(it->container != NULL);
	assert(it->pointer != NULL);

	l = (slist_t *)it->container;
	free_f = type_get_func(l->type_name, "free");
	first = l->first;
	node = it->pointer;
	iterator_next(it);
	prev = slist_node_del(&first, node, free_f);
	if(node == l->last) l->last = prev;
	l->first = first;

	return 0;
}

void *slist_get_first(slist_t *l)
{
	assert(l != NULL);

	return slist_node_get(l->first);
}

void *slist_get_last(slist_t *l)
{
	assert(l != NULL);

	return slist_node_get(l->last);
}

void *slist_get(slist_t *l, slist_node_t *node)
{
	assert(l != NULL);

	return slist_node_get(node);
}

iterator_t *slist_iterator_new(slist_t *l)
{
	iterator_t *it;
	s8 tr;

	assert(l != NULL);

	tr = type_reg("slist_it", sizeof(iterator_t));
	if(tr < 0) return NULL;

	if(tr == 0){
		type_reg_func("slist_it", "first", (func_ptr_t)&slist_first);
		type_reg_func("slist_it", "next", (func_ptr_t)&slist_next);
		type_reg_func("slist_it", "get", (func_ptr_t)&slist_get);
		type_reg_func("slist_it", "end", (func_ptr_t)&slist_end);
	}

	it = iterator_new("slist_it", l);
	iterator_reset(it);

	return it;
}		


slist_node_t *slist_chk(slist_t *l, void *data)
{
	func_ptr_t cmp_f;
	slist_node_t *node = NULL;
	slist_node_t *tmp;
	u32 type_size;

	assert(l != NULL);
	assert(data != NULL);
	
	cmp_f = type_get_func(l->type_name, "cmp");
	type_size = type_sizeof(l->type_name);
	tmp = l->first;
	while(tmp){
		if((cmp_f && cmp_f(tmp->data, data) == 0) ||
		(!cmp_f && memcmp(tmp->data, data, type_size) == 0)){
			node = tmp;
			break;
		}	
		tmp = tmp->next;
	}

	return node;
}

void slist_free(slist_t *l)
{
	slist_node_t *tmp, *tmp2;
	func_ptr_t free_f;

	if(l != NULL){
		free_f = type_get_func(l->type_name, "free");
		tmp = l->first;
		while(tmp != NULL){
			tmp2 = tmp->next;
			if(free_f) free_f(tmp->data);
			else free(tmp->data);
			free(tmp);
			tmp = tmp2;
		}
		free(l);
	}
}

void slist_destroy(slist_t *l)
{
	slist_node_t *tmp, *tmp2;

	if(l != NULL){
		tmp = l->first;
		while(tmp != NULL){
			tmp2 = tmp->next;
			free(tmp);
			tmp = tmp2;
		}
		free(l);
	}
}

