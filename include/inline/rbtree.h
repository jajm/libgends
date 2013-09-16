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

#ifndef gds_inline_rbtree_h_included
#define gds_inline_rbtree_h_included

#include <stdbool.h>
#include <stdint.h>
#include "iterator.h"

struct gds_inline_rbtree_node_s {
	_Bool red;
	struct gds_inline_rbtree_node_s *son[2];
};
typedef struct gds_inline_rbtree_node_s gds_inline_rbtree_node_t;

typedef int32_t (*gds_rbt_cmp_cb)(gds_inline_rbtree_node_t *,
	gds_inline_rbtree_node_t *, const void *);
typedef int32_t (*gds_rbt_cmp_with_key_cb)(gds_inline_rbtree_node_t *,
	const void *, const void *);
typedef void (*gds_rbt_replace_cb)(gds_inline_rbtree_node_t *,
	gds_inline_rbtree_node_t *, void *);

void
gds_inline_rbtree_node_init(
	gds_inline_rbtree_node_t *node
);

int8_t
gds_inline_rbtree_add(
	gds_inline_rbtree_node_t **root,
	gds_inline_rbtree_node_t *node,
	gds_rbt_cmp_cb rbt_cmp_cb,
	void *rbt_cmp_data
);

gds_inline_rbtree_node_t *
gds_inline_rbtree_get_node(
	gds_inline_rbtree_node_t *root,
	const void *key,
	gds_rbt_cmp_with_key_cb rbt_cmp_with_key_cb,
	void *rbt_cmp_data
);

int8_t
gds_inline_rbtree_del(
	gds_inline_rbtree_node_t **root,
	const void *key,
	gds_rbt_cmp_with_key_cb rbt_cmp_with_key_cb,
	void *rbt_cmp_data,
	gds_rbt_replace_cb rbt_replace_cb,
	void *rbt_free_data
);

gds_iterator_t *
gds_inline_rbtree_iterator_new(
	gds_inline_rbtree_node_t *root
);

#endif /* Not gds_inline_rbtree_h_included */

