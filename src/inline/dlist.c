/*
 * Copyright 2013 Julian Maurice
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
#include "../check_arg.h"
#include "inline/dlist.h"
#include "undefined.h"

/* INTERNAL FUNCTIONS */

static int _gds_inline_dlist_head_tail(gds_inline_dlist_node_t *node,
	gds_inline_dlist_node_t *check, gds_inline_dlist_node_t **head,
	gds_inline_dlist_node_t **tail)
{
	gds_inline_dlist_node_t *tmp;
	int size = 1;

	/* Get head */
	tmp = node;
	while (tmp->prev != NULL && tmp->prev != check) {
		tmp = tmp->prev;
		size++;
	}
	if (check != NULL && tmp->prev == check) {
		return -1;
	}
	if (head != NULL) *head = tmp;

	/* Get tail */
	tmp = node;
	while (tmp->next != NULL && tmp->next != check) {
		tmp = tmp->next;
		size++;
	}
	if (check != NULL && tmp->next == check) {
		return -1;
	}
	if (tail != NULL) *tail = tmp;

	return size;
}

static unsigned int _gds_inline_dlist_node_remove(gds_inline_dlist_node_t *node,
	unsigned int length_before, unsigned int length_after, void *callback,
	void *callback_data, gds_inline_dlist_node_t **before,
	gds_inline_dlist_node_t **after)
{
	unsigned int i, deleted_nodes = 0;
	gds_inline_dlist_node_t *it, *a, *b;
	void (*cb)(gds_inline_dlist_node_t *, void *) = callback;

	if (node != NULL) {
		i = 0; it = b = node->prev;
		while (it != NULL && i < length_before) {
			b = it->prev;
			if (cb != NULL) cb(it, callback_data);
			it = b;
			i++;
		}
		deleted_nodes += i;

		i = 0; it = a = node->next;
		while (it != NULL && i < length_after) {
			a = it->next;
			if (cb != NULL) cb(it, callback_data);
			it = a;
			i++;
		}
		deleted_nodes += i;

		if (cb != NULL) cb(node, callback_data);
		deleted_nodes++;

		if (b != NULL) b->next = a;
		if (a != NULL) a->prev = b;

		if (before != NULL) *before = b;
		if (after != NULL) *after = a;
	}

	return deleted_nodes;
}

static unsigned int _gds_inline_dlist_remove(gds_inline_dlist_node_t *node,
	int offset, int length, void *callback, void *callback_data,
	gds_inline_dlist_node_t **before, gds_inline_dlist_node_t **after)
{
	unsigned int length_before = 0, length_after = 0;
	int offset1, offset2;
	unsigned int removed = 0;

	if (length != 0) {
		if (length < 0) {
			offset1 = offset + length + 1;
			offset2 = offset;
		} else {
			offset1 = offset;
			offset2 = offset + length - 1;
		}

		if (offset1 >= 0) {
			length_after = offset2 - offset1;
			node = gds_inline_dlist_get(node, offset1);
		} else if (offset2 <= 0) {
			length_before = offset2 - offset1;
			node = gds_inline_dlist_get(node, offset2);
		} else {
			/* offset1 < 0 && offset2 > 0 */
			length_before = -offset1;
			length_after = offset2;
		}
		removed = _gds_inline_dlist_node_remove(node, length_before,
			length_after, callback, callback_data, before, after);
	}

	return removed;
}

static gds_inline_dlist_node_t * _gds_inline_dlist_get(
	gds_inline_dlist_node_t *node, int offset,
	gds_inline_dlist_node_t **before, gds_inline_dlist_node_t **after)
{
	gds_inline_dlist_node_t *it = NULL, *a = NULL, *b = NULL;
	int i = 0;

	if (node != NULL) {
		b = node->prev;
		a = node->next;
		it = node;
		if (offset >= 0) {
			while (it != NULL && i < offset) {
				b = it;
				it = it->next;
				i++;
			}
			a = (it != NULL) ? it->next : it;
		} else {
			while (it != NULL && i > offset) {
				a = it;
				it = it->prev;
				i--;
			}
			b = (it != NULL) ? it->prev : NULL;
		}
	}

	if (before != NULL) *before = b;
	if (after != NULL) *after = a;

	return it;
}

/* PUBLIC API */

void gds_inline_dlist_node_init(gds_inline_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->next = node->prev = NULL;
}

gds_inline_dlist_node_t * gds_inline_dlist_node_prev(
	gds_inline_dlist_node_t *node, gds_inline_dlist_node_t *prev)
{
	gds_inline_dlist_node_t *_prev = UNDEFINED;

	if (!NULL_OR_UNDEFINED(node)) {
		if (prev != UNDEFINED) {
			node->prev = prev;
		}
		_prev = node->prev;
	}

	return _prev;
}

gds_inline_dlist_node_t * gds_inline_dlist_node_next(
	gds_inline_dlist_node_t *node, gds_inline_dlist_node_t *next)
{
	gds_inline_dlist_node_t *_next = UNDEFINED;

	if (!NULL_OR_UNDEFINED(node)) {
		if (next != UNDEFINED) {
			node->next = next;
		}
		_next = node->next;
	}

	return _next;
}

int gds_inline_dlist_node_prepend_list(gds_inline_dlist_node_t *node1,
	gds_inline_dlist_node_t *node2, gds_inline_dlist_node_t **newhead)
{
	gds_inline_dlist_node_t *head, *tail;
	int added = 0;

	GDS_CHECK_ARG_NOT_NULL(node1);

	if (node2 != NULL) {
		added = _gds_inline_dlist_head_tail(node2, node1, &head, &tail);
		if (added < 0) {
			GDS_LOG_ERROR("node1 and node2 are in the same list. "
				"Refusing to continue.");
			return -1;
		}

		head->prev = node1->prev;
		if (head->prev != NULL) head->prev->next = head;
		tail->next = node1;
		node1->prev = tail;

		if (head->prev == NULL && newhead != NULL) *newhead = head;
	}

	return added;
}

int gds_inline_dlist_node_append_list(gds_inline_dlist_node_t *node1,
	gds_inline_dlist_node_t *node2, gds_inline_dlist_node_t **newtail)
{
	gds_inline_dlist_node_t *head, *tail;
	int added = 0;

	GDS_CHECK_ARG_NOT_NULL(node1);

	if (node2 != NULL) {
		added = _gds_inline_dlist_head_tail(node2, node1, &head, &tail);
		if (added < 0) {
			GDS_LOG_ERROR("node1 and node2 are in the same list. "
				"Refusing to continue.");
			return -1;
		}

		tail->next = node1->next;
		if (tail->next != NULL) tail->next->prev = tail;
		head->prev = node1;
		node1->next = head;

		if (tail->next == NULL && newtail != NULL) *newtail = tail;
	}

	return added;
}

unsigned int gds_inline_dlist_remove(gds_inline_dlist_node_t *node, int offset,
	int length, void *callback, void *callback_data,
	gds_inline_dlist_node_t **newhead, gds_inline_dlist_node_t **newtail)
{
	gds_inline_dlist_node_t *before = UNDEFINED, *after = UNDEFINED;
	unsigned int removed;

	removed = _gds_inline_dlist_remove(node, offset, length, callback,
		callback_data, &before, &after);

	if (before == NULL && newhead != NULL) *newhead = after;
	if (after == NULL && newtail != NULL) *newtail = before;

	return removed;
}

int gds_inline_dlist_insert(gds_inline_dlist_node_t *node, int offset,
	gds_inline_dlist_node_t *list, gds_inline_dlist_node_t **newhead,
	gds_inline_dlist_node_t **newtail)
{
	gds_inline_dlist_node_t *before, *after;
	int added = 0;

	if (list != NULL) {
		node = _gds_inline_dlist_get(node, offset, &before, &after);
		if (node == NULL) node = after;

		if (node != NULL) {
			added = gds_inline_dlist_node_prepend_list(node,
				list, newhead);
		} else if (before != NULL) {
			added = gds_inline_dlist_node_append_list(before,
				list, newtail);
		} else {
			added = _gds_inline_dlist_head_tail(list, NULL,
				newhead, newtail);
		}
	}

	return added;
}

int gds_inline_dlist_splice(gds_inline_dlist_node_t *node, int offset,
	int length, void *callback, void *callback_data,
	gds_inline_dlist_node_t *replacement, gds_inline_dlist_node_t **newhead,
	gds_inline_dlist_node_t **newtail)
{
	gds_inline_dlist_node_t *before_rm = UNDEFINED, *after_rm = UNDEFINED;
	int removed = 0, added = 0, ret;

	if (length != 0) {
		removed = _gds_inline_dlist_remove(node, offset, length,
			callback, callback_data, &before_rm, &after_rm);

		if (before_rm == NULL && newhead != NULL) *newhead = after_rm;
		if (after_rm == NULL && newtail != NULL) *newtail = before_rm;

		/* Set node and offset for insert. */
		if (after_rm != NULL) {
			node = after_rm;
			offset = 0;
		} else if (before_rm != NULL) {
			node = before_rm;
			offset = 1;
		} else {
			node = NULL;
		}
	}

	added = gds_inline_dlist_insert(node, offset, replacement, newhead,
		newtail);

	ret = (added > 0) ? added - removed : -removed;

	return ret;
}

gds_inline_dlist_node_t * gds_inline_dlist_get(gds_inline_dlist_node_t *node,
	int offset)
{
	gds_inline_dlist_node_t *tmp = node;
	int i = 0;

	if (offset > 0) {
		while (i < offset && tmp != NULL) {
			tmp = tmp->next;
			i++;
		}
	} else {
		while (i > offset && tmp != NULL) {
			tmp = tmp->prev;
			i--;
		}
	}

	return tmp;
}

gds_inline_dlist_node_t * gds_inline_dlist_get_head(
	gds_inline_dlist_node_t *node)
{
	gds_inline_dlist_node_t *tmp = NULL;

	if (node != NULL) {
		tmp = node;
		while (tmp->prev != NULL) {
			tmp = tmp->prev;
		}
	}

	return tmp;
}

gds_inline_dlist_node_t * gds_inline_dlist_get_tail(
	gds_inline_dlist_node_t *node)
{
	gds_inline_dlist_node_t *tmp = NULL;

	if (node != NULL) {
		tmp = node;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
	}

	return tmp;
}

int gds_inline_dlist_size(gds_inline_dlist_node_t *head)
{
	gds_inline_dlist_node_t *curr = head;
	unsigned int size = 0;

	while (curr != NULL) {
		curr = curr->next;
		size++;
	}

	return size;
}

void gds_inline_dlist_map(gds_inline_dlist_node_t *head, void *callback,
	void *callback_data)
{
	gds_inline_dlist_node_t *curr = head;
	void (*cb)(gds_inline_dlist_node_t *, unsigned int, void *) = callback;
	unsigned int i = 0;

	GDS_CHECK_ARG_NOT_NULL(callback);

	while (curr != NULL) {
		cb(curr, i, callback_data);
		curr = curr->next;
		i++;
	}
}
