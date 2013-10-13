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

#ifndef rbtree_keyin_h_included
#define rbtree_keyin_h_included

#include <stdint.h>
#include "callbacks.h"
#include "slist.h"
#include "inline/rbtree.h"

typedef struct {
	void *data;
	gds_inline_rbtree_node_t rbtree;
} gds_rbtree_keyin_node_t;

/* Insert data into the tree.
 *
 * Parameters
 *   root      : pointer to pointer to the root node
 *   data      : pointer to the data
 *   getkey_cb : function that takes a pointer and return a key for the pointed
 *               data. Key will be used to determine where the new node will be
 *               inserted
 *   cmpkey_cb : function that takes two keys and compare them. Should return 0
 *               if keys are the same, a negative number if first key is lesser
 *               than second key, and a positive number otherwise.
 *
 * Returns
 *   0 on success
 *   a positive value if key of data is already in the tree: nothing done
 */
int
gds_rbtree_keyin_add(
	gds_rbtree_keyin_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb
);

/* Set data of a node in red-black tree.
 *
 * If key is not in the tree, a new node is added. Otherwise the existing node
 * is replaced and memory is free using the callback free_cb.
 *
 * Parameters
 *   root      : Pointer to pointer to root node. Will be updated to point to
 *               the root node, if the root changes.
 *   data      : Node's data
 *   getkey_cb : Callback function to get key from data.
 *   cmpkey_cb : Callback function to compare two keys.
 *   free_cb   : Callback function to free memory used by data.
 *
 * Returns
 *   0 if key was not in the tree (node added)
 *   1 if key was already in the tree (node replaced)
 */
int
gds_rbtree_keyin_set(
	gds_rbtree_keyin_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

/* Get data of a node in red-black tree.
 *
 * Parameters
 *   root      : Pointer to root node.
 *   key       : Key of node to retrieve.
 *   getkey_cb : Callback function to get key from data.
 *   cmpkey_cb : Callback function to compare two keys.
 *
 * Returns
 *   Pointer to data.
 *   NULL if key is not in tree.
 */
void *
gds_rbtree_keyin_get(
	gds_rbtree_keyin_node_t *root,
	const void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb
);

/* Remove a node from a red-black tree.
 *
 * Parameters
 *   root      : Pointer to pointer to root node. Will be updated to point to
 *               root node, if root changes.
 *   key       : Key of node to remove
 *   getkey_cb : Callback function to get key from data.
 *   cmpkey_cb : Callback function to compare two keys.
 *   free_cb   : Callback function to free memory used by data.
 *
 * Returns
 *   0 if node was successfully deleted.
 *   1 if key was not in tree.
 */
int
gds_rbtree_keyin_del(
	gds_rbtree_keyin_node_t **root,
	const void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

/* Destroy a red-black-tree.
 *
 * Parameters
 *   root    : Pointer to root node.
 *   free_cb : Callback function to free memory used by data.
 */
void
gds_rbtree_keyin_free(
	gds_rbtree_keyin_node_t *root,
	gds_free_cb free_cb
);

/* Return list of values contained in tree.
 *
 * Parameters
 *   root : Pointer to root node.
 *
 * Returns
 *   List of values
 */
gds_slist_t *
gds_rbtree_keyin_values(
	gds_rbtree_keyin_node_t *root
);

#endif /* Not defined rbtree_keyin_h_included */

