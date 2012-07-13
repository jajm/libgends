/*****************************************************************************
 * Copyright 2012 Julian Maurice                                             *
 *                                                                           *
 * This file is part of libgends.                                            *
 *                                                                           *
 * libgends is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * libgends is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.         *
 *****************************************************************************/

/*****************************************************************************
 * File              : ll_hash_map.c                                         *
 * Short description : Low-level hash map management                         *
 *****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "ll_slist.h"
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "ll_hash_map.h"

gds_ll_hash_map_t * gds_ll_hash_map_new(uint32_t size)
{
	gds_ll_hash_map_t *h;

	GDS_CHECK_ARG_NOT_ZERO(size);

	h = malloc(sizeof(gds_ll_hash_map_t));
	if (h == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_ll_hash_map_t));
	}

	h->map = calloc(size, sizeof(gds_compact_rbtree_node_t *));
	if (h->map == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_compact_rbtree_node_t *));
	}

	h->size = size;

	return h;
}

uint32_t gds_ll_hash_map_hash(gds_ll_hash_map_t *h, gds_hash_cb hash_cb,
	void *key)
{
	return hash_cb(key, h->size) % h->size;
}

int8_t gds_ll_hash_map_set(gds_ll_hash_map_t *h, gds_hash_cb hash_cb,
	void *data, gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb, gds_alloc_cb alloc_cb)
{
	uint32_t hash;

	GDS_CHECK_ARG_NOT_NULL(h);
	GDS_CHECK_ARG_NOT_NULL(hash_cb);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);

	hash = gds_ll_hash_map_hash(h, hash_cb, getkey_cb(data));
	return gds_compact_rbtree_set(&(h->map[hash]), data, getkey_cb,
		cmpkey_cb, free_cb, alloc_cb);
}

int8_t gds_ll_hash_map_unset(gds_ll_hash_map_t *h, gds_hash_cb hash_cb,
	void *key, gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb)
{
	uint32_t hash;
	int8_t rv;

	GDS_CHECK_ARG_NOT_NULL(h);
	GDS_CHECK_ARG_NOT_NULL(hash_cb);

	hash = gds_ll_hash_map_hash(h, hash_cb, key);
	rv = gds_compact_rbtree_del(&(h->map[hash]), key, getkey_cb,
		cmpkey_cb, free_cb);
	
	return rv;
}

gds_slist_node_t * gds_ll_hash_map_values(gds_ll_hash_map_t *h,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *l = NULL;

	GDS_CHECK_ARG_NOT_NULL(h);

	for (uint32_t i = 0; i < h->size; i++) {
		gds_slist_node_t *list = gds_compact_rbtree_values(h->map[i],
			alloc_cb);
		if (list != NULL) {
			gds_ll_slist_add_list_first(&l, list);
		}
	}

	return l;
}

gds_compact_rbtree_node_t ** gds_ll_hash_map_build_map(gds_ll_hash_map_t *h,
	uint32_t size, gds_hash_cb hash_cb, gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	gds_compact_rbtree_node_t **map;
	gds_slist_node_t *l;
	gds_iterator_t *it;
	void *data;
	uint32_t hash;

	map = calloc(size, sizeof(gds_compact_rbtree_node_t *));
	if (map == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_compact_rbtree_node_t *));
	}
	l = gds_ll_hash_map_values(h, NULL);
	it = gds_ll_slist_iterator_new(l);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it)) {
		data = gds_iterator_get(it);
		hash = hash_cb(getkey_cb(data), size) % size;
		gds_compact_rbtree_add(&(map[hash]), data, getkey_cb,
			cmpkey_cb, NULL);
	}
	gds_ll_slist_iterator_free(it);

	return map;
}

void gds_ll_hash_map_change_size(gds_ll_hash_map_t *h, uint32_t new_size,
	gds_hash_cb hash_cb, gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb)
{
	gds_compact_rbtree_node_t **map;

	GDS_CHECK_ARG_NOT_NULL(h);
	GDS_CHECK_ARG_NOT_ZERO(new_size);

	map = gds_ll_hash_map_build_map(h, new_size, hash_cb, getkey_cb,
		cmpkey_cb);
	for (uint32_t i = 0; i < h->size; i++) {
		gds_compact_rbtree_free(h->map[i], NULL);
	}
	free(h->map);
	h->map = map;
	h->size = new_size;
}

void gds_ll_hash_map_free(gds_ll_hash_map_t *h, gds_free_cb free_cb)
{
	uint32_t i;
	if (h != NULL) {
		for (i=0; i<h->size; i++) {
			gds_compact_rbtree_free(h->map[i], free_cb);
		}
		free(h->map);
		free(h);
	}
}
