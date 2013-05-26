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
#include "inline/slist.h"

void gds_inline_slist_node_init(gds_inline_slist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->next = NULL;
}

gds_inline_slist_node_t * gds_inline_slist_node_get_next(
	gds_inline_slist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->next;
}

int gds_inline_slist_node_set_next(gds_inline_slist_node_t *node,
	gds_inline_slist_node_t *next)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->next = next;

	return 0;
}

int gds_inline_slist_node_append_list(gds_inline_slist_node_t *node,
	gds_inline_slist_node_t *head, gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *tmp;
	int added = 0;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (head != NULL) {
		added = 1;
		tmp = head;
		while (tmp->next != NULL && tmp->next != node) {
			tmp = tmp->next;
			added++;
		}
		if (tmp->next == node) {
			GDS_LOG_ERROR("<node> and <next> are in the same list. Refusing to continue.");
			return -1;
		}
		tmp->next = node->next;
		node->next = head;

		if (tmp->next == NULL && newtail != NULL) *newtail = tmp;
	}

	return added;
}


int gds_inline_slist_insert(gds_inline_slist_node_t **head1,
	unsigned int offset, gds_inline_slist_node_t *head2,
	gds_inline_slist_node_t **newtail)
{
	unsigned int i, added = 0;
	gds_inline_slist_node_t *tmp;

	GDS_CHECK_ARG_NOT_NULL(head1);
	GDS_CHECK_ARG_NOT_NULL(*head1);

	if (head2) {
		if (offset == 0) {
			tmp = head2;
			while (tmp->next != NULL) {
				tmp = tmp->next;
				added++;
			}
			added++;
			gds_inline_slist_node_set_next(tmp, *head1);
			*head1 = head2;
		} else {
			tmp = *head1;
			i = 1;
			while (i < offset && tmp->next != NULL) {
				tmp = tmp->next;
				i++;
			}
			added = gds_inline_slist_node_append_list(tmp, head2,
				newtail);
		}
	}

	return added;
}

int gds_inline_slist_remove(gds_inline_slist_node_t **head,
	unsigned int offset, unsigned int length, void *callback,
	void *callback_data, gds_inline_slist_node_t **newtail)
{
	gds_inline_slist_node_t *tmp, *tmp_prev, *tmp_next;
	gds_inline_slist_node_t *before_rm, *after_rm;
	void (*cb)(gds_inline_slist_node_t *, void *) = callback;
	unsigned int i = 0;
	int removed = 0;

	GDS_CHECK_ARG_NOT_NULL(head);
	GDS_CHECK_ARG_NOT_NULL(*head);

	if (length > 0) {
		tmp = *head;
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

		if (before_rm != NULL) {
			before_rm->next = after_rm;
		} else {
			*head = after_rm;
		}

		if (newtail != NULL && after_rm == NULL) {
			*newtail = before_rm;
		}
	}

	return removed;
}

int gds_inline_slist_remove_last(gds_inline_slist_node_t **head,
	void *callback, void *callback_data, gds_inline_slist_node_t **newlast)
{
	gds_inline_slist_node_t *tmp, *tmp2 = NULL;
	void (*cb)(gds_inline_slist_node_t *, void *) = callback;

	GDS_CHECK_ARG_NOT_NULL(head);
	GDS_CHECK_ARG_NOT_NULL(*head);

	tmp = *head;
	while (tmp->next != NULL) {
		tmp2 = tmp;
		tmp = tmp->next;
	}

	if (cb) cb(tmp, callback_data);

	if (tmp2) tmp2->next = NULL;
	else *head = NULL;
	if (newlast) *newlast = tmp2;

	return 0;
}

gds_inline_slist_node_t * gds_inline_slist_get(
	gds_inline_slist_node_t *head, unsigned int offset)
{
	gds_inline_slist_node_t *tmp = head;
	unsigned int i = 0;

	GDS_CHECK_ARG_NOT_NULL(head);

	while (i < offset && tmp != NULL) {
		tmp = tmp->next;
		i++;
	}

	return tmp;
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
