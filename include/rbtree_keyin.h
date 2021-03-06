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

#ifndef rbtree_keyin_h_included
#define rbtree_keyin_h_included

#include "slist.h"
#include "inline/rbtree.h"
#include "iterator.h"

typedef struct {
	void *data;
	gds_inline_rbtree_node_t rbtree;
} gds_rbtree_keyin_node_t;

/* Insert data into the tree.
 *
 * Parameters
 *   root      : pointer to pointer to the root node
 *   data      : pointer to the data
 *   getkey_cb : getkey callback
 *               Prototype: void * getkey_cb(const void *ptr)
 *               It should return the key of object referenced by ptr.
 *   cmpkey_cb : cmpkey callback
 *               Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *               It should compare key1 to key2 and returns:
 *               - a negative value if key1 < key2
 *               - 0 if key1 == key2
 *               - a positive value if key1 > key2
 *
 * Returns
 *   0 on success
 *   1 if key of data is already in the tree (nothing done)
 *   a negative value on failure
 */
int
gds_rbtree_keyin_add(
	gds_rbtree_keyin_node_t **root,
	void *data,
	void *getkey_cb,
	void *cmpkey_cb
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
 *   getkey_cb : getkey callback
 *               Prototype: void * getkey_cb(const void *ptr)
 *               It should return the key of object referenced by ptr.
 *   cmpkey_cb : cmpkey callback
 *               Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *               It should compare key1 to key2 and returns:
 *               - a negative value if key1 < key2
 *               - 0 if key1 == key2
 *               - a positive value if key1 > key2
 *   free_cb   : free callback
 *               Prototype: void free_cb(void *ptr)
 *               It should free memory used by object referenced by ptr
 *
 * Returns
 *   0 if key was not in the tree (node added)
 *   1 if key was already in the tree (node replaced)
 *   a negative value on failure
 */
int
gds_rbtree_keyin_set(
	gds_rbtree_keyin_node_t **root,
	void *data,
	void *getkey_cb,
	void *cmpkey_cb,
	void *free_cb
);

/* Get data of a node in red-black tree.
 *
 * Parameters
 *   root      : Pointer to root node.
 *   key       : Key of node to retrieve.
 *   getkey_cb : getkey callback
 *               Prototype: void * getkey_cb(const void *ptr)
 *               It should return the key of object referenced by ptr.
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
gds_rbtree_keyin_get(
	gds_rbtree_keyin_node_t *root,
	const void *key,
	void *getkey_cb,
	void *cmpkey_cb
);

/* Remove a node from a red-black tree.
 *
 * Parameters
 *   root      : Pointer to pointer to root node. Will be updated to point to
 *               root node, if root changes.
 *   key       : Key of node to remove
 *   getkey_cb : getkey callback
 *               Prototype: void * getkey_cb(const void *ptr)
 *               It should return the key of object referenced by ptr.
 *   cmpkey_cb : cmpkey callback
 *               Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *               It should compare key1 to key2 and returns:
 *               - a negative value if key1 < key2
 *               - 0 if key1 == key2
 *               - a positive value if key1 > key2
 *   free_cb   : free callback
 *               Prototype: void free_cb(void *ptr)
 *               It should free memory used by object referenced by ptr
 *
 * Returns
 *   0 if node was successfully deleted.
 *   1 if key was not in tree.
 *   a negative value on failure.
 */
int
gds_rbtree_keyin_del(
	gds_rbtree_keyin_node_t **root,
	const void *key,
	void *getkey_cb,
	void *cmpkey_cb,
	void *free_cb
);

/* Remove a node from a red-black tree and return its data.
 *
 * Parameters
 *   root      : Pointer to pointer to root node. Will be updated to point to
 *               root node, if root changes.
 *   key       : Key of node to remove
 *   getkey_cb : getkey callback
 *               Prototype: void * getkey_cb(const void *ptr)
 *               It should return the key of object referenced by ptr.
 *   cmpkey_cb : cmpkey callback
 *               Prototype: int cmpkey_cb(const void *key1, const void *key2)
 *               It should compare key1 to key2 and returns:
 *               - a negative value if key1 < key2
 *               - 0 if key1 == key2
 *               - a positive value if key1 > key2
 *
 * Returns
 *   pointer to the data
 *   NULL on failure
 */
void *
gds_rbtree_keyin_pop(
	gds_rbtree_keyin_node_t **root,
	const void *key,
	void *getkey_cb,
	void *cmpkey_cb
);

/* Destroy a red-black-tree.
 *
 * Parameters
 *   root    : Pointer to root node.
 *   free_cb : free callback
 *             Prototype: void free_cb(void *ptr)
 *             It should free memory used by object referenced by ptr
 */
void
gds_rbtree_keyin_free(
	gds_rbtree_keyin_node_t *root,
	void *free_cb
);

/* Create an iterator on a red-black tree.
 *
 * Parameters
 *   root : Pointer to root node.
 *   getkey_cb : getkey callback
 *               Prototype: void * getkey_cb(const void *ptr)
 *               It should return the key of object referenced by ptr.
 *
 * Returns
 *   Pointer to iterator.
 *   NULL on failure.
 */
gds_iterator_t *
gds_rbtree_keyin_iterator_new(
	gds_rbtree_keyin_node_t *root,
	void *getkey_cb
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

