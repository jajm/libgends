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
 * File              : slist.c                                               *
 * Short description : Generic singly linked list management                 *
 *****************************************************************************
 * A generic singly linked list can contain whatever type of data, since a   *
 * corresponding custom types exists (see core/types.h).                     *
 *****************************************************************************/

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "slist_node.h"
#include "ll_slist.h"
#include "slist.h"
#include "types.h"
#include "callbacks.h"

typedef struct {
	gds_slist_t *l;
	gds_slist_node_t *cur;
} gds_slist_iterator_t;

gds_slist_t *gds_slist_new(const char *type_name)
{
	gds_slist_t *l;
	size_t len;

	if(type_name == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	l = malloc(sizeof(gds_slist_t));
	if(l == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	
	len = strlen(type_name);
	l->type_name = malloc(len+1);
	if(l->type_name == NULL){
		free(l);
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	strncpy(l->type_name, type_name, len+1);
	assert(l->type_name[len] == '\0');

	l->first = NULL;
	l->last = NULL;
	
	return l;
}

bool gds_slist_empty(gds_slist_t *l)
{
	if(l != NULL && l->first != NULL) {
		return false;
	}
	return true;
}

gds_slist_node_t *gds_slist_first(gds_slist_t *l)
{
	return (l != NULL) ? l->first : NULL;
}

gds_slist_node_t *gds_slist_next(gds_slist_t *l, gds_slist_node_t *node)
{
	return (l != NULL) ? gds_slist_node_get_next(node) : NULL;
}

bool gds_slist_has_next(gds_slist_t *l, gds_slist_node_t *node)
{
	if(l != NULL) {
		return (gds_slist_node_get_next(node) != NULL) ? true : false;
	}
	return false;
}

gds_slist_node_t *gds_slist_add(gds_slist_t *l, gds_slist_node_t *node,
	void *data, bool copy_data)
{
	gds_slist_node_t *newnode;
	gds_alloc_cb alloc_cb = NULL;

	if(l == NULL || data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	if(copy_data) {
		alloc_cb = (gds_alloc_cb)gds_type_get_func(l->type_name, "alloc");
	}

	if (l->first == NULL || node == NULL) {
		/* Add in first position */
		newnode = gds_ll_slist_add_first(l->first, data, alloc_cb);
		if (newnode == NULL) {
			GDS_LOG_ERROR("Failed to add data to the list");
			return NULL;
		}
		l->first = newnode;
	} else {
		newnode = gds_ll_slist_add_after(node, data, alloc_cb);
	}

	if (newnode->next == NULL)
		l->last = newnode;

	return newnode;
}


gds_slist_node_t *gds_slist_add_first(gds_slist_t *l, void *data,
	bool copy_data)
{
	return gds_slist_add(l, NULL, data, copy_data);
}

gds_slist_node_t *gds_slist_add_last(gds_slist_t *l, void *data,
	bool copy_data)
{
	if(l == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	return gds_slist_add(l, l->last, data, copy_data);
}

gds_slist_node_t *gds_slist_it_add(gds_iterator_t *it, void *data,
	bool copy_data)
{
	gds_slist_t *l;
	gds_slist_node_t *node;
	gds_slist_iterator_t *sit;

	if(it == NULL || it->data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}
	sit = it->data;
	l = sit->l;
	node = sit->cur;

	return gds_slist_add(l, node, data, copy_data);
}


void *gds_slist_pop(gds_slist_t *l, gds_slist_node_t *node)
{
	void *data;
	gds_slist_node_t *tmp, *tmp2 = NULL;

	if(l == NULL || node == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	tmp = l->first;
	while(tmp != NULL && tmp != node){
		tmp2 = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL){
		GDS_LOG_ERROR("Failed to find the node");
		return NULL;
	}

	data = gds_slist_node_get_data(tmp, NULL);
	if(tmp2 == NULL)
		l->first = gds_ll_slist_del_first(l->first, NULL);
	else
		gds_ll_slist_del_after(tmp2, NULL);
	if(tmp2->next == NULL)
		l->last = tmp2;

	return data;
}

void *gds_slist_pop_first(gds_slist_t *l)
{
	void *data;

	if(l == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	data = gds_slist_node_get_data(l->first, NULL);
	l->first = gds_ll_slist_del_first(l->first, NULL);

	return data;
}

void *gds_slist_pop_last(gds_slist_t *l)
{
	void *data;

	if(l == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	data = gds_slist_node_get_data(l->last, NULL);
	l->first = gds_ll_slist_del_last(l->first, NULL);

	return data;
}

void *gds_slist_it_pop(gds_iterator_t *it)
{
	gds_slist_t *l;
	gds_slist_node_t *node;
	gds_slist_iterator_t *sit;

	if(it == NULL || it->data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	sit = it->data;
	l = sit->l;
	node = sit->cur;

	if(l == NULL || node == NULL) {
		GDS_LOG_ERROR("Iterator does not point on anything");
		return NULL;
	}

	gds_iterator_step(it);

	return gds_slist_pop(l, node);
}

int8_t gds_slist_del(gds_slist_t *l, gds_slist_node_t *node, bool free_data)
{
	gds_slist_node_t *tmp, *tmp2 = NULL;
	gds_free_cb free_cb = NULL;

	if(l == NULL || node == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	tmp = l->first;
	while(tmp != NULL && tmp != node){
		tmp2 = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL){
		GDS_LOG_ERROR("Failed to find the node");
		return -1;
	}

	if(tmp2 == NULL) {
		l->first = tmp->next;
	} else {
		tmp2->next = tmp->next;
	}

	if(tmp->next == NULL) l->last = tmp2;
	if(free_data) {
		free_cb = (gds_free_cb)gds_type_get_func(l->type_name, "free");
	}
	gds_slist_node_free(tmp, free_cb);

	return 0;
}

int8_t gds_slist_del_first(gds_slist_t *l, bool free_data)
{
	if(l == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	return gds_slist_del(l, l->first, free_data);
}

int8_t gds_slist_del_last(gds_slist_t *l, bool free_data)
{
	if(l == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	return gds_slist_del(l, l->last, free_data);
}

int8_t gds_slist_it_del(gds_iterator_t *it, bool free_data)
{
	gds_slist_t *l;
	gds_slist_node_t *node;
	gds_slist_iterator_t *sit;

	if(it == NULL || it->data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	sit = it->data;
	l = sit->l;
	node = sit->cur;

	if(l == NULL || node == NULL) {
		GDS_LOG_ERROR("Iterator does not point on anything");
		return -1;
	}

	gds_iterator_step(it);

	return gds_slist_del(l, node, free_data);
}

void *gds_slist_get(gds_slist_t *l, gds_slist_node_t *node, bool copy_data)
{
	gds_alloc_cb alloc_cb = NULL;

	if(l == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	if(copy_data) {
		alloc_cb = (gds_alloc_cb)gds_type_get_func(l->type_name, "alloc");
	}

	return gds_slist_node_get_data(node, alloc_cb);
}

void *gds_slist_get_first(gds_slist_t *l, bool copy_data)
{
	if(l == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	return gds_slist_get(l, l->first, copy_data);
}

void *gds_slist_get_last(gds_slist_t *l, bool copy_data)
{
	if(l == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	return gds_slist_get(l, l->last, copy_data);
}

void *gds_slist_it_get(gds_iterator_t *it, bool copy_data)
{
	gds_slist_t *l;
	gds_slist_node_t *node;
	gds_slist_iterator_t *sit;

	if(it == NULL || it->data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	sit = it->data;
	l = sit->l;
	node = sit->cur;

	return gds_slist_get(l, node, copy_data);
}

int8_t gds_slist_iterator_reset(gds_slist_iterator_t *it)
{
	if(it == NULL || it->l == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	it->cur = NULL;

	return 0;
}

int8_t gds_slist_iterator_step(gds_slist_iterator_t *it)
{
	gds_slist_node_t *next;

	if(it == NULL || it->l == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	next = it->cur ? gds_slist_node_get_next(it->cur)
	                  : gds_slist_first(it->l);
	if(next == NULL) {
		return 1;
	}

	it->cur = next;

	return 0;
}

void * gds_slist_iterator_get(gds_slist_iterator_t *it)
{
	if(it == NULL || it->l == NULL || it->cur == NULL) {
		GDS_LOG_ERROR("Iterator doesn't point on anything");
		return NULL;
	}

	return gds_slist_node_get_data(it->cur, NULL);
}

gds_iterator_t *gds_slist_iterator_new(gds_slist_t *l)
{
	gds_iterator_t *it;
	gds_slist_iterator_t *sit;

	if(l == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	sit = malloc(sizeof(gds_slist_iterator_t));
	if(sit == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	sit->l = l;
	sit->cur = NULL;

	it = gds_iterator_new(sit,
		(gds_iterator_reset_cb)&gds_slist_iterator_reset,
		(gds_iterator_step_cb)&gds_slist_iterator_step,
		(gds_iterator_get_cb)&gds_slist_iterator_get);

	return it;
}

void gds_slist_iterator_free(gds_iterator_t *it)
{
	gds_iterator_free(it, &free);
}

gds_slist_node_t *gds_slist_chk(gds_slist_t *l, void *data)
{
	gds_cmp_cb cmp_cb;
	gds_slist_node_t *node = NULL;
	gds_slist_node_t *tmp;

	if(l == NULL || data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	cmp_cb = (gds_cmp_cb)gds_type_get_func(l->type_name, "cmp");
	if(cmp_cb == NULL) {
		GDS_LOG_ERROR("Failed to retrieve function 'cmp' for type %s",
			l->type_name);
		return NULL;
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

void gds_slist_free(gds_slist_t *l, bool free_data)
{
	gds_free_cb free_cb = NULL;

	if(l != NULL){
		if(free_data) {
			free_cb = (gds_free_cb)gds_type_get_func(l->type_name, "free");
		}
		gds_ll_slist_free(l->first, free_cb);
		free(l->type_name);
		free(l);
	}
}
