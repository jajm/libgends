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
#include "inline/dlist.h"
#include "dlist_node.h"
#include "dlist.h"
#include "undefined.h"
#include "lambda.h"

struct gds_dlist_s {
	gds_dlist_node_t *head;
	gds_dlist_node_t *tail;
	unsigned int size;
};

gds_dlist_t * gds_dlist_new(void)
{
	gds_dlist_t *list = malloc(sizeof(gds_dlist_t));

	if (list) {
		list->head = list->tail = NULL;
		list->size = 0;
	}

	return list;
}

gds_dlist_t * gds_dlist_new_from_array(unsigned int n, void *data[])
{
	gds_dlist_t *list = gds_dlist_new();
	unsigned int i;

	for(i = 0; i < n; i++) {
		gds_dlist_push(list, data[i]);
	}

	return list;
}

int gds_dlist_unshift(gds_dlist_t *list, void *data)
{
	gds_dlist_node_t *n;
	gds_inline_dlist_node_t *i, *head, *tail;
	int added;

	GDS_CHECK_ARG_NOT_NULL(list);

	n = gds_dlist_node_new(data);

	head = gds_dlist_node_get_inline(list->head);
	tail = gds_dlist_node_get_inline(list->tail);
	i = gds_dlist_node_get_inline(n);

	added = gds_inline_dlist_insert(head, 0, i, &head, &tail);
	if (added != 1) {
		GDS_LOG_ERROR("Insertion failed");
		return -1;
	}

	list->size += added;
	list->head = gds_dlist_node_get_container_of(head);
	list->tail = gds_dlist_node_get_container_of(tail);

	return 0;
}

int gds_dlist_push(gds_dlist_t *list, void *data)
{
	gds_dlist_node_t *n;
	gds_inline_dlist_node_t *i, *head, *tail;
	int added;

	GDS_CHECK_ARG_NOT_NULL(list);

	n = gds_dlist_node_new(data);

	head = gds_dlist_node_get_inline(list->head);
	tail = gds_dlist_node_get_inline(list->tail);
	i = gds_dlist_node_get_inline(n);

	added = gds_inline_dlist_insert(tail, 1, i, &head, &tail);
	if (added != 1) {
		GDS_LOG_ERROR("Insertion failed");
		return -1;
	}

	list->size += added;
	list->head = gds_dlist_node_get_container_of(head);
	list->tail = gds_dlist_node_get_container_of(tail);

	return 0;
}

void _gds_dlist_remove_callback(gds_inline_dlist_node_t *idn,
	void *params[2])
{
	gds_dlist_node_t *node;
	void (*callback)(gds_dlist_node_t *, void *) = NULL;
	void *callback_data = NULL;

	if (params != NULL) {
		callback = params[0];
		callback_data = params[1];
	}
	node = gds_dlist_node_get_container_of(idn);
	gds_dlist_node_free(node, callback, callback_data);
}

void * gds_dlist_shift(gds_dlist_t *list)
{
	gds_inline_dlist_node_t *head, *tail;
	void *callback = _gds_dlist_remove_callback;
	void *d = UNDEFINED;

	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->head != NULL) {
		head = gds_dlist_node_get_inline(list->head);
		tail = gds_dlist_node_get_inline(list->tail);
		d = gds_dlist_node_get_data(list->head);

		list->size -= gds_inline_dlist_remove(head, 0, 1, callback,
			NULL, &head, &tail);
		list->head = gds_dlist_node_get_container_of(head);
		list->tail = gds_dlist_node_get_container_of(tail);
	}

	return d;
}

void * gds_dlist_pop(gds_dlist_t *list)
{
	gds_inline_dlist_node_t *head, *tail;
	void *callback = _gds_dlist_remove_callback;
	void *d = UNDEFINED;

	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->tail != NULL) {
		head = gds_dlist_node_get_inline(list->head);
		tail = gds_dlist_node_get_inline(list->tail);
		d = gds_dlist_node_get_data(list->tail);

		list->size -= gds_inline_dlist_remove(tail, 0, 1, callback,
			NULL, &head, &tail);
		list->head = gds_dlist_node_get_container_of(head);
		list->tail = gds_dlist_node_get_container_of(tail);
	}

	return d;
}

void * gds_dlist_get(gds_dlist_t *list, unsigned int offset)
{
	void *d = UNDEFINED;
	gds_dlist_node_t *n;
	gds_inline_dlist_node_t *i;
	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->head != NULL) {
		i = gds_dlist_node_get_inline(list->head);
		i = gds_inline_dlist_get(i, offset);
		n = gds_dlist_node_get_container_of(i);
		if (n != NULL) {
			d = gds_dlist_node_get_data(n);
		}
	}

	return d;
}

void gds_dlist_splice(gds_dlist_t *list, unsigned int offset,
	unsigned int length, void *callback, void *callback_data,
	gds_dlist_t *replacement)
{
	gds_inline_dlist_node_t *head, *tail, *ci = NULL;
	void *cb = _gds_dlist_remove_callback;
	gds_dlist_node_t *copy;

	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->head) {
		head = gds_dlist_node_get_inline(list->head);
		tail = gds_dlist_node_get_inline(list->tail);

		if (replacement != NULL) {
			copy = gds_dlist_node_copy(replacement->head);
			ci = gds_dlist_node_get_inline(copy);
		}
		list->size += gds_inline_dlist_splice(head, offset, length,
			cb, (void *[]){callback, callback_data}, ci,
			&head, &tail);
		list->head = gds_dlist_node_get_container_of(head);
		list->tail = gds_dlist_node_get_container_of(tail);
	}
}

gds_dlist_t * gds_dlist_slice(gds_dlist_t *list, unsigned int offset,
	unsigned int length, void *callback, void *callback_data)
{
	gds_dlist_t *slice;
	gds_dlist_node_t *n;
	gds_inline_dlist_node_t *i;
	void *(*cb)(void *, void *) = callback;
	void *data;
	unsigned int cnt = 0;

	GDS_CHECK_ARG_NOT_NULL(list);

	slice = gds_dlist_new();

	if (list->head != NULL) {
		i = gds_dlist_node_get_inline(list->head);
		i = gds_inline_dlist_get(i, offset);
		while (i != NULL && cnt < length) {
			n = gds_dlist_node_get_container_of(i);
			data = gds_dlist_node_get_data(n);
			if (cb != NULL) {
				data = cb(data, callback_data);
			}
			gds_dlist_push(slice, data);
			i = gds_inline_dlist_node_get_next(i);
			cnt++;
		}
	}

	return slice;
}

void _gds_dlist_map_callback(gds_inline_dlist_node_t *current, unsigned int pos,
	void *params[2])
{
	gds_dlist_node_t *n = gds_dlist_node_get_container_of(current);
	void *data = gds_dlist_node_get_data(n);
	void *(*callback)(void *, unsigned int, void *) = params[0];
	void *callback_data = params[1];

	callback(data, pos, callback_data);
}

void gds_dlist_map(gds_dlist_t *list, void *callback, void *callback_data)
{
	gds_inline_dlist_node_t *i;

	GDS_CHECK_ARG_NOT_NULL(list);
	GDS_CHECK_ARG_NOT_NULL(callback);

	if (list->head != NULL) {
		i = gds_dlist_node_get_inline(list->head);
		gds_inline_dlist_map(i, _gds_dlist_map_callback,
			(void *[]) {callback, callback_data});
	}
}

gds_dlist_t * gds_dlist_filter(gds_dlist_t *list, void *callback,
	void *callback_data)
{
	void *d;
	gds_dlist_t *l;
	gds_dlist_node_t *n;
	gds_inline_dlist_node_t *i;
	int (*cb)(void *, void *) = callback;

	GDS_CHECK_ARG_NOT_NULL(list);
	GDS_CHECK_ARG_NOT_NULL(callback);

	l = gds_dlist_new();

	if (list->head != NULL) {
		i = gds_dlist_node_get_inline(list->head);
		while (i != NULL) {
			n = gds_dlist_node_get_container_of(i);
			d = gds_dlist_node_get_data(n);
			if (cb(d, callback_data)) {
				gds_dlist_push(l, d);
			}
			i = gds_inline_dlist_node_get_next(i);
		}
	}

	return l;
}

void _gds_dlist_reduce_callback(gds_inline_dlist_node_t *current,
	unsigned int pos, void *params[3])
{
	gds_dlist_node_t *n = gds_dlist_node_get_container_of(current);
	void *data = gds_dlist_node_get_data(n);
	void *(*callback)(void *, void *, unsigned int, void *) = params[0];
	void *callback_data = params[1];
	void **result = params[2];

	*result = callback(*result, data, pos, callback_data);
}

void * gds_dlist_reduce(gds_dlist_t *list, void *callback, void *callback_data)
{
	gds_inline_dlist_node_t *i;
	void *result = NULL;

	GDS_CHECK_ARG_NOT_NULL(list);
	GDS_CHECK_ARG_NOT_NULL(callback);

	if (list->head != NULL) {
		i = gds_dlist_node_get_inline(list->head);
		gds_inline_dlist_map(i, _gds_dlist_reduce_callback,
			(void *[]) {callback, callback_data, &result});
	}

	return result;
}

unsigned int gds_dlist_size(gds_dlist_t *list)
{
	GDS_CHECK_ARG_NOT_NULL(list);

	return list->size;
}

void gds_dlist_free(gds_dlist_t *list, void *callback, void *callback_data)
{
	gds_dlist_node_t *tmp;
	gds_inline_dlist_node_t *ti, *ti2;

	if (list != NULL && list->head != NULL) {
		ti = gds_dlist_node_get_inline(list->head);
		while (ti != NULL) {
			ti2 = gds_inline_dlist_node_get_next(ti);
			tmp = gds_dlist_node_get_container_of(ti);
			gds_dlist_node_free(tmp, callback, callback_data);
			ti = ti2;
		}
	}
	free(list);
}

typedef struct {
	gds_dlist_t *list;
	gds_inline_dlist_node_t *cur;
} gds_dlist_iterator_data_t;

int8_t gds_dlist_iterator_reset(gds_dlist_iterator_data_t *it_data)
{
	GDS_CHECK_ARG_NOT_NULL(it_data);

	it_data->cur = NULL;

	return 0;
}

int8_t gds_dlist_iterator_step(gds_dlist_iterator_data_t *it_data)
{
	gds_inline_dlist_node_t *next = NULL;

	GDS_CHECK_ARG_NOT_NULL(it_data);

	if (it_data->cur != NULL) {
		next = gds_inline_dlist_node_get_next(it_data->cur);
	} else if(it_data->list->head != NULL) {
		next = gds_dlist_node_get_inline(it_data->list->head);
	}

	if (next == NULL) {
		return 1;
	}

	it_data->cur = next;

	return 0;
}

void * gds_dlist_iterator_get(gds_dlist_iterator_data_t *it_data)
{
	gds_dlist_node_t *node;

	GDS_CHECK_ARG_NOT_NULL(it_data);

	node = gds_dlist_node_get_container_of(it_data->cur);
	return gds_dlist_node_get_data(node);
}

gds_iterator_t * gds_dlist_iterator_new(gds_dlist_t *list)
{
	gds_dlist_iterator_data_t *it_data;
	gds_iterator_t *it;

	it_data = malloc(sizeof(gds_dlist_iterator_data_t));
	if (it_data == NULL) {
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_dlist_iterator_data_t));
	}

	it_data->list = list;
	it_data->cur = NULL;

	it = gds_iterator_new(it_data,
		(gds_iterator_reset_cb) &gds_dlist_iterator_reset,
		(gds_iterator_step_cb) &gds_dlist_iterator_step,
		(gds_iterator_get_cb) &gds_dlist_iterator_get,
		(gds_free_cb) &free);

	return it;
}
