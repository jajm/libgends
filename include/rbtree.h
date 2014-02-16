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

#ifndef gds_rbtree_h_included
#define gds_rbtree_h_included

#include "inline/rbtree.h"
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
 *   cmpkey_cb : cmpkey callback
 *               Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *               It should compare key1 to key2 and returns:
 *               - a negative value if key1 < key2
 *               - 0 if key1 == key2
 *               - a positive value if key1 > key2
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
	void *cmpkey_cb
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
 *   0 if key was not in the tree (node added)
 *   1 if key was already in the tree (node replaced)
 */
int
gds_rbtree_set(
	gds_rbtree_node_t **root,
	void *key,
	void *data,
	void *cmpkey_cb,
	void *key_free_cb,
	void *free_cb
);

/* Get data of a node in red-black tree.
 *
 * Parameters
 *   root      : Pointer to root node.
 *   key       : Key of node to retrieve.
 *   cmpkey_cb : cmpkey callback
 *               Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *               It should compare key1 to key2 and returns:
 *               - a negative value if key1 < key2
 *               - 0 if key1 == key2
 *               - a positive value if key1 > key2
 *
 * Returns
 *   Pointer to data.
 *   NULL if key is not in tree.
 */
void *
gds_rbtree_get(
	gds_rbtree_node_t *root,
	const void *key,
	void *cmpkey_cb
);

/* Remove a node from a red-black tree.
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
 *   free_cb     : free callback
 *                 Prototype: void free_cb(void *ptr)
 *                 It should free memory used by object referenced by ptr
 *
 * Returns
 *   0 if node was successfully deleted.
 *   1 if key was not in tree.
 */
int
gds_rbtree_del(
	gds_rbtree_node_t **root,
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
gds_rbtree_pop(
	gds_rbtree_node_t **root,
	const void *key,
	void *cmpkey_cb,
	void *key_free_cb
);

/* Destroy a red-black-tree.
 *
 * Parameters
 *   root        : Pointer to root node.
 *   key_free_cb : key_free callback
 *                 Prototype: void key_free_cb(void *key)
 *                 It should free memory used by object referenced by key
 *   free_cb     : free callback
 *                 Prototype: void free_cb(void *ptr)
 *                 It should free memory used by object referenced by ptr
 */
void
gds_rbtree_free(
	gds_rbtree_node_t *root,
	void *key_free_cb,
	void *free_cb
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

