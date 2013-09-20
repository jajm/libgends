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

#ifndef gds_inline_rbtree_fast_h_included
#define gds_inline_rbtree_fast_h_included

#include <stdbool.h>
#include <stdint.h>
#include "callbacks.h"
#include "iterator.h"

struct gds_inline_rbtree_fast_node_s {
	_Bool red;
	struct gds_inline_rbtree_fast_node_s *parent;
	struct gds_inline_rbtree_fast_node_s *left;
	struct gds_inline_rbtree_fast_node_s *right;
};

typedef struct gds_inline_rbtree_fast_node_s gds_inline_rbtree_fast_node_t;

/* Compare two red-black tree nodes.
 *
 * Should returns:
 *   -1 if node1 should be at left of node2
 *   0 if node1 is equal to node2
 *   1 if node1 should be at right of node2
 */
typedef int (*gds_rbtf_cmp_cb)(
	gds_inline_rbtree_fast_node_t *,    // node1
	gds_inline_rbtree_fast_node_t *,    // node2
	void *     // user data
);

/* Compare a key and a red-black tree node */
/* Used during search (for retrieving and deleting data) to determine where to
 * search for data. */
/* Must return a negative value if search should continue in left sub-tree of
 * node, zero if key corresponds to the node, and a positive value if search
 * should continue in right sub-tree of node. */
typedef int32_t (*gds_rbtf_cmp_key_cb)(
	void *,                             // key
	gds_inline_rbtree_fast_node_t *,    // red-black tree node
	void *                              // user data
);

/* Initialize red-black tree inline node with default values */
void
gds_inline_rbtree_fast_node_init(
	gds_inline_rbtree_fast_node_t *node
);

/* Insert data */
/* root                  : root node of tree
 * node                  : node to insert
 * rbtf_cmp_cb           : see above documentation about gds_rbtf_cmp_cb
 * rbtf_cmp_data         : user data passed to rbtf_cmp_cb */
/* Return: 0 if data was successfully inserted
 *         1 if data was already in tree */
/* NOTE: root will be modified so it will always point to the root of tree after
 * the function call. You should consider this when using this function. */
int8_t
gds_inline_rbtree_fast_add(
	gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node,
	gds_rbtf_cmp_cb rbtf_cmp_cb,
	void *rbtf_cmp_data
);

/* Insert data or change it if it is already in the tree */
/* root                  : root node of tree
 * node                  : node to insert
 * rbtf_cmp_cb           : see above documentation about gds_rbtf_cmp_cb
 * rbtf_cmp_data         : user data passed to rbtf_cmp_cb */
/* NOTE: root will be modified so it will always point to the root of tree after
 * the function call. You should consider this when using this function. */
gds_inline_rbtree_fast_node_t *
gds_inline_rbtree_fast_set(
	gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node,
	gds_rbtf_cmp_cb rbtf_cmp_cb,
	void *rbtf_cmp_data
);

/* Search a node by key */
/* root              : root node of tree
 * key               : key to search
 * rbtf_cmp_key_cb   : see above documentation about gds_rbtf_cmp_key_cb
 * rbtf_cmp_key_data : user data passed to rbtf_cmp_key_cb */
/* Return pointer to found node, or NULL if key was not found. */
gds_inline_rbtree_fast_node_t *
gds_inline_rbtree_fast_get_node(
	gds_inline_rbtree_fast_node_t *root,
	void *key,
	gds_rbtf_cmp_key_cb rbtf_cmp_key_cb,
	void *rbtf_cmp_key_data
);

/* Remove data by its key */
/* root              : root node of tree
 * key               : key of data to remove
 * rbtf_cmp_key_cb   : see above documentation about gds_rbtf_cmp_key_cb
 * rbtf_cmp_key_data : user data passed to rbtf_cmp_key_cb */
/* Return: pointer to the removed node, or NULL if node was not found. */
/* NOTE: root will be modified so it will always point to the root of tree after
 * the function call. You should consider this when using this function. */
gds_inline_rbtree_fast_node_t *
gds_inline_rbtree_fast_del(
	gds_inline_rbtree_fast_node_t **root,
	void *key,
	gds_rbtf_cmp_key_cb rbtf_cmp_key_cb,
	void *rbtf_cmp_key_data
);

/* Create an iterator on tree
 *
 * Parameters:
 *   root: root node of tree
 *
 * Returns:
 *   Pointer to the new iterator. Free with gds_iterator_free.
 */
gds_iterator_t *
gds_inline_rbtree_fast_iterator_new(
	gds_inline_rbtree_fast_node_t *root
);

#endif /* Not gds_inline_rbtree_fast_h_included */

