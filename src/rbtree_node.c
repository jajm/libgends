/*
 * Copyright 2012-2013 Julian Maurice
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
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree_node.h"
#include "callbacks.h"

gds_rbtree_node_t * gds_rbtree_node_new(void *key, void *data)
{
	gds_rbtree_node_t *node;
	
	node = malloc(sizeof(gds_rbtree_node_t));

	if(node == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_node_t));
	}

	node->key = key;
	node->data = data;

	gds_inline_rbtree_node_init(&(node->rbtree));

	return node;
}

void * gds_rbtree_node_get_key(gds_rbtree_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->key;
}

void gds_rbtree_node_set_key(gds_rbtree_node_t *node, void *key,
	gds_free_cb key_free_cb)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	if (key_free_cb != NULL) {
		key_free_cb(node->key);
	}
	node->key = key;
}

void * gds_rbtree_node_get_data(gds_rbtree_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->data;
}

void gds_rbtree_node_set_data(gds_rbtree_node_t *node, void *data,
	gds_free_cb free_cb)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	if (free_cb != NULL) {
		free_cb(node->data);
	}
	node->data = data;
}

void gds_rbtree_node_free(gds_rbtree_node_t *node,
	gds_free_cb key_free_cb, gds_free_cb free_cb)
{
	if(node) {
		if (key_free_cb != NULL) {
			key_free_cb(node->key);
		}
		if (free_cb != NULL) {
			free_cb(node->data);
		}
		free(node);
	}
}
