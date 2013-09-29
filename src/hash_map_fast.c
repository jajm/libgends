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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "slist.h"
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree_fast.h"
#include "hash_map_fast.h"

gds_hash_map_fast_t * gds_hash_map_fast_new(uint32_t size, gds_hash_cb hash_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	gds_hash_map_fast_t *h;

	GDS_CHECK_ARG_NOT_ZERO(size);
	GDS_CHECK_ARG_NOT_NULL(hash_cb)
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb)

	h = malloc(sizeof(gds_hash_map_fast_t));
	if (h == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_hash_map_fast_t));
	}

	h->map = calloc(size, sizeof(gds_rbtree_fast_node_t *));
	if (h->map == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_fast_node_t *));
	}

	h->size = size;
	h->hash_cb = hash_cb;
	h->cmpkey_cb = cmpkey_cb;

	return h;
}

uint32_t gds_hash_map_fast_hash(gds_hash_map_fast_t *h, const void *key)
{
	return h->hash_cb(key, h->size) % h->size;
}

int gds_hash_map_fast_set(gds_hash_map_fast_t *h, void *key, void *data,
	gds_free_cb free_cb)
{
	uint32_t hash;

	GDS_CHECK_ARG_NOT_NULL(h);

	hash = gds_hash_map_fast_hash(h, key);
	gds_rbtree_fast_set(&(h->map[hash]), key, data, h->cmpkey_cb, free_cb);

	return 0;
}

void * gds_hash_map_fast_get(gds_hash_map_fast_t *h, const void *key)
{
	uint32_t hash;
	void *data;

	GDS_CHECK_ARG_NOT_NULL(h);

	hash = gds_hash_map_fast_hash(h, key);
	data = gds_rbtree_fast_get(h->map[hash], key, h->cmpkey_cb);
	return data;
}

int gds_hash_map_fast_unset(gds_hash_map_fast_t *h, const void *key,
	gds_free_cb key_free_cb, gds_free_cb free_cb)
{
	uint32_t hash;
	int rv;

	GDS_CHECK_ARG_NOT_NULL(h);

	hash = gds_hash_map_fast_hash(h, key);
	rv = gds_rbtree_fast_del(&(h->map[hash]), key, h->cmpkey_cb,
		key_free_cb, free_cb);

	return rv;
}

typedef struct {
	gds_hash_map_fast_t *hash;
	uint32_t i;
	gds_iterator_t *rbtree_fast_it;
} gds_hash_map_fast_iterator_data_t;

void gds_hash_map_fast_iterator_find_next_rbtree_fast(gds_hash_map_fast_iterator_data_t *data)
{
	gds_hash_map_fast_t *hash = data->hash;
	uint32_t i = data->i;

	while (i < hash->size && hash->map[i] == NULL)
		i++;

	gds_iterator_free(data->rbtree_fast_it);
	data->rbtree_fast_it = NULL;

	if (i < hash->size) {
		data->rbtree_fast_it = gds_rbtree_fast_iterator_new(hash->map[i]);
	}

	data->i = i;
}

int gds_hash_map_fast_iterator_reset(gds_hash_map_fast_iterator_data_t *data)
{
	data->i = 0;
	gds_hash_map_fast_iterator_find_next_rbtree_fast(data);

	return 0;
}

int gds_hash_map_fast_iterator_step(gds_hash_map_fast_iterator_data_t *data)
{
	int rc;

	if (data->rbtree_fast_it == NULL)
		return 1;

	rc = gds_iterator_step(data->rbtree_fast_it);
	if (rc > 0) {
		data->i++;
		gds_hash_map_fast_iterator_find_next_rbtree_fast(data);
		if (data->rbtree_fast_it != NULL) {
			rc = gds_iterator_step(data->rbtree_fast_it);
		}
	}

	return rc;
}

void * gds_hash_map_fast_iterator_get(gds_hash_map_fast_iterator_data_t *data)
{
	return gds_iterator_get(data->rbtree_fast_it);
}

void * gds_hash_map_fast_iterator_getkey(gds_hash_map_fast_iterator_data_t *data)
{
	return gds_iterator_getkey(data->rbtree_fast_it);
}

void gds_hash_map_fast_iterator_data_free(gds_hash_map_fast_iterator_data_t *data)
{
	gds_iterator_free(data->rbtree_fast_it);
	free(data);
}

gds_iterator_t * gds_hash_map_fast_iterator_new(gds_hash_map_fast_t *h)
{
	gds_hash_map_fast_iterator_data_t *data;
	gds_iterator_t *it;

	data = malloc(sizeof(gds_hash_map_fast_iterator_data_t));
	if (data == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_hash_map_fast_iterator_data_t));
	}

	data->hash = h;
	data->rbtree_fast_it = NULL;

	it = gds_iterator_new(data,
		(gds_iterator_reset_cb) gds_hash_map_fast_iterator_reset,
		(gds_iterator_step_cb) gds_hash_map_fast_iterator_step,
		(gds_iterator_get_cb) gds_hash_map_fast_iterator_get,
		(gds_iterator_getkey_cb) gds_hash_map_fast_iterator_getkey,
		(gds_free_cb) gds_hash_map_fast_iterator_data_free);

	return it;
}

void gds_hash_map_fast_free(gds_hash_map_fast_t *h, gds_free_cb key_free_cb,
	gds_free_cb free_cb)
{
	uint32_t i;
	if (h != NULL) {
		for (i=0; i<h->size; i++) {
			gds_rbtree_fast_free(h->map[i], key_free_cb,
				free_cb);
		}
		free(h->map);
		free(h);
	}
}
