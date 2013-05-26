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
#include "slist_node.h"
#include "slist.h"
#include "undefined.h"

struct gds_slist_s {
	gds_slist_node_t *first;
	gds_slist_node_t *last;
	unsigned int size;
};

gds_slist_t * gds_slist_new(void)
{
	gds_slist_t *list = malloc(sizeof(gds_slist_t));

	if (list) {
		list->first = list->last = NULL;
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
	gds_inline_slist_node_t *i, *fi;

	GDS_CHECK_ARG_NOT_NULL(list);

	n = gds_slist_node_new(data);
	if (list->first != NULL) {
		i = gds_slist_node_get_inline(n);
		fi = gds_slist_node_get_inline(list->first);
		gds_inline_slist_node_set_next(i, fi);
	} else {
		list->last = n;
	}
	list->first = n;

	list->size++;

	return 0;
}

int gds_slist_push(gds_slist_t *list, void *data)
{
	gds_slist_node_t *n;
	gds_inline_slist_node_t *i, *li;

	GDS_CHECK_ARG_NOT_NULL(list);

	n = gds_slist_node_new(data);

	if (list->last != NULL) {
		i = gds_slist_node_get_inline(n);
		li = gds_slist_node_get_inline(list->last);
		gds_inline_slist_node_set_next(li, i);
	} else {
		list->first = n;
	}
	list->last = n;

	list->size++;

	return 0;
}

void * gds_slist_shift(gds_slist_t *list)
{
	gds_slist_node_t *n = NULL;
	gds_inline_slist_node_t *i, *fi;
	void *d = UNDEFINED;

	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->first != NULL) {
		d = gds_slist_node_get_data(list->first);
		fi = gds_slist_node_get_inline(list->first);
		i = gds_inline_slist_node_get_next(fi);
		n = gds_slist_node_get_container_of(i);
		gds_slist_node_free(list->first, NULL, NULL);
		list->first = n;
		if (list->first == NULL) list->last = NULL;

		list->size--;
	}

	return d;
}

void * gds_slist_pop(gds_slist_t *list)
{
	gds_inline_slist_node_t *fi, *li;
	void *d = UNDEFINED;

	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->last != NULL) {
		d = gds_slist_node_get_data(list->last);
		fi = gds_slist_node_get_inline(list->first);
		gds_inline_slist_remove_last(&fi, NULL, NULL, &li);
		gds_slist_node_free(list->last, NULL, NULL);
		list->first = gds_slist_node_get_container_of(fi);
		list->last = gds_slist_node_get_container_of(li);

		list->size--;
	}

	return d;
}

void * gds_slist_get(gds_slist_t *list, unsigned int offset)
{
	void *d = UNDEFINED;
	gds_slist_node_t *n;
	gds_inline_slist_node_t *i;
	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->first != NULL) {
		i = gds_slist_node_get_inline(list->first);
		i = gds_inline_slist_get(i, offset);
		n = gds_slist_node_get_container_of(i);
		if (n != NULL) {
			d = gds_slist_node_get_data(n);
		}
	}

	return d;
}

void gds_slist_node_remove_callback(gds_inline_slist_node_t *node_inline,
	void *params[2])
{
	gds_slist_node_t *node;
	void (*callback)(gds_slist_node_t *, void *) = params[0];
	void *callback_data = params[1];

	GDS_CHECK_ARG_NOT_NULL(node_inline);

	node = gds_slist_node_get_container_of(node_inline);
	gds_slist_node_free(node, callback, callback_data);
}


void gds_slist_splice(gds_slist_t *list, unsigned int offset,
	unsigned int length, void *callback, void *callback_data,
	gds_slist_t *replacement)
{
	gds_inline_slist_node_t *fi, *li, *ci;
	gds_slist_node_t *copy;
	int removed, added = 0;

	GDS_CHECK_ARG_NOT_NULL(list);

	if (list->first) {
		fi = gds_slist_node_get_inline(list->first);
		li = gds_slist_node_get_inline(list->last);
		removed = gds_inline_slist_remove(&fi, offset, length,
			gds_slist_node_remove_callback,
			(void *[]){callback, callback_data}, &li);
		if (replacement) {
			copy = gds_slist_node_copy(replacement->first);
			ci = gds_slist_node_get_inline(copy);
			added = gds_inline_slist_insert(&fi, offset, ci, &li);
		}
		list->first = gds_slist_node_get_container_of(fi);
		list->last = gds_slist_node_get_container_of(li);

		list->size += added - removed;
	}
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

	if (list->first != NULL) {
		i = gds_slist_node_get_inline(list->first);
		i = gds_inline_slist_get(i, offset);
		while (i != NULL && cnt < length) {
			n = gds_slist_node_get_container_of(i);
			data = gds_slist_node_get_data(n);
			if (cb != NULL) {
				gds_slist_push(slice, cb(data, callback_data));
			} else {
				gds_slist_push(slice, data);
			}
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
	gds_inline_slist_node_t *i;

	GDS_CHECK_ARG_NOT_NULL(list);
	GDS_CHECK_ARG_NOT_NULL(callback);

	if (list->first != NULL) {
		i = gds_slist_node_get_inline(list->first);
		gds_inline_slist_map(i, gds_slist_map_callback,
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

	if (list->first != NULL) {
		i = gds_slist_node_get_inline(list->first);
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

	if (list->first != NULL) {
		i = gds_slist_node_get_inline(list->first);
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

	if (list != NULL && list->first != NULL) {
		ti = gds_slist_node_get_inline(list->first);
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
} gds_slist_iterator_data_t;

int8_t gds_slist_iterator_reset(gds_slist_iterator_data_t *it_data)
{
	GDS_CHECK_ARG_NOT_NULL(it_data);

	it_data->cur = NULL;

	return 0;
}

int8_t gds_slist_iterator_step(gds_slist_iterator_data_t *it_data)
{
	gds_inline_slist_node_t *next = NULL;

	GDS_CHECK_ARG_NOT_NULL(it_data);

	if (it_data->cur != NULL) {
		next = gds_inline_slist_node_get_next(it_data->cur);
	} else if(it_data->list->first != NULL) {
		next = gds_slist_node_get_inline(it_data->list->first);
	}

	if (next == NULL) {
		return 1;
	}

	it_data->cur = next;

	return 0;
}

void * gds_slist_iterator_get(gds_slist_iterator_data_t *it_data)
{
	gds_slist_node_t *node;

	GDS_CHECK_ARG_NOT_NULL(it_data);

	node = gds_slist_node_get_container_of(it_data->cur);
	return gds_slist_node_get_data(node);
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
		(gds_free_cb) &free);
	
	return it;
}
