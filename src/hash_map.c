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
 * File              : hash_map.c                                            *
 * Short description : Low-level hash map management                         *
 *****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "slist.h"
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree.h"
#include "hash_map.h"
#include "key_value.h"

gds_hash_map_t * gds_hash_map_new(uint32_t size, gds_hash_cb hash_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	gds_hash_map_t *h;

	GDS_CHECK_ARG_NOT_ZERO(size);
	GDS_CHECK_ARG_NOT_NULL(hash_cb)
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb)

	h = malloc(sizeof(gds_hash_map_t));
	if (h == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_hash_map_t));
	}

	h->map = calloc(size, sizeof(gds_rbtree_node_t *));
	if (h->map == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_node_t *));
	}

	h->size = size;
	h->hash_cb = hash_cb;
	h->cmpkey_cb = cmpkey_cb;

	return h;
}

uint32_t gds_hash_map_hash(gds_hash_map_t *h, void *key)
{
	return h->hash_cb(key, h->size) % h->size;
}

int8_t gds_hash_map_set(gds_hash_map_t *h, void *key, void *data,
	gds_free_cb free_cb)
{
	uint32_t hash;

	GDS_CHECK_ARG_NOT_NULL(h);

	hash = gds_hash_map_hash(h, key);
	return gds_rbtree_set(&(h->map[hash]), key, data, h->cmpkey_cb,
		free_cb);
}

void * gds_hash_map_get(gds_hash_map_t *h, void *key)
{
	uint32_t hash;
	void *data;

	GDS_CHECK_ARG_NOT_NULL(h);

	hash = gds_hash_map_hash(h, key);
	data = gds_rbtree_get(h->map[hash], key, h->cmpkey_cb);
	return data;
}

int8_t gds_hash_map_unset(gds_hash_map_t *h, void *key,
	gds_free_cb key_free_cb, gds_free_cb free_cb)
{
	uint32_t hash;
	int8_t rv;

	GDS_CHECK_ARG_NOT_NULL(h);

	hash = gds_hash_map_hash(h, key);
	rv = gds_rbtree_del(&(h->map[hash]), key, h->cmpkey_cb,
		key_free_cb, free_cb);
	
	return rv;
}

gds_slist_t * gds_hash_map_keys(gds_hash_map_t *h)
{
	gds_slist_t *l = gds_slist_new();
	gds_slist_t *list;
	uint32_t i;

	GDS_CHECK_ARG_NOT_NULL(h);

	for (i = h->size; i > 0; i--) {
		list = gds_rbtree_keys(h->map[i-1]);
		if (list != NULL) {
			gds_slist_unshift(l, list);
		}
	}

	return l;
}

gds_slist_t * gds_hash_map_values(gds_hash_map_t *h)
{
	gds_slist_t *l = gds_slist_new();
	gds_slist_t *list;
	uint32_t i;

	GDS_CHECK_ARG_NOT_NULL(h);

	for (i = h->size; i > 0; i--) {
		list = gds_rbtree_values(h->map[i-1]);
		if (list != NULL) {
			gds_slist_unshift(l, list);
		}
	}

	return l;
}

gds_slist_t * gds_hash_map_keys_values(gds_hash_map_t *h)
{
	gds_slist_t *l = NULL;
	gds_slist_t *list;
	uint32_t i;

	GDS_CHECK_ARG_NOT_NULL(h);

	for (i = h->size; i > 0; i--) {
		list = gds_rbtree_keys_values(h->map[i-1]);
		if (list != NULL) {
			gds_slist_unshift(l, list);
		}
	}

	return l;
}

gds_rbtree_node_t ** gds_hash_map_build_map(gds_hash_map_t *h, uint32_t size)
{
	gds_rbtree_node_t **map;
	gds_slist_t *l;
	gds_iterator_t *it;
	gds_key_value_t *kv;
	uint32_t hash;

	map = calloc(size, sizeof(gds_rbtree_node_t *));
	if (map == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_node_t *));
	}
	l = gds_hash_map_keys_values(h);
	it = gds_slist_iterator_new(l);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it)) {
		kv = gds_iterator_get(it);
		hash = h->hash_cb(kv->key, size) % size;
		gds_rbtree_add(&(map[hash]), kv->key,
			kv->value, h->cmpkey_cb);
	}
	gds_iterator_free(it);
	gds_slist_free(l, free, NULL);

	return map;
}

void gds_hash_map_change_size(gds_hash_map_t *h, uint32_t new_size)
{
	gds_rbtree_node_t **map;

	GDS_CHECK_ARG_NOT_NULL(h);
	GDS_CHECK_ARG_NOT_ZERO(new_size);

	map = gds_hash_map_build_map(h, new_size);
	for (uint32_t i = 0; i < h->size; i++) {
		gds_rbtree_free(h->map[i], NULL, NULL);
	}
	free(h->map);
	h->map = map;
	h->size = new_size;
}

void gds_hash_map_free(gds_hash_map_t *h, gds_free_cb key_free_cb,
	gds_free_cb free_cb)
{
	uint32_t i;
	if (h != NULL) {
		for (i=0; i<h->size; i++) {
			gds_rbtree_free(h->map[i], key_free_cb,
				free_cb);
		}
		free(h->map);
		free(h);
	}
}
