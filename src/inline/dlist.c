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

void gds_inline_dlist_node_init(gds_inline_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->next = node->prev = NULL;
}

gds_inline_dlist_node_t * gds_inline_dlist_node_get_prev(
	gds_inline_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->prev;
}

gds_inline_dlist_node_t * gds_inline_dlist_node_get_next(
	gds_inline_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->next;
}

int gds_inline_dlist_node_set_prev(gds_inline_dlist_node_t *node,
	gds_inline_dlist_node_t *prev)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->prev = prev;

	return 0;
}

int gds_inline_dlist_node_set_next(gds_inline_dlist_node_t *node,
	gds_inline_dlist_node_t *next)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->next = next;

	return 0;
}

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
	*head = tmp;

	/* Get tail */
	tmp = node;
	while (tmp->next != NULL && tmp->next != check) {
		tmp = tmp->next;
		size++;
	}
	if (check != NULL && tmp->next == check) {
		return -1;
	}
	*tail = tmp;

	return size;
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

int _gds_inline_dlist_remove(gds_inline_dlist_node_t *node, int offset,
	int length, void *callback, void *callback_data, int backward,
	gds_inline_dlist_node_t **before, gds_inline_dlist_node_t **after)
{
	gds_inline_dlist_node_t * (*prev)(gds_inline_dlist_node_t *);
	int (*set_prev)(gds_inline_dlist_node_t *, gds_inline_dlist_node_t *);
	gds_inline_dlist_node_t * (*next)(gds_inline_dlist_node_t *);
	int (*set_next)(gds_inline_dlist_node_t *, gds_inline_dlist_node_t *);
	void (*cb)(gds_inline_dlist_node_t *, void *) = callback;
	gds_inline_dlist_node_t *a, *b, *tmp, *tmp2;
	int i, n;

	offset = abs(offset);
	length = abs(length);

	if (backward) {
		prev = gds_inline_dlist_node_get_next;
		set_prev = gds_inline_dlist_node_set_next;
		next = gds_inline_dlist_node_get_prev;
		set_next = gds_inline_dlist_node_set_prev;
	} else {
		prev = gds_inline_dlist_node_get_prev;
		set_prev = gds_inline_dlist_node_set_prev;
		next = gds_inline_dlist_node_get_next;
		set_next = gds_inline_dlist_node_set_next;
	}

	b = prev(node);
	tmp = node;
	i = 0;
	while (tmp != NULL && i != offset) {
		b = tmp;
		tmp = next(tmp);
		i++;
	}

	n = 0;
	while (tmp != NULL && i != offset+length) {
		tmp2 = next(tmp);
		if (cb != NULL) {
			cb(tmp, callback_data);
		}
		tmp = tmp2;
		n++;
		i++;
	}
	a = tmp;

	if (a != NULL) set_prev(a, b);
	if (b != NULL) set_next(b, a);

	if (before != NULL) {
		*before = backward ? a : b;
	}
	if (after != NULL) {
		*after = backward ? b : a;
	}

	return n;
}

int gds_inline_dlist_splice(gds_inline_dlist_node_t *node, int offset,
	int length, void *callback, void *callback_data,
	gds_inline_dlist_node_t *replacement, gds_inline_dlist_node_t **newhead,
	gds_inline_dlist_node_t **newtail)
{
	gds_inline_dlist_node_t *before_rm;
	gds_inline_dlist_node_t *after_rm;
	int offset1, offset2;
	int added = 0;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (length < 0) {
		offset1 = offset + length + 1;
		offset2 = offset;
	} else {
		offset1 = offset;
		offset2 = offset + length - 1;
	}

	if (offset1 >= 0) {
		added -= _gds_inline_dlist_remove(node, offset1, length,
			callback, callback_data, 0, &before_rm, &after_rm);
	} else if (offset2 <= 0) {
		added -= _gds_inline_dlist_remove(node, offset2, length,
			callback, callback_data, 1, &before_rm, &after_rm);
	} else {
		/* offset1 < 0 && offset2 > 0 */
		added -= _gds_inline_dlist_remove(node, 1, offset1, callback,
			callback_data, 1, &before_rm, NULL);
		added -= _gds_inline_dlist_remove(node, 0, offset2 + 1,
			callback, callback_data, 0, NULL, &after_rm);
	}

	if (before_rm == NULL && newhead != NULL) {
		*newhead = after_rm;
	}
	if (after_rm == NULL && newtail != NULL) {
		*newtail = before_rm;
	}

	if (replacement != NULL) {
		if (before_rm != NULL) {
			added += gds_inline_dlist_node_append_list(before_rm,
				replacement, newtail);
		} else if (after_rm != NULL) {
			added += gds_inline_dlist_node_prepend_list(after_rm,
				replacement, newhead);
		} else {
			added += _gds_inline_dlist_head_tail(replacement, NULL, newhead,
				newtail);
		}
	}

	return added;
}

gds_inline_dlist_node_t * gds_inline_dlist_get(gds_inline_dlist_node_t *node,
	int offset)
{
	gds_inline_dlist_node_t *tmp = node;
	int i = 0;

	GDS_CHECK_ARG_NOT_NULL(node);

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
	gds_inline_dlist_node_t *tmp = node;

	GDS_CHECK_ARG_NOT_NULL(node);

	while (tmp->prev != NULL) {
		tmp = tmp->prev;
	}

	return tmp;
}

gds_inline_dlist_node_t * gds_inline_dlist_get_tail(
	gds_inline_dlist_node_t *node)
{
	gds_inline_dlist_node_t *tmp = node;

	GDS_CHECK_ARG_NOT_NULL(node);

	while (tmp->next != NULL) {
		tmp = tmp->next;
	}

	return tmp;
}

int gds_inline_dlist_size(gds_inline_dlist_node_t *head)
{
	gds_inline_dlist_node_t *curr = head;
	unsigned int size = 0;

	GDS_CHECK_ARG_NOT_NULL(head);

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

	GDS_CHECK_ARG_NOT_NULL(head);
	GDS_CHECK_ARG_NOT_NULL(callback);

	while (curr != NULL) {
		cb(curr, i, callback_data);
		curr = curr->next;
		i++;
	}
}
