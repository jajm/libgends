/*
 * Copyright 2012-2014 Julian Maurice
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

#include "slist.h"
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
 *   cmpkey_cb : cmpkey callback
 *               Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *               It should compare key1 to key2 and returns:
 *               - a negative value if key1 < key2
 *               - 0 if key1 == key2
 *               - a positive value if key1 > key2
 *
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
	void *cmpkey_cb
);

/* Insert or replace data into the tree.
 *
 * Parameters
 *   root        : pointer to the root node pointer
 *   key         : pointer to key
 *   data        : pointer to data
 *   cmpkey_cb   : cmpkey callback
 *                 Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *                 It should compare key1 to key2 and returns:
 *                 - a negative value if key1 < key2
 *                 - 0 if key1 == key2
 *                 - a positive value if key1 > key2
 *   key_free_cb : key_free callback
 *                 Prototype: void key_free_cb(void *key)
 *                 It should free memory used by object referenced by key
 *   free_cb     : free callback
 *                 Prototype: void free_cb(void *ptr)
 *                 It should free memory used by object referenced by ptr
 *
 * Returns
 *   0 if key was already in the hash map
 *   1 if key was just added
 */
int
gds_rbtree_fast_set(
	gds_rbtree_fast_node_t **root,
	void *key,
	void *data,
	void *cmpkey_cb,
	void *key_free_cb,
	void *free_cb
);

/* Retrieve data from the tree.
 *
 * Parameters
 *   root      : pointer to the root node
 *   key       : pointer to key
 *   cmpkey_cb : cmpkey callback
 *               Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *               It should compare key1 to key2 and returns:
 *               - a negative value if key1 < key2
 *               - 0 if key1 == key2
 *               - a positive value if key1 > key2
 *
 * Returns
 *   pointer to the data
 *   or NULL if key doesn't exist in the tree
 */
void *
gds_rbtree_fast_get(
	gds_rbtree_fast_node_t *root,
	const void *key,
	void *cmpkey_cb
);

/* Remove a node from the tree.
 *
 * Parameters
 *   root        : pointer to the root node pointer
 *   key         : pointer to key
 *   cmpkey_cb   : cmpkey callback
 *                 Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *                 It should compare key1 to key2 and returns:
 *                 - a negative value if key1 < key2
 *                 - 0 if key1 == key2
 *                 - a positive value if key1 > key2
 *   key_free_cb : key_free callback
 *                 Prototype: void key_free_cb(void *key)
 *                 It should free memory used by object referenced by key
 *   free_cb     : free callback
 *                 Prototype: void free_cb(void *ptr)
 *                 It should free memory used by object referenced by ptr
 *
 * Returns
 *   0 if node was successfully deleted.
 *   1 if node was not found.
 */
int
gds_rbtree_fast_del(
	gds_rbtree_fast_node_t **root,
	const void *key,
	void *cmpkey_cb,
	void *key_free_cb,
	void *free_cb
);

/* Remove a node from a red-black tree and return its data.
 *
 * Parameters
 *   root        : Pointer to pointer to root node. Will be updated to point to
 *                 root node, if root changes.
 *   key         : Key of node to remove
 *   cmpkey_cb   : cmpkey callback
 *                 Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *                 It should compare key1 to key2 and returns:
 *                 - a negative value if key1 < key2
 *                 - 0 if key1 == key2
 *                 - a positive value if key1 > key2
 *   key_free_cb : key_free callback
 *                 Prototype: void key_free_cb(void *key)
 *                 It should free memory used by object referenced by key
 *
 * Returns
 *   pointer to the data
 */
void *
gds_rbtree_fast_pop(
	gds_rbtree_fast_node_t **root,
	const void *key,
	void *cmpkey_cb,
	void *key_free_cb
);

/* Destroy the entire tree.
 *
 * Parameters
 *   root        : pointer to the root node
 *   key_free_cb : key_free callback
 *                 Prototype: void key_free_cb(void *key)
 *                 It should free memory used by object referenced by key
 *   free_cb     : free callback
 *                 Prototype: void free_cb(void *ptr)
 *                 It should free memory used by object referenced by ptr
 */
void
gds_rbtree_fast_free(
	gds_rbtree_fast_node_t *root,
	void *key_free_cb,
	void *free_cb
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

/* Create a list of keys that are in red-black tree.
 *
 * Parameters
 *   root : Pointer to root node.
 *
 * Returns
 *   Pointer to list.
 */
gds_slist_t *
gds_rbtree_fast_keys(
	gds_rbtree_fast_node_t *root
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
gds_rbtree_fast_values(
	gds_rbtree_fast_node_t *root
);

#endif /* Not rbtree_fast_h_included */

