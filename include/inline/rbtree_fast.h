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
#include "../iterator.h"

struct gds_inline_rbtree_fast_node_s {
	_Bool red;
	struct gds_inline_rbtree_fast_node_s *parent;
	struct gds_inline_rbtree_fast_node_s *left;
	struct gds_inline_rbtree_fast_node_s *right;
};

typedef struct gds_inline_rbtree_fast_node_s gds_inline_rbtree_fast_node_t;

/* Initialize red-black tree inline node with default values */
void
gds_inline_rbtree_fast_node_init(
	gds_inline_rbtree_fast_node_t *node
);

/* Insert a node in red-black tree.
 *
 * The node is not inserted if an equivalent node is already in the tree
 * (ie. rbtf_cmp_cb returns 0 at some point).
 *
 * Parameters:
 *   root     : root node of tree
 *   node     : node to insert
 *   cmp_cb   : cmp callback
 *              Prototype: int cmp_cb(gds_inline_rbtree_fast_node_t *node1, gds_inline_rbtree_fast_node_t *node2, void *userdata)
 *              It should compare two nodes and return:
 *              - a negative value if node1 should be at left of node2
 *              - 0 if node1 is equal to node2
 *              - a positive value if node1 should be at right of node2
 *   cmp_data : user data passed to cmp_cb
 *
 * Returns:
 *   0 if data was successfully inserted
 *   1 if data was already in tree
 *
 * NOTE: root will be modified so it will always point to the root of tree after
 * the function call. You should consider this when using this function.
 */
int
gds_inline_rbtree_fast_add(
	gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node,
	void *cmp_cb,
	void *cmp_data
);

/* Insert a node in red-black tree.
 *
 * Unlike gds_inline_rbtree_fast_add, this function forces insertion even if
 * an equivalent node is already in the tree. The latter node is then removed
 * from tree.
 *
 * Parameters:
 *   root     : root node of tree
 *   node     : node to insert
 *   cmp_cb   : cmp callback
 *              Prototype: int cmp_cb(gds_inline_rbtree_fast_node_t *node1, gds_inline_rbtree_fast_node_t *node2, void *userdata)
 *              It should compare two nodes and return:
 *              - a negative value if node1 should be at left of node2
 *              - 0 if node1 is equal to node2
 *              - a positive value if node1 should be at right of node2
 *   cmp_data : user data passed to cmp_cb
 *   removed  : if not NULL, *removed will contain the address of removed
 *              node, if any.
 *
 * Returns:
 *   0 if node was inserted in the tree
 *   1 if the node is already in the tree (no replacement done)
 *   2 if node has replaced another node in the tree (*removed will be set)
 *
 * NOTE: root will be modified so it will always point to the root of tree after
 * the function call. You should consider this when using this function.
 */
int
gds_inline_rbtree_fast_set(
	gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node,
	void *cmp_cb,
	void *cmp_data,
	gds_inline_rbtree_fast_node_t **removed
);

/* Search a node by key.
 *
 * Parameters:
 *   root              : root node of tree
 *   key               : key to search
 *   cmp_with_key_cb   : cmp_with_key callback
 *                       Prototype: int cmp_with_key_cb(gds_inline_rbtree_fast_node_t *node, void *key, void *userdata)
 *                       It should compare node against key and return:
 *                       - a negative value if search should continue in left
 *                         sub-tree of node,
 *                       - zero if key corresponds to the node,
 *                       - a positive value if search should continue in right
 *                         sub-tree of node.
 *   cmp_with_key_data : user data passed to cmp_with_key_cb
 *
 * Returns:
 *   Pointer to found node, or NULL if key was not found.
 */
gds_inline_rbtree_fast_node_t *
gds_inline_rbtree_fast_get_node(
	gds_inline_rbtree_fast_node_t *root,
	const void *key,
	void *cmp_with_key_cb,
	void *cmp_with_key_data
);

/* Remove a node from red-black tree by its key.
 *
 * Parameters:
 *   root              : root node of tree
 *   key               : key of data to remove
 *   cmp_with_key_cb   : cmp_with_key callback
 *                       Prototype: int cmp_with_key_cb(gds_inline_rbtree_fast_node_t *node, void *key, void *userdata)
 *                       It should compare node against key and return:
 *                       - a negative value if search should continue in left
 *                         sub-tree of node,
 *                       - zero if key corresponds to the node,
 *                       - a positive value if search should continue in right
 *                         sub-tree of node.
 *   cmp_with_key_data : user data passed to cmp_with_key_cb
 *
 * Returns:
 *   Pointer to the removed node, or NULL if node was not found.
 *
 * NOTE: root will be modified so it will always point to the root of tree after
 * the function call. You should consider this when using this function.
 */
gds_inline_rbtree_fast_node_t *
gds_inline_rbtree_fast_del(
	gds_inline_rbtree_fast_node_t **root,
	const void *key,
	void *cmp_with_key_cb,
	void *cmp_with_key_data
);

/* Create an iterator on red-black tree.
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

