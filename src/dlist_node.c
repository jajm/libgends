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

/*
 * File              : dlist_node.c
 * Short description : Double linked list node
 */

#include <stdlib.h>
#include <stdint.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "dlist_node.h"
#include "callbacks.h"
#include "container_of.h"

gds_dlist_node_t * gds_dlist_node_get_container_of(
	gds_inline_dlist_node_t *node_inline)
{
	gds_dlist_node_t *node = NULL;

	if (node_inline) {
		node = container_of(node_inline, gds_dlist_node_t,
			inline_dlist_node);
	}

	return node;
}

gds_inline_dlist_node_t *gds_dlist_node_get_inline(gds_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return &(node->inline_dlist_node);
}

gds_dlist_node_t *gds_dlist_node_new(void *data)
{
	gds_dlist_node_t *node;

	node = malloc(sizeof(gds_dlist_node_t));
	if (node == NULL) {
		GDS_THROW(NotEnoughMemoryException,
			"failed to allocate %d bytes",
			sizeof(gds_dlist_node_t));
	}
	node->data = data;
	gds_inline_dlist_node_init(gds_dlist_node_get_inline(node));

	return node;
}


int8_t gds_dlist_node_set_data(gds_dlist_node_t *node, void *data,
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

void * gds_dlist_node_get_data(gds_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->data;
}

gds_dlist_node_t * gds_dlist_node_copy(gds_dlist_node_t *node)
{
	gds_dlist_node_t *head, *tmp, *tmp2, *n;
	gds_inline_dlist_node_t *i, *ti, *t2i;

	GDS_CHECK_ARG_NOT_NULL(node);

	head = gds_dlist_node_new(node->data);
	tmp = head;
	ti = gds_dlist_node_get_inline(tmp);
	tmp2 = node;
	t2i = gds_dlist_node_get_inline(tmp2);
	while ( (t2i = gds_inline_dlist_node_get_next(t2i)) != NULL ) {
		tmp2 = gds_dlist_node_get_container_of(t2i);
		n = gds_dlist_node_new(tmp2->data);
		i = gds_dlist_node_get_inline(n);
		gds_inline_dlist_node_next(ti, i);
		gds_inline_dlist_node_prev(i, ti);

		ti = gds_inline_dlist_node_get_next(ti);
	}

	return head;
}

void gds_dlist_node_free(gds_dlist_node_t *node, void *callback,
	void *callback_data)
{
	void (*cb)(void *, void *) = callback;

	if (cb != NULL && node != NULL) {
		cb(node->data, callback_data);
	}
	free(node);
}
