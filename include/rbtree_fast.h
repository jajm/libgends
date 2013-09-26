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

#ifndef rbtree_fast_h_included
#define rbtree_fast_h_included

#include <stdint.h>
#include "callbacks.h"
#include "inline/rbtree_fast.h"

typedef struct {
	void *key;
	void *data;
	gds_inline_rbtree_fast_node_t rbtree;
} gds_rbtree_fast_node_t;

/* Insert data into the tree.
 *
 * Parameters
 *   root      : pointer to the root node pointer
 *   key       : pointer to key
 *   data      : pointer to the data
 *   cmpkey_cb : function that takes two keys and compare them.
 *               Should return 0 if keys are the same, a negative number if
 *               first key is lesser than second key, and a positive number
 *               otherwise.
 * Returns
 *   0 on success
 *   a negative value on failure
 *   a positive value if key of data is already in the tree: nothing was done
 */
int
gds_rbtree_fast_add(
	gds_rbtree_fast_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb
);

/* Insert or replace data into the tree.
 *
 * Parameters
 *   root      : pointer to the root node pointer
 *   key       : pointer to key
 *   data      : pointer to data
 *   cmpkey_cb : callback that should takes two keys and compare them.
 *   free_cb   : callback that should free data.
 */
void
gds_rbtree_fast_set(
	gds_rbtree_fast_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

/* Retrieve data from the tree.
 *
 * Parameters
 *   root      : pointer to the root node
 *   key       : pointer to key
 *   cmpkey_cb : callback that should takes two keys and compare them
 *
 * Returns
 *   pointer to the data
 *   or NULL if key doesn't exist in the tree
 */
void *
gds_rbtree_fast_get(
	gds_rbtree_fast_node_t *root,
	void *key,
	gds_cmpkey_cb cmpkey_cb
);

/* Remove a node from the tree.
 *
 * Parameters
 *   root        : pointer to the root node pointer
 *   key         : pointer to key
 *   cmpkey_cp   : callback that should takes two keys and compare them
 *   key_free_cb : callback that should free key
 *   free_cb     : callback that should free data
 *
 * Returns
 *   0 if node was successfully deleted.
 *   1 if node was not found.
 */
int
gds_rbtree_fast_del(
	gds_rbtree_fast_node_t **root,
	void *key,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

/* Destroy the entire tree.
 *
 * Parameters
 *   root        : pointer to the root node
 *   key_free_cb : callback that should free key
 *   free_cb     : callback that should free data
 */
void
gds_rbtree_fast_free(
	gds_rbtree_fast_node_t *root,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

/* Create a new iterator.
 *
 * Parameters
 *   root : pointer to the root node
 *
 * Returns
 *   Pointer to the new iterator.
 */
gds_iterator_t *
gds_rbtree_fast_iterator_new(
	gds_rbtree_fast_node_t *root
);

#endif /* Not rbtree_fast_h_included */

