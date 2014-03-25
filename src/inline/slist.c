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
#include "../assert.h"
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

unsigned int _gds_inline_slist_remove(gds_inline_slist_node_t *head,
	unsigned int offset, unsigned int length, void *callback,
	gds_inline_slist_node_t **before, gds_inline_slist_node_t **after,
	gds_inline_slist_node_t **newhead, gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *tmp, *tmp_prev, *tmp_next;
	gds_inline_slist_node_t *before_rm, *after_rm;
	void (*cb)(gds_inline_slist_node_t *) = callback;
	unsigned int i = 0;
	int removed = 0;

	gds_assert(head != NULL, 0);

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
				cb(tmp);
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

int gds_inline_slist_node_init(gds_inline_slist_node_t *node)
{
	gds_assert(node != NULL, -1);

	node->next = NULL;

	return 0;
}

int gds_inline_slist_node_set_next(gds_inline_slist_node_t *node,
	gds_inline_slist_node_t *next)
{
	gds_assert(node != NULL, -1);

	node->next = next;

	return 0;
}

gds_inline_slist_node_t * gds_inline_slist_node_get_next(
	gds_inline_slist_node_t *node)
{
	gds_inline_slist_node_t *next = NULL;

	if (node != NULL) {
		next = node->next;
	}

	return next;
}

unsigned int gds_inline_slist_node_append(gds_inline_slist_node_t *node,
	gds_inline_slist_node_t *list, gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *tmp;
	int added = 0;

	gds_assert(node != NULL, 0);

	if (list != NULL) {
		added = 1;
		tmp = list;
		while (tmp->next != NULL && tmp->next != node) {
			tmp = tmp->next;
			added++;
		}
		if (tmp->next == node) {
			gds_log_error("<node> and <next> are in the same list. Refusing to continue.");
			return 0;
		}
		tmp->next = node->next;
		node->next = list;

		if (tmp->next == NULL && newtail != NULL) *newtail = tmp;
	}

	return added;
}

unsigned int gds_inline_slist_insert(gds_inline_slist_node_t *head,
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

unsigned int gds_inline_slist_remove(gds_inline_slist_node_t *head,
	unsigned int offset, unsigned int length, void *callback,
	gds_inline_slist_node_t **newhead, gds_inline_slist_node_t **newtail)
{
	return _gds_inline_slist_remove(head, offset, length, callback,
		NULL, NULL, newhead, newtail);
}

unsigned int gds_inline_slist_remove_tail(gds_inline_slist_node_t *head,
	void *callback, gds_inline_slist_node_t **newhead,
	gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *tmp, *tmp2 = NULL;
	void (*cb)(gds_inline_slist_node_t *) = callback;

	gds_assert(head != NULL, 0);

	tmp = head;
	while (tmp->next != NULL) {
		tmp2 = tmp;
		tmp = tmp->next;
	}

	if (cb) cb(tmp);

	if (tmp2 != NULL) tmp2->next = NULL;
	else if (newhead != NULL) *newhead = NULL;
	if (newtail != NULL) *newtail = tmp2;

	return 1;
}

int gds_inline_slist_splice(gds_inline_slist_node_t *node, unsigned int offset,
	unsigned int length, void *callback, gds_inline_slist_node_t *list,
	gds_inline_slist_node_t **newhead, gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *before_rm = NULL, *after_rm = NULL;
	int removed = 0, added = 0, ret;

	if (length != 0) {
		removed = _gds_inline_slist_remove(node, offset, length,
			callback, &before_rm, &after_rm, newhead,
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

unsigned int gds_inline_slist_size(gds_inline_slist_node_t *head)
{
	gds_inline_slist_node_t *curr = head;
	unsigned int size = 0;

	while (curr != NULL) {
		curr = curr->next;
		size++;
	}

	return size;
}

int gds_inline_slist_map(gds_inline_slist_node_t *head, void *callback)
{
	gds_inline_slist_node_t *curr = head;
	void (*cb)(gds_inline_slist_node_t *) = callback;

	gds_assert(callback != NULL, -1);

	while (curr != NULL) {
		cb(curr);
		curr = curr->next;
	}

	return 0;
}
