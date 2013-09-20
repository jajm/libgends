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

#ifndef rbtree_keyin_fast_h_included
#define rbtree_keyin_fast_h_included

#include <stdint.h>
#include "callbacks.h"
#include "rbtree_keyin_fast_node.h"

/* Insert data into the tree */
/*      root : pointer to pointer to the root node
 *      data : pointer to the data
 * copy_data : true => copy the data using alloc_cb
 *             false => don't copy the data, only the pointer
 *  alloc_cb : function that takes a pointer and return another pointer to a
 *             copy of data
 * getkey_cb : function that takes a pointer and return a key for the pointed
 *             data. Key will be used to determine where the new node will be
 *             inserted
 * cmpkey_cb : function that takes two keys and compare them. Should return 0 if
 *             keys are the same, a negative number if first key is lesser than
 *             second key, and a positive number otherwise. */
/* Return: 0: success
 *         a negative value: failure
 *         a positive value: key of data is already in the tree: nothing done */
int
gds_rbtree_keyin_fast_add(
	gds_rbtree_keyin_fast_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb
);

void
gds_rbtree_keyin_fast_set(
	gds_rbtree_keyin_fast_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

void *
gds_rbtree_keyin_fast_get(
	gds_rbtree_keyin_fast_node_t *root,
	void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb
);

int
gds_rbtree_keyin_fast_del(
	gds_rbtree_keyin_fast_node_t **root,
	void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

void
gds_rbtree_keyin_fast_free(
	gds_rbtree_keyin_fast_node_t *root,
	gds_free_cb free_cb
);

#endif /* Not rbtree_keyin_fast_h_included */

