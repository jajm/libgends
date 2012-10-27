/*****************************************************************************
 * Copyright (C) 2010-2012 Julian Maurice                                    *
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
 * File              : typed_dlist.c                                         *
 * Short description : Generic double linked list management                 *
 *****************************************************************************
 * A generic linked list can contain data of any type, since a corresponding *
 * custom type is defined (see types.h)                                      *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "exception.h"
#include "check_arg.h"
#include "callbacks.h"
#include "typed_dlist.h"
#include "types.h"
#include "log.h"

typedef struct {
	gds_typed_dlist_t *l;
	gds_dlist_node_t *cur;
} gds_typed_dlist_iterator_t;

gds_typed_dlist_t *gds_typed_dlist_new(const char *type_name)
{
	gds_typed_dlist_t *dlist;
	size_t len;

	GDS_CHECK_ARG_NOT_NULL(type_name);

	dlist = malloc(sizeof(gds_typed_dlist_t));
	if(dlist == NULL){
		GDS_THROW(NotEnoughMemoryException, "Failed to allocate %d "
			"bytes", sizeof(gds_typed_dlist_t));
	}

	len = strlen(type_name);
	dlist->type_name = malloc(len+1);
	if(dlist->type_name == NULL){
		free(dlist);
		GDS_THROW(NotEnoughMemoryException, "Failed to allocate %d "
			"bytes", len+1);
	}

	strncpy(dlist->type_name, type_name, len+1);
	assert(dlist->type_name[len] == '\0');
	dlist->first = NULL;
	dlist->last = NULL;

	return dlist;
}


bool gds_typed_dlist_empty(gds_typed_dlist_t *l)
{
	if(l != NULL && l->first != NULL)
		return false;

	return true;
}


gds_dlist_node_t *gds_typed_dlist_first(gds_typed_dlist_t *l)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return l->first;
}

gds_dlist_node_t *gds_typed_dlist_last(gds_typed_dlist_t *l)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return l->last;
}

gds_dlist_node_t *gds_typed_dlist_next(gds_typed_dlist_t *l, gds_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(l);
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->next;
}

gds_dlist_node_t *gds_typed_dlist_prev(gds_typed_dlist_t *l, gds_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(l);
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->prev;
}

bool gds_typed_dlist_has_next(gds_typed_dlist_t *l, gds_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(l);
	GDS_CHECK_ARG_NOT_NULL(node);

	if (node->next != NULL)
		return true;
	
	return false;
}

bool gds_typed_dlist_has_prev(gds_typed_dlist_t *l, gds_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(l);
	GDS_CHECK_ARG_NOT_NULL(node);

	if(node->prev != NULL)
		return true;
	
	return false;
}

gds_dlist_node_t *gds_typed_dlist_add_after(gds_typed_dlist_t *l, gds_dlist_node_t *node, void *data,
	bool copy_data)
{
	gds_dlist_node_t *newnode;
	gds_alloc_cb alloc_cb = NULL;

	GDS_CHECK_ARG_NOT_NULL(l);
	GDS_CHECK_ARG_NOT_NULL(data);

	if(copy_data) {
		alloc_cb = (gds_alloc_cb)gds_type_get_func(l->type_name, "alloc");
	}
	newnode = gds_dlist_node_new(data, alloc_cb);

	/* If node is NULL, add newnode at the end */
	if(node == NULL) node = l->last;
	if(node) {
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

gds_dlist_node_t *gds_typed_dlist_add_before(gds_typed_dlist_t *l, gds_dlist_node_t *node,
	void *data, bool copy_data)
{
	gds_dlist_node_t *newnode;
	gds_alloc_cb alloc_cb = NULL;

	GDS_CHECK_ARG_NOT_NULL(l);
	GDS_CHECK_ARG_NOT_NULL(data);

	if(copy_data) {
		alloc_cb = (gds_alloc_cb)gds_type_get_func(l->type_name, "alloc");
	}
	newnode = gds_dlist_node_new(data, alloc_cb);

	/* If node is NULL, add newnode at beginning */
	if(node == NULL) node = l->first;
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

gds_dlist_node_t *gds_typed_dlist_add_first(gds_typed_dlist_t *l, void *data, bool copy_data)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return gds_typed_dlist_add_before(l, l->first, data, copy_data);
}

gds_dlist_node_t *gds_typed_dlist_add_last(gds_typed_dlist_t *l, void *data, bool copy_data)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return gds_typed_dlist_add_after(l, l->last, data, copy_data);
}

gds_dlist_node_t *gds_typed_dlist_it_add_after(gds_iterator_t *it, void *data, bool copy_data)
{
	gds_dlist_node_t *node;
	gds_typed_dlist_t *l;
	gds_typed_dlist_iterator_t *dit;

	GDS_CHECK_ARG_NOT_NULL(it);
	GDS_CHECK_ARG_NOT_NULL(it->data);
	GDS_CHECK_ARG_NOT_NULL(data);

	dit = it->data;
	l = dit->l;
	node = dit->cur;

	return gds_typed_dlist_add_after(l, node, data, copy_data);
}

gds_dlist_node_t *gds_typed_dlist_it_add_before(gds_iterator_t *it, void *data, bool copy_data)
{
	gds_dlist_node_t *node;
	gds_typed_dlist_t *l;
	gds_typed_dlist_iterator_t *dit;

	GDS_CHECK_ARG_NOT_NULL(it);
	GDS_CHECK_ARG_NOT_NULL(it->data);
	GDS_CHECK_ARG_NOT_NULL(data);

	dit = it->data;
	l = dit->l;
	node = dit->cur;

	return gds_typed_dlist_add_before(l, node, data, copy_data);
}

void *gds_typed_dlist_pop(gds_typed_dlist_t *l, gds_dlist_node_t *node)
{
	void *data;

	GDS_CHECK_ARG_NOT_NULL(l);
	if (l->first == NULL) {
		GDS_THROW_EMPTY_LIST();
	}
	GDS_CHECK_ARG_NOT_NULL(node);

	if(node->next) {
		node->next->prev = node->prev;
	} else {
		l->last = node->prev;
	}
	if(node->prev) {
		node->prev->next = node->next;
	} else {
		l->first = node->next;
	}

	data = gds_dlist_node_get_data(node, NULL);
	gds_dlist_node_free(node, NULL);

	return data;
}

void *gds_typed_dlist_pop_first(gds_typed_dlist_t *l)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return gds_typed_dlist_pop(l, l->first);
}

void *gds_typed_dlist_pop_last(gds_typed_dlist_t *l)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return gds_typed_dlist_pop(l, l->last);
}

void *gds_typed_dlist_it_pop(gds_iterator_t *it)
{
	gds_typed_dlist_t *l;
	gds_dlist_node_t *node;
	gds_typed_dlist_iterator_t *dit;

	GDS_CHECK_ARG_NOT_NULL(it);
	GDS_CHECK_ARG_NOT_NULL(it->data);

	dit = it->data;
	l = dit->l;
	node = dit->cur;

	if(l == NULL || node == NULL) {
		GDS_THROW(InvalidIteratorException,
			"iterator doesn't point to anything");
	}

	gds_iterator_step(it);

	return gds_typed_dlist_pop(l, node);
}


int8_t gds_typed_dlist_del(gds_typed_dlist_t *l, gds_dlist_node_t *node, bool free_data)
{
	gds_free_cb free_cb = NULL;

	GDS_CHECK_ARG_NOT_NULL(l);
	if (l->first == NULL) {
		GDS_THROW_EMPTY_LIST();
	}
	GDS_CHECK_ARG_NOT_NULL(node);

	if(node->next) {
		node->next->prev = node->prev;
	} else {
		l->last = node->prev;
	}
	if(node->prev) {
		node->prev->next = node->next;
	} else {
		l->first = node->next;
	}

	if(free_data) {
		free_cb = (gds_free_cb)gds_type_get_func(l->type_name, "free");
	}
	gds_dlist_node_free(node, free_cb);

	return 0;
}

int8_t gds_typed_dlist_del_first(gds_typed_dlist_t *l, bool free_data)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return gds_typed_dlist_del(l, l->first, free_data);
}

int8_t gds_typed_dlist_del_last(gds_typed_dlist_t *l, bool free_data)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return gds_typed_dlist_del(l, l->last, free_data);
}

int8_t gds_typed_dlist_it_del(gds_iterator_t *it, bool free_data)
{
	gds_typed_dlist_t *l;
	gds_dlist_node_t *node;
	gds_typed_dlist_iterator_t *dit;

	GDS_CHECK_ARG_NOT_NULL(it);
	GDS_CHECK_ARG_NOT_NULL(it->data);

	dit = it->data;
	l = dit->l;
	node = dit->cur;

	if(l == NULL || node == NULL) {
		GDS_THROW(InvalidIteratorException,
			"iterator doesn't point to anything");
	}

	gds_iterator_step(it);

	return gds_typed_dlist_del(l, node, free_data);
}

void *gds_typed_dlist_get(gds_typed_dlist_t *l, gds_dlist_node_t *node, bool copy_data)
{
	gds_alloc_cb alloc_cb = NULL;

	GDS_CHECK_ARG_NOT_NULL(l);
	if (l->first == NULL) {
		GDS_THROW_EMPTY_LIST();
	}

	if(copy_data) {
		alloc_cb = (gds_alloc_cb)gds_type_get_func(l->type_name, "alloc");
	}

	return gds_dlist_node_get_data(node, alloc_cb);
}

void *gds_typed_dlist_get_first(gds_typed_dlist_t *l, bool copy_data)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return gds_typed_dlist_get(l, l->first, copy_data);
}

void *gds_typed_dlist_get_last(gds_typed_dlist_t *l, bool copy_data)
{
	GDS_CHECK_ARG_NOT_NULL(l);

	return gds_typed_dlist_get(l, l->last, copy_data);
}

void *gds_typed_dlist_it_get(gds_iterator_t *it, bool copy_data)
{
	gds_typed_dlist_t *l;
	gds_dlist_node_t *node;
	gds_typed_dlist_iterator_t *dit;

	GDS_CHECK_ARG_NOT_NULL(it);
	GDS_CHECK_ARG_NOT_NULL(it->data);

	dit = it->data;
	l = dit->l;
	node = dit->cur;

	return gds_typed_dlist_get(l, node, copy_data);
}

int8_t gds_typed_dlist_iterator_reset(gds_typed_dlist_iterator_t *it)
{
	GDS_CHECK_ARG_NOT_NULL(it);
	if (it->l == NULL) {
		GDS_THROW(InvalidIteratorException,
			"iterator doesn't point to anything");
	}

	it->cur = NULL;
	return 0;
}

int8_t gds_typed_dlist_iterator_step(gds_typed_dlist_iterator_t *it)
{
	gds_dlist_node_t *next;

	GDS_CHECK_ARG_NOT_NULL(it);
	if (it->l == NULL) {
		GDS_THROW(InvalidIteratorException,
			"iterator doesn't point to anything");
	}

	next = it->cur ? gds_dlist_node_get_next(it->cur)
	               : gds_typed_dlist_first(it->l);
	if(next == NULL) {
		return 1;
	}

	it->cur = next;

	return 0;
}

int8_t gds_typed_dlist_reverse_iterator_step(gds_typed_dlist_iterator_t *it)
{
	gds_dlist_node_t *prev;

	GDS_CHECK_ARG_NOT_NULL(it);
	if (it->l == NULL) {
		GDS_THROW(InvalidIteratorException,
			"iterator doesn't point to anything");
	}

	prev = it->cur ? gds_dlist_node_get_prev(it->cur)
	               : gds_typed_dlist_last(it->l);
	if(prev == NULL) {
		return 1;
	}

	it->cur = prev;

	return 0;
}

void * gds_typed_dlist_iterator_get(gds_typed_dlist_iterator_t *it)
{
	GDS_CHECK_ARG_NOT_NULL(it);
	if (it->l == NULL || it->cur == NULL) {
		GDS_THROW(InvalidIteratorException,
			"iterator doesn't point to anything");
	}

	return gds_dlist_node_get_data(it->cur, NULL);
}

gds_iterator_t *gds_typed_dlist_iterator_new(gds_typed_dlist_t *l)
{
	gds_iterator_t *it;
	gds_typed_dlist_iterator_t *dit;

	GDS_CHECK_ARG_NOT_NULL(l);

	dit = malloc(sizeof(gds_typed_dlist_iterator_t));
	if(dit == NULL) {
		GDS_THROW(NotEnoughMemoryException, "Failed to allocate %d "
			"bytes", sizeof(gds_typed_dlist_iterator_t));
	}
	dit->l = l;
	dit->cur = NULL;

	it = gds_iterator_new(dit,
		(gds_iterator_reset_cb)&gds_typed_dlist_iterator_reset,
		(gds_iterator_step_cb)&gds_typed_dlist_iterator_step,
		(gds_iterator_get_cb)&gds_typed_dlist_iterator_get,
		(gds_free_cb)&free);
	
	return it;
}

gds_iterator_t *gds_typed_dlist_reverse_iterator_new(gds_typed_dlist_t *l)
{
	gds_iterator_t *it;
	gds_typed_dlist_iterator_t *dit;

	GDS_CHECK_ARG_NOT_NULL(l);

	dit = malloc(sizeof(gds_typed_dlist_iterator_t));
	if(dit == NULL) {
		GDS_THROW(NotEnoughMemoryException, "Failed to allocate %d "
			"bytes", sizeof(gds_typed_dlist_iterator_t));
	}
	dit->l = l;
	dit->cur = NULL;

	it = gds_iterator_new(dit,
		(gds_iterator_reset_cb)&gds_typed_dlist_iterator_reset,
		(gds_iterator_step_cb)&gds_typed_dlist_reverse_iterator_step,
		(gds_iterator_get_cb)&gds_typed_dlist_iterator_get,
		(gds_free_cb)&free);
	
	return it;
}

gds_dlist_node_t *gds_typed_dlist_chk(gds_typed_dlist_t *l, void *data)
{
	gds_cmp_cb cmp_cb;
	gds_dlist_node_t *node = NULL;
	gds_dlist_node_t *tmp;

	GDS_CHECK_ARG_NOT_NULL(l);
	GDS_CHECK_ARG_NOT_NULL(data);

	cmp_cb = (gds_cmp_cb)gds_type_get_func(l->type_name, "cmp");
	if(cmp_cb == NULL) {
		GDS_THROW(FunctionNotFoundException, "failed to retrieve 'cmp' "
			"function for type %s", l->type_name);
	}
	tmp = l->first;
	while(tmp){
		if(cmp_cb(tmp->data, data) == 0) {
			node = tmp;
			break;
		}
		tmp = tmp->next;
	}

	return node;
}

void gds_typed_dlist_free(gds_typed_dlist_t *l, bool free_data)
{
	gds_dlist_node_t *tmp, *tmp2;
	gds_free_cb free_cb = NULL;

	if(l != NULL){
		if(free_data) {
			free_cb = (gds_free_cb)gds_type_get_func(l->type_name, "free");
		}
		tmp = l->first;
		while(tmp){
			tmp2 = tmp->next;
			gds_dlist_node_free(tmp, free_cb);
			tmp = tmp2;
		}
		free(l);
	}
}

