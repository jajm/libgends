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
#include "rbtree_keyin_node.h"
#include "callbacks.h"
#include "slist.h"

int
gds_rbtree_keyin_add(
	gds_rbtree_keyin_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb
);

int
gds_rbtree_keyin_set(
	gds_rbtree_keyin_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

void *
gds_rbtree_keyin_get(
	gds_rbtree_keyin_node_t *root,
	const void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb
);

int
gds_rbtree_keyin_del(
	gds_rbtree_keyin_node_t **root,
	const void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

void
gds_rbtree_keyin_free(
	gds_rbtree_keyin_node_t *root,
	gds_free_cb free_cb
);

gds_slist_t *
gds_rbtree_keyin_values(
	gds_rbtree_keyin_node_t *root
);

#endif /* Not defined rbtree_keyin_h_included */

