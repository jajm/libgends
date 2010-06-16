/*****************************************************************************
 * Copyright (C) 2010 Julian Maurice                                         *
 *                                                                           *
 * This file is part of libgends.                                            *
 *                                                                           *
 * libgends is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * libgends is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.         *
 *****************************************************************************/

/*****************************************************************************
 * Fichier		: dlist.c                                            *
 * Description Brève	: Gestion d'une liste doublement chainée générique   *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/06/2010                                         *
 *****************************************************************************
 * Une liste chainée générique peut contenir tout type de données, à partir  *
 * du moment où un type utilisateur correspondant existe.                    *
 * Voir types.h pour plus d'informations sur les types utilisateurs          *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dlist.h"
#include "types.h"
#include "error.h"

dlist_node_t *dlist_node_add_before(dlist_node_t *node, void *data)
{
	dlist_node_t *newnode;

	assert(data != NULL);

	newnode = malloc(sizeof(dlist_node_t));
	if(newnode == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	newnode->data = data;
	newnode->next = node;
	if(node){
		newnode->prev = node->prev;
		if(node->prev) node->prev->next = newnode;
		node->prev = newnode;
	}else{
		newnode->prev = NULL;
	}

	return newnode;
}

dlist_node_t *dlist_node_add_after(dlist_node_t *node, void *data)
{
	dlist_node_t *newnode;

	assert(data != NULL);

	newnode = malloc(sizeof(dlist_node_t));
	if(newnode == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	newnode->data = data;
	newnode->prev = node;
	if(node){
		newnode->next = node->next;
		if(node->next) node->next->prev = newnode;
		node->next = newnode;
	}else{
		newnode->next = NULL;
	}

	return newnode;
}

void *dlist_node_get(dlist_node_t *node)
{
	assert(node != NULL);

	return node->data;
}

void dlist_node_pop(dlist_node_t *node, void **data)
{
	assert(node != NULL);
	assert(data != NULL);

	if(node->prev) node->prev->next = node->next;
	if(node->next) node->next->prev = node->prev;
	*data = node->data;
	free(node);
}

void dlist_node_del(dlist_node_t *node, func_ptr_t free_f)
{
	if(node != NULL){
		if(node->prev) node->prev->next = node->next;
		if(node->next) node->next->prev = node->prev;
		if(free_f) free_f(node->data);
		else free(node->data);
		free(node);
	}
}



dlist_t *dlist_new(const char *type_name)
{
	dlist_t *dlist;
	size_t len;

	assert(type_name != NULL);
	
	dlist = malloc(sizeof(dlist_t));
	if(dlist == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	len = strlen(type_name);
	dlist->type_name = malloc(len+1);
	if(dlist->type_name == NULL){
		Error("Memory allocation error");
		free(dlist);
		return NULL;
	}
	strncpy(dlist->type_name, type_name, len+1);
	dlist->first = NULL;
	dlist->last = NULL;

	return dlist;
}


s8 dlist_empty(dlist_t *l)
{
	assert(l != NULL);

	if(l->first == NULL) return 1;

	return 0;
}


dlist_node_t *dlist_first(dlist_t *l)
{
	assert(l != NULL);

	return l->first;
}

dlist_node_t *dlist_last(dlist_t *l)
{
	assert(l != NULL);

	return l->last;
}

dlist_node_t *dlist_next(dlist_t *l, dlist_node_t *node)
{
	assert(l != NULL);

	if(node == NULL)
		return NULL;
	
	return node->next;
}

dlist_node_t *dlist_prev(dlist_t *l, dlist_node_t *node)
{
	assert(l != NULL);

	if(node == NULL)
		return NULL;
	
	return node->prev;
}

s8 dlist_end(dlist_t *l, dlist_node_t *node)
{
	assert(l != NULL);

	if(node == NULL)
		return 1;
	
	return 0;
}


dlist_node_t *dlist_add_first(dlist_t *l, void *data)
{
	dlist_node_t *newnode;

	assert(l != NULL);

	newnode = dlist_node_add_before(l->first, data);
	if(newnode){
		l->first = newnode;
		if(l->last == NULL) l->last = newnode;
	}

	return newnode;
}

dlist_node_t *dlist_add_last(dlist_t *l, void *data)
{
	dlist_node_t *newnode;

	assert(l != NULL);

	newnode = dlist_node_add_after(l->last, data);
	if(newnode){
		l->last = newnode;
		if(l->first == NULL) l->first = newnode;
	}

	return newnode;
}

dlist_node_t *dlist_add_after(iterator_t *it, void *data)
{
	dlist_node_t *newnode;
	dlist_t *l;

	assert(it != NULL);
	assert(it->container != NULL);
	assert(it->pointer != NULL);
	assert(data != NULL);

	if((newnode = dlist_node_add_after(it->pointer, data)) == NULL){
		ErrorP("Failed to create a new node");
		return NULL;
	}
	l = (dlist_t *)it->container;
	if(it->pointer == l->last) l->last = newnode;

	return newnode;
}

dlist_node_t *dlist_add_before(iterator_t *it, void *data)
{
	dlist_node_t *newnode;
	dlist_t *l;

	assert(it != NULL);
	assert(it->container != NULL);
	assert(it->pointer != NULL);
	assert(data != NULL);

	if((newnode = dlist_node_add_before(it->pointer, data)) == NULL){
		ErrorP("Failed to create a new node");
		return NULL;
	}
	l = (dlist_t *)it->container;
	if(it->pointer == l->first) l->first = newnode;

	return newnode;
}

void *dlist_pop_first(dlist_t *l)
{
	void *data;
	dlist_node_t *next;

	assert(l != NULL);
	if(l->first == NULL){
		Error("Empty list");
		return NULL;
	}
	
	next = l->first->next;
	dlist_node_pop(l->first, &data);
	if(data == NULL){
		ErrorP("Failed to pop the node");
		return NULL;
	}
	if(l->first == l->last) l->last = NULL;
	l->first = next;

	return data;
}

void *dlist_pop_last(dlist_t *l)
{
	void *data;
	dlist_node_t *prev;
	
	assert(l != NULL);
	if(l->last == NULL){
		Error("Empty list");
		return NULL;
	}

	prev = l->last->prev;
	dlist_node_pop(l->last, &data);
	if(data == NULL){
		ErrorP("Failed to pop the node");
		return NULL;
	}
	if(l->first == l->last) l->first = NULL;
	l->last = prev;

	return data;
}

void *dlist_pop(iterator_t *it)
{
	void *data;
	dlist_t *l;
	dlist_node_t *node;

	assert(it != NULL);
	assert(it->container != NULL);
	assert(it->pointer != NULL);

	l = (dlist_t *)it->container;
	node = (dlist_node_t *)it->pointer;
	if(l->first == node) l->first = l->first->next;
	if(l->last == node) l->last = l->last->prev;
	iterator_next(it);
	dlist_node_pop(node, &data);
	if(data == NULL){
		ErrorP("Failed to pop the node");
		return NULL;
	}

	return data;
}


s8 dlist_del_first(dlist_t *l)
{
	func_ptr_t free_f;
	dlist_node_t *next;

	assert(l != NULL);
	if(l->first == NULL){
		Error("Empty list");
		return -1;
	}

	free_f = type_get_func(l->type_name, "free");
	next = l->first->next;
	dlist_node_del(l->first, free_f);
	if(l->first == l->last) l->last = NULL;
	l->first = next;

	return 0;
}

s8 dlist_del_last(dlist_t *l)
{
	func_ptr_t free_f;
	dlist_node_t *prev;

	assert(l != NULL);
	if(l->last == NULL){
		Error("Empty list");
		return -1;
	}

	free_f = type_get_func(l->type_name, "free");
	prev = l->last->prev;
	dlist_node_del(l->last, free_f);
	if(l->first == l->last) l->first = NULL;
	l->last = prev;
	
	return 0;
}

s8 dlist_del(iterator_t *it)
{
	dlist_t *l;
	dlist_node_t *node;
	func_ptr_t free_f;

	assert(it != NULL);
	assert(it->container != NULL);
	assert(it->pointer != NULL);

	l = (dlist_t *)it->container;
	node = (dlist_node_t *)it->pointer;
	free_f = type_get_func(l->type_name, "free");
	if(l->first == node) l->first = l->first->next;
	if(l->last == node) l->last = l->last->prev;
	iterator_next(it);
	dlist_node_del(node, free_f);

	return 0;
}

void *dlist_get_first(dlist_t *l)
{
	assert(l != NULL);

	return dlist_node_get(l->first);
}

void *dlist_get_last(dlist_t *l)
{
	assert(l != NULL);

	return dlist_node_get(l->last);
}

void *dlist_get(dlist_t *l, dlist_node_t *node)
{
	assert(l != NULL);

	return dlist_node_get(node);
}


iterator_t *dlist_iterator_new(dlist_t *l)
{
	iterator_t *it;
	s8 tr;

	assert(l != NULL);

	tr = type_reg("dlist_it", sizeof(iterator_t));
	if(tr < 0) return NULL;

	if(tr == 0){
		type_reg_func("dlist_it", "first", (func_ptr_t)&dlist_first);
		type_reg_func("dlist_it", "next", (func_ptr_t)&dlist_next);
		type_reg_func("dlist_it", "get", (func_ptr_t)&dlist_get);
		type_reg_func("dlist_it", "end", (func_ptr_t)&dlist_end);
	}

	it = iterator_new("dlist_it", l);
	iterator_reset(it);
	
	return it;
}

iterator_t *dlist_reverse_iterator_new(dlist_t *l)
{
	iterator_t *it;
	s8 tr;

	assert(l != NULL);

	tr = type_reg("dlist_rit", sizeof(iterator_t));
	if(tr < 0) return NULL;

	if(tr == 0){
		type_reg_func("dlist_rit", "first", (func_ptr_t)&dlist_last);
		type_reg_func("dlist_rit", "next", (func_ptr_t)&dlist_prev);
		type_reg_func("dlist_rit", "get", (func_ptr_t)&dlist_get);
		type_reg_func("dlist_rit", "end", (func_ptr_t)&dlist_end);
	}

	it = iterator_new("dlist_rit", l);
	iterator_reset(it);
	
	return it;
}

dlist_node_t *dlist_chk(dlist_t *l, void *data)
{
	func_ptr_t cmp_f;
	dlist_node_t *node = NULL;
	dlist_node_t *tmp;
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

void dlist_free(dlist_t *l)
{
	dlist_node_t *tmp, *tmp2;
	func_ptr_t free_f;

	if(l != NULL){
		free_f = type_get_func(l->type_name, "free");
		tmp = l->first;
		while(tmp){
			tmp2 = tmp->next;
			if(free_f) free_f(tmp->data);
			else free(tmp->data);
			free(tmp);
			tmp = tmp2;
		}
		free(l);
	}
}

void dlist_destroy(dlist_t *l)
{
	dlist_node_t *tmp, *tmp2;

	if(l != NULL){
		tmp = l->first;
		while(tmp){
			tmp2 = tmp->next;
			free(tmp);
			tmp = tmp2;
		}
		free(l);
	}
}

