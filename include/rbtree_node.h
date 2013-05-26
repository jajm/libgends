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

#ifndef rbtree_node_h_included
#define rbtree_node_h_included

#include <stdint.h>
#include "callbacks.h"
#include "inline/rbtree.h"

struct gds_rbtree_node_s {
	void *key;
	void *data;
	gds_inline_rbtree_node_t rbtree;
};
typedef struct gds_rbtree_node_s gds_rbtree_node_t;

gds_rbtree_node_t *
gds_rbtree_node_new(
	void *key,
	void *data
);

void *
gds_rbtree_node_get_key(
	gds_rbtree_node_t *node
);

void
gds_rbtree_node_set_key(
	gds_rbtree_node_t *node,
	void *key,
	gds_free_cb key_free_cb
);

void *
gds_rbtree_node_get_data(
	gds_rbtree_node_t *node
);

void
gds_rbtree_node_set_data(
	gds_rbtree_node_t *node,
	void *data,
	gds_free_cb free_cb
);

void
gds_rbtree_node_free(
	gds_rbtree_node_t *node,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

#endif /* Not rbtree_node_h_included */

