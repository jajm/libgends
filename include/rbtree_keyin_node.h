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

#ifndef rbtree_keyin_node_h_included
#define rbtree_keyin_node_h_included

#include <stdint.h>
#include "callbacks.h"
#include "inline/rbtree.h"

struct gds_rbtree_keyin_node_s {
	void *data;
	gds_inline_rbtree_node_t rbtree;
};
typedef struct gds_rbtree_keyin_node_s gds_rbtree_keyin_node_t;

gds_rbtree_keyin_node_t *
gds_rbtree_keyin_node_new(
	void *data
);

void *
gds_rbtree_keyin_node_get_data(
	gds_rbtree_keyin_node_t *node
);

int8_t
gds_rbtree_keyin_node_set_data(
	gds_rbtree_keyin_node_t *node,
	void *data,
	gds_free_cb free_cb
);

void
gds_rbtree_keyin_node_free(
	gds_rbtree_keyin_node_t *node,
	gds_free_cb free_cb
);

#endif /* Not rbtree_keyin_node_h_included */

