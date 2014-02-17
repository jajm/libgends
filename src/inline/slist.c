/*
 * Copyright 2013-2014 Julian Maurice
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
#include "undefined.h"
#include "inline/slist.h"

/* INTERNAL FUNCTIONS */

gds_inline_slist_node_t * _gds_inline_slist_get(gds_inline_slist_node_t *node,
	unsigned int offset, gds_inline_slist_node_t **before)
{
	gds_inline_slist_node_t *b = NULL;
	unsigned int i = 0;

	while (node != NULL && i < offset) {
		b = node;
		node = node->next;
		i++;
	}

	if (before != NULL) *before = b;

	return node;
}

unsigned int _gds_inline_slist_tail(gds_inline_slist_node_t *node,
	gds_inline_slist_node_t **tail)
{
	unsigned int size = 0;

	if (node != NULL) {
		size = 1;
		while (node->next != NULL) {
			size++;
			node = node->next;
		}
	}

	if (tail != NULL) *tail = node;

	return size;
}

int _gds_inline_slist_remove(gds_inline_slist_node_t *head,
	unsigned int offset, unsigned int length, void *callback,
	void *callback_data, gds_inline_slist_node_t **before,
	gds_inline_slist_node_t **after, gds_inline_slist_node_t **newhead,
	gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *tmp, *tmp_prev, *tmp_next;
	gds_inline_slist_node_t *before_rm, *after_rm;
	void (*cb)(gds_inline_slist_node_t *, void *) = callback;
	unsigned int i = 0;
	int removed = 0;

	GDS_CHECK_ARG_NOT_NULL(head);

	if (length > 0) {
		tmp = head;
		tmp_prev = NULL;
		while (i < offset && tmp->next != NULL) {
			tmp_prev = tmp;
			tmp = tmp->next;
			i++;
		}
		if (i < offset) {
			/* offset is greater than list size */
			return 0;
		}
		before_rm = tmp_prev;

		i = 0;
		while (i < length && tmp != NULL) {
			tmp_next = tmp->next;

			if (cb != NULL) {
				cb(tmp, callback_data);
			}

			tmp = tmp_next;
			removed++;
			i++;
		}
		after_rm = tmp;

		if (before_rm != NULL) before_rm->next = after_rm;

		if (before != NULL) *before = before_rm;
		if (after != NULL) *after = after_rm;

		if (newhead != NULL && before_rm == NULL) {
			*newhead = after_rm;
		}
		if (newtail != NULL && after_rm == NULL) {
			*newtail = before_rm;
		}
	}

	return removed;
}

/* PUBLIC API */

void gds_inline_slist_node_init(gds_inline_slist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->next = NULL;
}

gds_inline_slist_node_t * gds_inline_slist_node_next(
	gds_inline_slist_node_t *node, gds_inline_slist_node_t *next)
{
	gds_inline_slist_node_t *_next = UNDEFINED;

	if (isset(node)) {
		if (isdef(next)) {
			node->next = next;
		}
		_next = node->next;
	}

	return _next;
}

int gds_inline_slist_node_append(gds_inline_slist_node_t *node,
	gds_inline_slist_node_t *list, gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *tmp;
	int added = 0;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (list != NULL) {
		added = 1;
		tmp = list;
		while (tmp->next != NULL && tmp->next != node) {
			tmp = tmp->next;
			added++;
		}
		if (tmp->next == node) {
			GDS_LOG_ERROR("<node> and <next> are in the same list. Refusing to continue.");
			return -1;
		}
		tmp->next = node->next;
		node->next = list;

		if (tmp->next == NULL && newtail != NULL) *newtail = tmp;
	}

	return added;
}

int gds_inline_slist_insert(gds_inline_slist_node_t *head,
	unsigned int offset, gds_inline_slist_node_t *list,
	gds_inline_slist_node_t **newhead, gds_inline_slist_node_t **newtail)
{
	unsigned int added = 0;
	gds_inline_slist_node_t *before, *tail;

	if (list != NULL) {
		_gds_inline_slist_get(head, offset, &before);
		if (before != NULL) {
			added = gds_inline_slist_node_append(before, list,
				newtail);
		} else {
			added = _gds_inline_slist_tail(list, &tail);
			tail->next = head;

			if (newhead != NULL) *newhead = list;
			if (tail->next == NULL && newtail != NULL) {
				*newtail = tail;
			}
		}
	}

	return added;
}

int gds_inline_slist_remove(gds_inline_slist_node_t *head,
	unsigned int offset, unsigned int length, void *callback,
	void *callback_data, gds_inline_slist_node_t **newhead,
	gds_inline_slist_node_t **newtail)
{
	return _gds_inline_slist_remove(head, offset, length, callback,
		callback_data, NULL, NULL, newhead, newtail);
}

int gds_inline_slist_remove_tail(gds_inline_slist_node_t *head, void *callback,
	void *callback_data, gds_inline_slist_node_t **newhead,
	gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *tmp, *tmp2 = NULL;
	void (*cb)(gds_inline_slist_node_t *, void *) = callback;

	GDS_CHECK_ARG_NOT_NULL(head);

	tmp = head;
	while (tmp->next != NULL) {
		tmp2 = tmp;
		tmp = tmp->next;
	}

	if (cb) cb(tmp, callback_data);

	if (tmp2 != NULL) tmp2->next = NULL;
	else if (newhead != NULL) *newhead = NULL;
	if (newtail != NULL) *newtail = tmp2;

	return 0;
}

int gds_inline_slist_splice(gds_inline_slist_node_t *node, unsigned int offset,
	unsigned int length, void *callback, void *callback_data,
	gds_inline_slist_node_t *list, gds_inline_slist_node_t **newhead,
	gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *before_rm = UNDEFINED, *after_rm = UNDEFINED;
	int removed = 0, added = 0, ret;

	if (length != 0) {
		removed = _gds_inline_slist_remove(node, offset, length,
			callback, callback_data, &before_rm, &after_rm, newhead,
			newtail);

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

	added = gds_inline_slist_insert(node, offset, list, newhead,
		newtail);

	ret = (added > 0) ? added - removed : -removed;

	return ret;
}

gds_inline_slist_node_t * gds_inline_slist_get(gds_inline_slist_node_t *node,
	unsigned int offset)
{
	gds_inline_slist_node_t *tmp = node;
	unsigned int i = 0;

	while (i < offset && tmp != NULL) {
		tmp = tmp->next;
		i++;
	}

	return tmp;
}

gds_inline_slist_node_t * gds_inline_slist_get_tail(
	gds_inline_slist_node_t *node)
{
	gds_inline_slist_node_t *tmp = node;

	if (tmp != NULL) {
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
	}

	return tmp;
}

int gds_inline_slist_size(gds_inline_slist_node_t *head)
{
	gds_inline_slist_node_t *curr = head;
	unsigned int size = 0;

	while (curr != NULL) {
		curr = curr->next;
		size++;
	}

	return size;
}

void gds_inline_slist_map(gds_inline_slist_node_t *head, void *callback,
	void *callback_data)
{
	gds_inline_slist_node_t *curr = head;
	void (*cb)(gds_inline_slist_node_t *, unsigned int, void *) = callback;
	unsigned int i = 0;

	GDS_CHECK_ARG_NOT_NULL(head);
	GDS_CHECK_ARG_NOT_NULL(callback);

	while (curr != NULL) {
		cb(curr, i, callback_data);
		curr = curr->next;
		i++;
	}
}
