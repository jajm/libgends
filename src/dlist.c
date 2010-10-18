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

s8 dlist_has_next(dlist_t *l, dlist_node_t *node)
{
	assert(l != NULL);

	if(node != NULL && node->next != NULL)
		return 1;
	
	return 0;
}

s8 dlist_has_prev(dlist_t *l, dlist_node_t *node)
{
	assert(l != NULL);

	if(node != NULL && node->prev != NULL)
		return 1;
	
	return 0;
}

dlist_node_t *dlist_add_after(dlist_t *l, dlist_node_t *node, void *data)
{
	dlist_node_t *newnode;
	
	assert(l != NULL);
	assert(data != NULL);

	newnode = dlnode_new(data);
	if(newnode == NULL){
		ErrorP("Failed to create the node");
		return NULL;
	}
	if(node){
		newnode->next = node->next;
		newnode->prev = node;
		if(node->next != NULL)
			node->next->prev = newnode;
		node->next = newnode;
	}
	if(newnode->next == NULL) l->last = newnode;
	if(newnode->prev == NULL) l->first = newnode;

	return newnode;
}

dlist_node_t *dlist_add_before(dlist_t *l, dlist_node_t *node, void *data)
{
	dlist_node_t *newnode;
	
	assert(l != NULL);
	assert(data != NULL);

	newnode = dlnode_new(data);
	if(newnode == NULL){
		ErrorP("Failed to create the node");
		return NULL;
	}
	if(node){
		newnode->next = node;
		newnode->prev = node->prev;
		if(node->prev != NULL)
			node->prev->next = newnode;
		node->prev = newnode;
	}
	if(newnode->prev == NULL) l->first = newnode;
	if(newnode->next == NULL) l->last = newnode;

	return newnode;
}

dlist_node_t *dlist_add_first(dlist_t *l, void *data)
{
	assert(l != NULL);

	return dlist_add_before(l, l->first, data);
}

dlist_node_t *dlist_add_last(dlist_t *l, void *data)
{
	assert(l != NULL);

	return dlist_add_after(l, l->last, data);
}

dlist_node_t *dlist_it_add_after(iterator_t *it, void *data)
{
	dlist_node_t *node;
	dlist_t *l;

	assert(it != NULL);
	assert(data != NULL);

	l = (dlist_t *)it->container;
	node = (dlist_node_t *)it->pointer;

	return dlist_add_after(l, node, data);
}

dlist_node_t *dlist_it_add_before(iterator_t *it, void *data)
{
	dlist_node_t *node;
	dlist_t *l;

	assert(it != NULL);
	assert(data != NULL);

	l = (dlist_t *)it->container;
	node = (dlist_node_t *)it->pointer;

	return dlist_add_before(l, node, data);
}

void *dlist_pop(dlist_t *l, dlist_node_t *node)
{
	void *data;

	assert(l != NULL);
	assert(node != NULL);

	if(node->next) node->next->prev = node->prev;
	else l->last = node->prev;
	if(node->prev) node->prev->next = node->next;
	else l->first = node->next;

	data = dlnode_data(node);
	dlnode_free(node);

	return data;
}

void *dlist_pop_first(dlist_t *l)
{
	assert(l != NULL);

	return dlist_pop(l, l->first);
}

void *dlist_pop_last(dlist_t *l)
{
	assert(l != NULL);

	return dlist_pop(l, l->last);
}

void *dlist_it_pop(iterator_t *it)
{
	dlist_t *l;
	dlist_node_t *node;

	assert(it != NULL);

	l = (dlist_t *)it->container;
	node = (dlist_node_t *)it->pointer;
	it_next(it);

	return dlist_pop(l, node);
}


s8 dlist_del(dlist_t *l, dlist_node_t *node)
{
	func_ptr_t free_f;

	assert(l != NULL);
	assert(node != NULL);

	if(node->next) node->next->prev = node->prev;
	else l->last = node->prev;
	if(node->prev) node->prev->next = node->next;
	else l->first = node->next;

	free_f = type_get_func(l->type_name, "free");
	if(free_f) free_f(dlnode_data(node));
	dlnode_free(node);

	return 0;
}

s8 dlist_del_first(dlist_t *l)
{
	assert(l != NULL);

	return dlist_del(l, l->first);
}

s8 dlist_del_last(dlist_t *l)
{
	assert(l != NULL);

	return dlist_del(l, l->last);
}

s8 dlist_it_del(iterator_t *it)
{
	dlist_t *l;
	dlist_node_t *node;

	assert(it != NULL);

	l = (dlist_t *)it->container;
	node = (dlist_node_t *)it->pointer;
	it_next(it);

	return dlist_del(l, node);
}

void *dlist_get(dlist_t *l, dlist_node_t *node)
{
	assert(l != NULL);

	return dlnode_data(node);
}

void *dlist_get_first(dlist_t *l)
{
	assert(l != NULL);

	return dlist_get(l, l->first);
}

void *dlist_get_last(dlist_t *l)
{
	assert(l != NULL);

	return dlist_get(l, l->last);
}

void *dlist_it_get(iterator_t *it)
{
	dlist_t *l;
	dlist_node_t *node;

	assert(it != NULL);

	l = (dlist_t *)it->container;
	node = (dlist_node_t *)it->pointer;

	return dlist_get(l, node);
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
		type_reg_func("dlist_it", "has_next", (func_ptr_t)&dlist_has_next);
	}

	it = it_new("dlist_it", l);
	it_reset(it);
	
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
		type_reg_func("dlist_rit", "has_next", (func_ptr_t)&dlist_has_prev);
	}

	it = it_new("dlist_rit", l);
	it_reset(it);
	
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

