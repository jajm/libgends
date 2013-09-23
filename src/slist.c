/*
 * Copyright 2010-2013 Julian Maurice
 *
 * This file is part of libgends
 *
 * libgends is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libgends is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdlib.h>
#include "exception.h"
#include "check_arg.h"
#include "callbacks.h"
#include "log.h"
#include "inline/slist.h"
#include "slist.h"
#include "undefined.h"
#include "container_of.h"

typedef struct {
	void *data;
	gds_inline_slist_node_t slist_node_inline;
} gds_slist_node_t;

gds_slist_node_t * gds_slist_node_get_container_of(
	gds_inline_slist_node_t *node_inline)
{
	gds_slist_node_t *node = NULL;

	if (node_inline) {
		node = container_of(node_inline, gds_slist_node_t,
			slist_node_inline);
	}

	return node;
}

gds_inline_slist_node_t *gds_slist_node_get_inline(gds_slist_node_t *node)
{
	return (node != NULL) ? &(node->slist_node_inline) : NULL;
}

gds_slist_node_t *gds_slist_node_new(void *data)
{
	gds_slist_node_t *node;

	GDS_CHECK_ARG_NOT_NULL(data);

	node = malloc(sizeof(gds_slist_node_t));
	if (node == NULL) {
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_slist_node_t));
	}
	node->data = data;
	gds_inline_slist_node_init(&(node->slist_node_inline));

	return node;
}


int8_t gds_slist_node_set_data(gds_slist_node_t *node, void *data,
	void *callback, void *callback_data)
{
	void (*cb)(void *, void *) = callback;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (cb != NULL) {
		cb(node->data, callback_data);
	}

	node->data = data;

	return 0;
}

void *gds_slist_node_get_data(gds_slist_node_t *node)
{
	void *data;

	GDS_CHECK_ARG_NOT_NULL(node);

	data = node->data;

	return data;
}

gds_slist_node_t * gds_slist_node_copy(gds_slist_node_t *node)
{
	gds_slist_node_t *head, *tmp, *tmp2, *n;
	gds_inline_slist_node_t *i, *ti, *t2i;

	GDS_CHECK_ARG_NOT_NULL(node);

	head = gds_slist_node_new(node->data);
	tmp = head;
	ti = gds_slist_node_get_inline(tmp);
	tmp2 = node;
	t2i = gds_slist_node_get_inline(tmp2);
	while ( (t2i = gds_inline_slist_node_get_next(t2i)) != NULL ) {
		tmp2 = gds_slist_node_get_container_of(t2i);
		n = gds_slist_node_new(tmp2->data);
		i = gds_slist_node_get_inline(n);
		gds_inline_slist_node_next(ti, i);

		ti = gds_inline_slist_node_get_next(ti);
	}

	return head;
}

void gds_slist_node_free(gds_slist_node_t *node, void *callback,
	void *callback_data)
{
	void (*cb)(void *, void *) = callback;

	if (cb != NULL && node != NULL) {
		cb(node->data, callback_data);
	}
	free(node);
}

struct gds_slist_s {
	gds_slist_node_t *head;
	gds_slist_node_t *tail;
	unsigned int size;
};

gds_slist_t * gds_slist_new(void)
{
	gds_slist_t *list = malloc(sizeof(gds_slist_t));

	if (list) {
		list->head = list->tail = NULL;
		list->size = 0;
	}

	return list;
}

gds_slist_t * gds_slist_new_from_array(unsigned int n, void *data[])
{
	gds_slist_t *list = gds_slist_new();
	unsigned int i;

	for(i = 0; i < n; i++) {
		gds_slist_push(list, data[i]);
	}

	return list;
}

int gds_slist_unshift(gds_slist_t *list, void *data)
{
	gds_slist_node_t *n;
	gds_inline_slist_node_t *i, *head, *tail;
	int added;

	GDS_CHECK_ARG_NOT_NULL(list);

	n = gds_slist_node_new(data);
	i = gds_slist_node_get_inline(n);
	head = gds_slist_node_get_inline(list->head);
	tail = gds_slist_node_get_inline(list->tail);

	added = gds_inline_slist_insert(head, 0, i, &head, &tail);
	if (added != 1) {
		GDS_LOG_ERROR("Insertion failed");
		return -1;
	}

	list->size += added;
	list->head = gds_slist_node_get_container_of(head);
	list->tail = gds_slist_node_get_container_of(tail);

	return 0;
}

int gds_slist_push(gds_slist_t *list, void *data)
{
	gds_slist_node_t *n;
	gds_inline_slist_node_t *i, *head, *tail;
	int added;

	GDS_CHECK_ARG_NOT_NULL(list);

	n = gds_slist_node_new(data);
	i = gds_slist_node_get_inline(n);
	head = gds_slist_node_get_inline(list->head);
	tail = gds_slist_node_get_inline(list->tail);

	added = gds_inline_slist_insert(tail, 1, i, &head, &tail);
	if (added != 1) {
		GDS_LOG_ERROR("Insertion failed");
		return -1;
	}

	list->size += added;
	list->head = gds_slist_node_get_container_of(head);
	list->tail = gds_slist_node_get_container_of(tail);

	return 0;
}

void gds_slist_node_remove_callback(gds_inline_slist_node_t *node_inline,
	void *params[2])
{
	gds_slist_node_t *node;
	void (*callback)(gds_slist_node_t *, void *) = NULL;
	void *callback_data = NULL;
	
	if (params != NULL) {
		callback = params[0];
		callback_data = params[1];
	}

	node = gds_slist_node_get_container_of(node_inline);
	gds_slist_node_free(node, callback, callback_data);
}

void * gds_slist_shift(gds_slist_t *list)
{
	gds_inline_slist_node_t *head, *tail;
	int removed;
	void *d = UNDEFINED;

	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->head != NULL) {
		d = gds_slist_node_get_data(list->head);
		head = gds_slist_node_get_inline(list->head);
		tail = gds_slist_node_get_inline(list->tail);

		removed = gds_inline_slist_remove(head, 0, 1,
			gds_slist_node_remove_callback, NULL, &head, &tail);
		if (removed != 1) {
			GDS_LOG_ERROR("Removal failed");
			return UNDEFINED;
		}

		list->size -= removed;
		list->head = gds_slist_node_get_container_of(head);
		list->tail = gds_slist_node_get_container_of(tail);
	}

	return d;
}

void * gds_slist_pop(gds_slist_t *list)
{
	gds_inline_slist_node_t *head, *tail;
	void *d = UNDEFINED;

	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->tail != NULL) {
		d = gds_slist_node_get_data(list->tail);
		head = gds_slist_node_get_inline(list->head);
		gds_inline_slist_remove_tail(head,
			gds_slist_node_remove_callback, NULL, &head, &tail);
		list->head = gds_slist_node_get_container_of(head);
		list->tail = gds_slist_node_get_container_of(tail);

		list->size--;
		head = gds_slist_node_get_inline(list->head);
		tail = gds_slist_node_get_inline(list->tail);
	}

	return d;
}

void * gds_slist_get(gds_slist_t *list, unsigned int offset)
{
	void *d = UNDEFINED;
	gds_slist_node_t *n;
	gds_inline_slist_node_t *i;
	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->head != NULL) {
		i = gds_slist_node_get_inline(list->head);
		i = gds_inline_slist_get(i, offset);
		n = gds_slist_node_get_container_of(i);
		if (n != NULL) {
			d = gds_slist_node_get_data(n);
		}
	}

	return d;
}


void gds_slist_splice(gds_slist_t *list, unsigned int offset,
	unsigned int length, void *callback, void *callback_data,
	gds_slist_t *replacement)
{
	gds_inline_slist_node_t *head, *tail, *ci = NULL;
	gds_slist_node_t *copy;
	int added;

	GDS_CHECK_ARG_NOT_NULL(list);

	head = gds_slist_node_get_inline(list->head);
	tail = gds_slist_node_get_inline(list->tail);
	if (replacement != NULL && replacement->head != NULL) {
		copy = gds_slist_node_copy(replacement->head);
		ci = gds_slist_node_get_inline(copy);
	}
	added = gds_inline_slist_splice(head, offset, length,
		gds_slist_node_remove_callback,
		(void *[]){callback, callback_data}, ci, &head, &tail);

	list->head = gds_slist_node_get_container_of(head);
	list->tail = gds_slist_node_get_container_of(tail);

	list->size += added;
}

gds_slist_t * gds_slist_slice(gds_slist_t *list, unsigned int offset,
	unsigned int length, void *callback, void *callback_data)
{
	gds_slist_t *slice;
	gds_slist_node_t *n;
	gds_inline_slist_node_t *i;
	void *(*cb)(void *, void *) = callback;
	void *data;
	unsigned int cnt = 0;

	GDS_CHECK_ARG_NOT_NULL(list);

	slice = gds_slist_new();

	if (list->head != NULL) {
		i = gds_slist_node_get_inline(list->head);
		i = gds_inline_slist_get(i, offset);
		while (i != NULL && cnt < length) {
			n = gds_slist_node_get_container_of(i);
			data = gds_slist_node_get_data(n);
			if (cb != NULL) {
				data = cb(data, callback_data);
			}
			gds_slist_push(slice, data);
			i = gds_inline_slist_node_get_next(i);
			cnt++;
		}
	}

	return slice;
}

void gds_slist_map_callback(gds_inline_slist_node_t *current, unsigned int pos,
	void *params[2])
{
	gds_slist_node_t *n = gds_slist_node_get_container_of(current);
	void *data = gds_slist_node_get_data(n);
	void *(*callback)(void *, unsigned int, void *) = params[0];
	void *callback_data = params[1];

	callback(data, pos, callback_data);
}

void gds_slist_map(gds_slist_t *list, void *callback, void *callback_data)
{
	gds_inline_slist_node_t *head;

	GDS_CHECK_ARG_NOT_NULL(list);
	GDS_CHECK_ARG_NOT_NULL(callback);

	if (list->head != NULL) {
		head = gds_slist_node_get_inline(list->head);
		gds_inline_slist_map(head, gds_slist_map_callback,
			(void *[]) {callback, callback_data});
	}
}

gds_slist_t * gds_slist_filter(gds_slist_t *list, void *callback,
	void *callback_data)
{
	void *d;
	gds_slist_t *l;
	gds_slist_node_t *n;
	gds_inline_slist_node_t *i;
	int (*cb)(void *, void *) = callback;

	GDS_CHECK_ARG_NOT_NULL(list);
	GDS_CHECK_ARG_NOT_NULL(callback);

	l = gds_slist_new();

	if (list->head != NULL) {
		i = gds_slist_node_get_inline(list->head);
		while (i != NULL) {
			n = gds_slist_node_get_container_of(i);
			d = gds_slist_node_get_data(n);
			if (cb(d, callback_data)) {
				gds_slist_push(l, d);
			}
			i = gds_inline_slist_node_get_next(i);
		}
	}

	return l;
}

void gds_slist_reduce_callback(gds_inline_slist_node_t *current,
	unsigned int pos, void *params[3])
{
	gds_slist_node_t *n = gds_slist_node_get_container_of(current);
	void *data = gds_slist_node_get_data(n);
	void *(*callback)(void *, void *, unsigned int, void *) = params[0];
	void *callback_data = params[1];
	void **result = params[2];

	*result = callback(*result, data, pos, callback_data);
}

void * gds_slist_reduce(gds_slist_t *list, void *callback, void *callback_data)
{
	gds_inline_slist_node_t *i;
	void *result = NULL;

	GDS_CHECK_ARG_NOT_NULL(list);
	GDS_CHECK_ARG_NOT_NULL(callback);

	if (list->head != NULL) {
		i = gds_slist_node_get_inline(list->head);
		gds_inline_slist_map(i, gds_slist_reduce_callback,
			(void *[]) {callback, callback_data, &result});
	}

	return result;
}

unsigned int gds_slist_size(gds_slist_t *list)
{
	GDS_CHECK_ARG_NOT_NULL(list);

	return list->size;
}

void gds_slist_free(gds_slist_t *list, void *callback, void *callback_data)
{
	gds_slist_node_t *tmp;
	gds_inline_slist_node_t *ti, *ti2;

	if (list != NULL && list->head != NULL) {
		ti = gds_slist_node_get_inline(list->head);
		while (ti != NULL) {
			ti2 = gds_inline_slist_node_get_next(ti);
			tmp = gds_slist_node_get_container_of(ti);
			gds_slist_node_free(tmp, callback, callback_data);
			ti = ti2;
		}
	}
	free(list);
}

typedef struct {
	gds_slist_t *list;
	gds_inline_slist_node_t *cur;
	int i;
} gds_slist_iterator_data_t;

int8_t gds_slist_iterator_reset(gds_slist_iterator_data_t *it_data)
{
	GDS_CHECK_ARG_NOT_NULL(it_data);

	it_data->cur = NULL;
	it_data->i = -1;

	return 0;
}

int8_t gds_slist_iterator_step(gds_slist_iterator_data_t *it_data)
{
	gds_inline_slist_node_t *next = NULL;

	GDS_CHECK_ARG_NOT_NULL(it_data);

	if (it_data->cur != NULL) {
		next = gds_inline_slist_node_get_next(it_data->cur);
	} else if(it_data->list->head != NULL) {
		next = gds_slist_node_get_inline(it_data->list->head);
	}

	if (next == NULL) {
		return 1;
	}

	it_data->cur = next;
	it_data->i++;

	return 0;
}

void * gds_slist_iterator_get(gds_slist_iterator_data_t *it_data)
{
	gds_slist_node_t *node;

	GDS_CHECK_ARG_NOT_NULL(it_data);

	node = gds_slist_node_get_container_of(it_data->cur);
	return (node != NULL) ? gds_slist_node_get_data(node) : NULL;
}

const void * gds_slist_iterator_getkey(gds_slist_iterator_data_t *it_data)
{
	GDS_CHECK_ARG_NOT_NULL(it_data);

	return &(it_data->i);
}

gds_iterator_t * gds_slist_iterator_new(gds_slist_t *list)
{
	gds_slist_iterator_data_t *it_data;
	gds_iterator_t *it;

	it_data = malloc(sizeof(gds_slist_iterator_data_t));
	if (it_data == NULL) {
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_slist_iterator_data_t));
	}

	it_data->list = list;
	it_data->cur = NULL;

	it = gds_iterator_new(it_data,
		(gds_iterator_reset_cb) &gds_slist_iterator_reset,
		(gds_iterator_step_cb) &gds_slist_iterator_step,
		(gds_iterator_get_cb) &gds_slist_iterator_get,
		(gds_iterator_getkey_cb) &gds_slist_iterator_getkey,
		(gds_free_cb) &free);
	
	return it;
}
