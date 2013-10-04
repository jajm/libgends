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

#ifndef gds_rbtree_h_included
#define gds_rbtree_h_included

#include <stdint.h>
#include "inline/rbtree.h"
#include "callbacks.h"
#include "slist.h"

typedef struct {
	void *key;
	void *data;
	gds_inline_rbtree_node_t rbtree;
} gds_rbtree_node_t;

/* Add a node in red-black tree
 *
 * Parameters
 *   root      : Pointer to pointer to root node. Will be updated to point to
 *               the root node, if the root changes.
 *   key       : Node's key.
 *   data      : Node's data.
 *   cmpkey_cb : Callback function to compare two keys.
 *
 * Returns
 *   0 if node was successfully inserted.
 *   1 if key was already in the tree.
 */
int
gds_rbtree_add(
	gds_rbtree_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb
);

/* Set data of a node in red-black tree.
 *
 * If key is not in the tree, a new node is added. Otherwise the existing node
 * is replaced and memory is free using the callbacks key_free_cb and free_cb.
 *
 * Parameters
 *   root        : Pointer to pointer to root node. Will be updated to point to
 *                 the root node, if the root changes.
 *   key         : Node's key
 *   data        : Node's data
 *   cmpkey_cb   : Callback function to compare two keys.
 *   key_free_cb : Callback function to free memory used by key.
 *   free_cb     : Callback function to free memory used by data.
 *
 * Returns
 *   0 if key was not in the tree (node added)
 *   1 if key was already in the tree (node replaced)
 */
int
gds_rbtree_set(
	gds_rbtree_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

/* Get data of a node in red-black tree.
 *
 * Parameters
 *   root      : Pointer to root node.
 *   key       : Key of node to retrieve.
 *   cmpkey_cb : Callback function to compare two keys.
 *
 * Returns
 *   Pointer to data.
 *   NULL if key is not in tree.
 */
void *
gds_rbtree_get(
	gds_rbtree_node_t *root,
	const void *key,
	gds_cmpkey_cb cmpkey_cb
);

/* Remove a node from a red-black tree.
 *
 * Parameters
 *   root        : Pointer to pointer to root node. Will be updated to point to
 *                 root node, if root changes.
 *   key         : Key of node to remove
 *   cmpkey_cb   : Callback function to compare two keys.
 *   key_free_cb : Callback function to free memory used by key.
 *   free_cb     : Callback function to free memory used by data.
 *
 * Returns
 *   0 if node was successfully deleted.
 *   1 if key was not in tree.
 */
int
gds_rbtree_del(
	gds_rbtree_node_t **root,
	const void *key,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

/* Destroy a red-black-tree.
 *
 * Parameters
 *   root        : Pointer to root node.
 *   key_free_cb : Callback function to free memory used by key.
 *   free_cb     : Callback function to free memory used by data.
 */
void
gds_rbtree_free(
	gds_rbtree_node_t *root,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

/* Create an iterator on a red-black tree.
 *
 * Parameters
 *   root : Pointer to root node.
 *
 * Returns
 *   Pointer to iterator.
 */
gds_iterator_t *
gds_rbtree_iterator_new(
	gds_rbtree_node_t *root
);

/* Create a list of keys that are in red-black tree.
 *
 * Parameters
 *   root : Pointer to root node.
 *
 * Returns
 *   Pointer to list.
 */
gds_slist_t *
gds_rbtree_keys(
	gds_rbtree_node_t *root
);

/* Create a list of values that are in red-black tree.
 *
 * Parameters
 *   root : Pointer to root node.
 *
 * Returns
 *   Pointer to list.
 */
gds_slist_t *
gds_rbtree_values(
	gds_rbtree_node_t *root
);

#endif /* Not defined gds_rbtree_h_included */

