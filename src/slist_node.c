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
 * File              : slist_node.c
 * Short description : Single linked list node
 */

#include <stdlib.h>
#include <stdint.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "slist_node.h"
#include "callbacks.h"
#include "container_of.h"

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
