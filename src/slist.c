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
 * Fichier           : slist.c                                               *
 * Description Brève : Gestion d'une liste simplement chainée générique      *
 * Auteur            : Julian Maurice                                        *
 * Créé le           : 01/03/2010                                            *
 *****************************************************************************
 * Une liste chainée générique peut contenir tout type de données, à partir  *
 * du moment où un type utilisateur correspondant existe.                    *
 * Voir types.h pour plus d'informations sur les types utilisateurs          *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "slist.h"
#include "types.h"
#include "error.h"


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

slist_node_t *slist_add(slist_t *l, slist_node_t *node, void *data)
{
	slist_node_t *newnode;

	assert(l != NULL);
	assert(data != NULL);

	newnode = slnode_new(data);
	if(newnode == NULL){
		ErrorP("Failed to create a new node");
		return NULL;
	}
	if(l->first == NULL || node == NULL){
		/* Ajout en première position */
		newnode->next = l->first;
		l->first = newnode;
	}else{
		newnode->next = node->next;
		node->next = newnode;
	}
	if(newnode->next == NULL) l->last = newnode;

	return newnode;
}


slist_node_t *slist_add_first(slist_t *l, void *data)
{	
	return slist_add(l, NULL, data);
}

slist_node_t *slist_add_last(slist_t *l, void *data)
{
	assert(l != NULL);
	
	return slist_add(l, l->last, data);
}

slist_node_t *slist_it_add(iterator_t *it, void *data)
{
	slist_t *l;
	slist_node_t *node;

	assert(it != NULL);

	l = (slist_t *)it->container;
	node = (slist_node_t *)it->pointer;
	
	return slist_add(l, node, data);
}


void *slist_pop(slist_t *l, slist_node_t *node)
{
	void *data;
	slist_node_t *tmp, *tmp2 = NULL;

	assert(l != NULL);
	assert(node != NULL);

	tmp = l->first;
	while(tmp != NULL && tmp != node){
		tmp2 = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL){
		Error("Failed to find the node");
		return NULL;
	}
	data = slnode_data(tmp);
	if(tmp2 == NULL)
		l->first = tmp->next;
	else
		tmp2->next = tmp->next;
	if(tmp->next == NULL) l->last = tmp2;
	slnode_free(tmp);

	return data;
}

void *slist_pop_first(slist_t *l)
{
	assert(l != NULL);

	return slist_pop(l, l->first);
}

void *slist_pop_last(slist_t *l)
{
	assert(l != NULL);

	return slist_pop(l, l->last);
}

void *slist_it_pop(iterator_t *it)
{
	slist_t *l;
	slist_node_t *node;

	assert(it != NULL);

	l = (slist_t *)it->container;
	node = (slist_node_t *)it->pointer;

	iterator_next(it);

	return slist_pop(l, node);
}

s8 slist_del(slist_t *l, slist_node_t *node)
{
	slist_node_t *tmp, *tmp2 = NULL;
	func_ptr_t free_f;
	void *data;

	assert(l != NULL);
	assert(node != NULL);

	tmp = l->first;
	while(tmp != NULL && tmp != node){
		tmp2 = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL){
		Error("Failed to find the node");
		return -1;
	}
	data = slnode_data(tmp);
	free_f = type_get_func(l->type_name, "free");
	if(free_f) free_f(data);
	if(tmp2 == NULL)
		l->first = tmp->next;
	else
		tmp2->next = tmp->next;
	if(tmp->next == NULL) l->last = tmp2;
	slnode_free(tmp);

	return 0;
}

s8 slist_del_first(slist_t *l)
{
	assert(l != NULL);

	return slist_del(l, l->first);
}

s8 slist_del_last(slist_t *l)
{
	assert(l != NULL);

	return slist_del(l, l->last);
}

s8 slist_it_del(iterator_t *it)
{
	slist_t *l;
	slist_node_t *node;

	assert(it != NULL);

	l = (slist_t *)it->container;
	node = (slist_node_t *)it->pointer;

	iterator_next(it);

	return slist_del(l, node);
}

void *slist_get(slist_t *l, slist_node_t *node)
{
	assert(l != NULL);

	return slnode_data(node);
}

void *slist_get_first(slist_t *l)
{
	assert(l != NULL);

	return slist_get(l, l->first);
}

void *slist_get_last(slist_t *l)
{
	assert(l != NULL);

	return slist_get(l, l->last);
}

void *slist_it_get(iterator_t *it)
{
	slist_t *l;
	slist_node_t *node;

	assert(it != NULL);
	
	l = (slist_t *)it->container;
	node = (slist_node_t *)it->pointer;

	return slist_get(l, node);
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
			slnode_free(tmp);
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
			slnode_free(tmp);
			tmp = tmp2;
		}
		free(l);
	}
}

