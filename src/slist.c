/*
 * Copyright 2010-2014 Julian Maurice
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

#include <stdlib.h>
#include "malloc.h"
#include "assert.h"
#include "log.h"
#include "inline/slist.h"
#include "slist.h"
#include "container_of.h"

typedef struct {
	void *data;
	gds_inline_slist_node_t slist_node_inline;
} gds_slist_node_t;

static gds_slist_node_t * gds_slist_node_get_container_of(
	gds_inline_slist_node_t *node_inline)
{
	gds_slist_node_t *node = NULL;

	if (node_inline) {
		node = container_of(node_inline, gds_slist_node_t,
			slist_node_inline);
	}

	return node;
}

static gds_inline_slist_node_t *gds_slist_node_get_inline(gds_slist_node_t *node)
{
	return (node != NULL) ? &(node->slist_node_inline) : NULL;
}

static gds_slist_node_t *gds_slist_node_new(void *data)
{
	gds_slist_node_t *node;

	gds_assert(data != NULL, NULL);

	node = gds_malloc(sizeof(gds_slist_node_t));
	node->data = data;
	gds_inline_slist_node_init(&(node->slist_node_inline));

	return node;
}

static void * gds_slist_node_get_data(gds_slist_node_t *node)
{
	void *data;

	gds_assert(node != NULL, NULL);

	data = node->data;

	return data;
}

static gds_slist_node_t * gds_slist_node_copy(gds_slist_node_t *node)
{
	gds_slist_node_t *head, *tmp, *tmp2, *n;
	gds_inline_slist_node_t *i, *ti, *t2i;

	gds_assert(node != NULL, NULL);

	head = gds_slist_node_new(node->data);
	tmp = head;
	ti = gds_slist_node_get_inline(tmp);
	tmp2 = node;
	t2i = gds_slist_node_get_inline(tmp2);
	while ( (t2i = gds_inline_slist_node_get_next(t2i)) != NULL ) {
		tmp2 = gds_slist_node_get_container_of(t2i);
		n = gds_slist_node_new(tmp2->data);
		i = gds_slist_node_get_inline(n);
		gds_inline_slist_node_set_next(ti, i);

		ti = gds_inline_slist_node_get_next(ti);
	}

	return head;
}

static void gds_slist_node_free(gds_slist_node_t *node, void *callback)
{
	void (*cb)(void *) = callback;

	if (cb != NULL && node != NULL) {
		cb(node->data);
	}
	free(node);
}

struct gds_slist_s {
	gds_slist_node_t *head;
	gds_slist_node_t *tail;
	unsigned int size;
	void (*free_cb)(void *);
};

gds_slist_t * gds_slist_new(void *free_cb)
{
	gds_slist_t *list = gds_malloc(sizeof(gds_slist_t));

	list->head = list->tail = NULL;
	list->size = 0;
	list->free_cb = free_cb;

	return list;
}

gds_slist_t * gds_slist_new_from_array(void *free_cb, unsigned int n,
	void *data[])
{
	gds_slist_t *list;
	unsigned int i;

	list = gds_slist_new(free_cb);

	for(i = 0; i < n; i++) {
		gds_slist_push(list, data[i]);
	}

	return list;
}

int gds_slist_set_free_callback(gds_slist_t *list, void *free_cb)
{
	gds_assert(list != NULL, -1);

	list->free_cb = free_cb;

	return 0;
}

void * gds_slist_get_free_callback(gds_slist_t *list)
{
	gds_assert(list != NULL, NULL);

	return list->free_cb;
}

int gds_slist_unshift_array(gds_slist_t *list, size_t size, void *data[])
{
	gds_slist_node_t *node;
	gds_inline_slist_node_t *inode, *head, *tail;
	int added;
	int i;

	gds_assert(list != NULL, -1);

	head = gds_slist_node_get_inline(list->head);
	tail = gds_slist_node_get_inline(list->tail);

	for (i = size - 1; i >= 0; i--) {
		node = gds_slist_node_new(data[i]);
		inode = gds_slist_node_get_inline(node);

		added = gds_inline_slist_insert(head, 0, inode, &head, &tail);
		if (added != 1) {
			gds_log_error("Insertion failed");
			return -1;
		}
		list->size++;
	}

	list->head = gds_slist_node_get_container_of(head);
	list->tail = gds_slist_node_get_container_of(tail);

	return 0;
}

int gds_slist_push_array(gds_slist_t *list, size_t size, void *data[])
{
	gds_slist_node_t *node;
	gds_inline_slist_node_t *inode, *head, *tail;
	int added;
	unsigned int i;

	gds_assert(list != NULL, -1);

	head = gds_slist_node_get_inline(list->head);
	tail = gds_slist_node_get_inline(list->tail);

	for (i = 0; i < size; i++) {
		node = gds_slist_node_new(data[i]);
		inode = gds_slist_node_get_inline(node);
		added = gds_inline_slist_insert(tail, 1, inode, &head, &tail);
		if (added != 1) {
			gds_log_error("Insertion failed");
			return -1;
		}
		list->size++;
	}

	list->head = gds_slist_node_get_container_of(head);
	list->tail = gds_slist_node_get_container_of(tail);

	return 0;
}

void gds_slist_node_remove_callback(gds_inline_slist_node_t *node_inline,
	void *callback)
{
	gds_slist_node_t *node;
	
	node = gds_slist_node_get_container_of(node_inline);
	gds_slist_node_free(node, callback);
}

void * gds_slist_shift(gds_slist_t *list)
{
	gds_inline_slist_node_t *head, *tail;
	int removed;
	void *d = NULL;

	gds_assert(list != NULL, NULL);

	if (list->head != NULL) {
		d = gds_slist_node_get_data(list->head);
		head = gds_slist_node_get_inline(list->head);
		tail = gds_slist_node_get_inline(list->tail);

		removed = gds_inline_slist_remove(head, 0, 1,
			gds_slist_node_remove_callback, NULL, &head, &tail);
		if (removed != 1) {
			gds_log_error("Removal failed");
			return NULL;
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
	void *d = NULL;

	gds_assert(list != NULL, NULL);

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
	void *d = NULL;
	gds_slist_node_t *n;
	gds_inline_slist_node_t *i;

	gds_assert(list != NULL, NULL);

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

int gds_slist_splice(gds_slist_t *list, unsigned int offset,
	unsigned int length, gds_slist_t *replacement)
{
	gds_inline_slist_node_t *head, *tail, *ci = NULL;
	gds_slist_node_t *copy;
	int added;

	gds_assert(list != NULL, -1);

	head = gds_slist_node_get_inline(list->head);
	tail = gds_slist_node_get_inline(list->tail);
	if (replacement != NULL && replacement->head != NULL) {
		copy = gds_slist_node_copy(replacement->head);
		ci = gds_slist_node_get_inline(copy);
	}
	added = gds_inline_slist_splice(head, offset, length,
		gds_slist_node_remove_callback, list->free_cb, ci, &head,
		&tail);

	list->head = gds_slist_node_get_container_of(head);
	list->tail = gds_slist_node_get_container_of(tail);

	list->size += added;

	return 0;
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

	gds_assert(list != NULL, NULL);

	slice = gds_slist_new(list->free_cb);

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

int gds_slist_map(gds_slist_t *list, void *callback, void *callback_data)
{
	gds_inline_slist_node_t *head;

	gds_assert(list != NULL, -1);
	gds_assert(callback != NULL, -1);

	if (list->head != NULL) {
		head = gds_slist_node_get_inline(list->head);
		gds_inline_slist_map(head, gds_slist_map_callback,
			(void *[]) {callback, callback_data});
	}

	return 0;
}

gds_slist_t * gds_slist_filter(gds_slist_t *list, void *callback,
	void *callback_data)
{
	void *d;
	gds_slist_t *l;
	gds_slist_node_t *n;
	gds_inline_slist_node_t *i;
	int (*cb)(void *, void *) = callback;

	gds_assert(list != NULL, NULL);
	gds_assert(callback != NULL, NULL);

	l = gds_slist_new(list->free_cb);

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

	gds_assert(list != NULL, NULL);
	gds_assert(callback != NULL, NULL);

	if (list->head != NULL) {
		i = gds_slist_node_get_inline(list->head);
		gds_inline_slist_map(i, gds_slist_reduce_callback,
			(void *[]) {callback, callback_data, &result});
	}

	return result;
}

unsigned int gds_slist_size(gds_slist_t *list)
{
	gds_assert(list != NULL, 0);

	return list->size;
}

void gds_slist_free(gds_slist_t *list)
{
	gds_slist_node_t *tmp;
	gds_inline_slist_node_t *ti, *ti2;

	if (list != NULL && list->head != NULL) {
		ti = gds_slist_node_get_inline(list->head);
		while (ti != NULL) {
			ti2 = gds_inline_slist_node_get_next(ti);
			tmp = gds_slist_node_get_container_of(ti);
			gds_slist_node_free(tmp, list->free_cb);
			ti = ti2;
		}
	}
	free(list);
}

void gds_slist_destroy(gds_slist_t *list)
{
	gds_slist_set_free_callback(list, NULL);
	gds_slist_free(list);
}

typedef struct {
	gds_slist_t *list;
	gds_inline_slist_node_t *cur;
	int i;
} gds_slist_iterator_data_t;

int gds_slist_iterator_reset(gds_slist_iterator_data_t *it_data)
{
	gds_assert(it_data != NULL, -1);

	it_data->cur = NULL;
	it_data->i = -1;

	return 0;
}

int gds_slist_iterator_step(gds_slist_iterator_data_t *it_data)
{
	gds_inline_slist_node_t *next = NULL;

	gds_assert(it_data != NULL, -1);

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

	gds_assert(it_data != NULL, NULL);

	node = gds_slist_node_get_container_of(it_data->cur);
	return (node != NULL) ? gds_slist_node_get_data(node) : NULL;
}

const void * gds_slist_iterator_getkey(gds_slist_iterator_data_t *it_data)
{
	gds_assert(it_data != NULL, NULL);

	return &(it_data->i);
}

gds_iterator_t * gds_slist_iterator_new(gds_slist_t *list)
{
	gds_slist_iterator_data_t *it_data;
	gds_iterator_t *it;

	it_data = gds_malloc(sizeof(gds_slist_iterator_data_t));

	it_data->list = list;
	it_data->cur = NULL;

	it = gds_iterator_new(it_data, gds_slist_iterator_reset,
		gds_slist_iterator_step, gds_slist_iterator_get,
		gds_slist_iterator_getkey, free);
	
	return it;
}
