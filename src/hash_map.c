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
#include "rbtree.h"
#include "hash_map.h"

gds_hash_map_t * gds_hash_map_new(uint32_t size, void *hash_cb, void *cmpkey_cb,
	void *key_free_cb, void *free_cb)
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
	h->key_free_cb = key_free_cb;
	h->free_cb = free_cb;

	return h;
}

uint32_t gds_hash_map_hash(gds_hash_map_t *h, const void *key)
{
	return h->hash_cb(key, h->size) % h->size;
}

int gds_hash_map_set(gds_hash_map_t *h, void *key, void *data)
{
	uint32_t hash;
	int rc;

	GDS_CHECK_ARG_NOT_NULL(h);

	hash = gds_hash_map_hash(h, key);
	rc = gds_rbtree_set(&(h->map[hash]), key, data, h->cmpkey_cb,
		h->key_free_cb, h->free_cb);

	return rc;
}

void * gds_hash_map_get(gds_hash_map_t *h, const void *key)
{
	uint32_t hash;
	void *data;

	GDS_CHECK_ARG_NOT_NULL(h);

	hash = gds_hash_map_hash(h, key);
	data = gds_rbtree_get(h->map[hash], key, h->cmpkey_cb);
	return data;
}

int gds_hash_map_unset(gds_hash_map_t *h, const void *key)
{
	uint32_t hash;
	int rv;

	GDS_CHECK_ARG_NOT_NULL(h);

	hash = gds_hash_map_hash(h, key);
	rv = gds_rbtree_del(&(h->map[hash]), key, h->cmpkey_cb,
		h->key_free_cb, h->free_cb);
	
	return rv;
}

typedef struct {
	gds_hash_map_t *hash;
	uint32_t i;
	gds_iterator_t *rbtree_it;
} gds_hash_map_iterator_data_t;

void gds_hash_map_iterator_find_next_rbtree(gds_hash_map_iterator_data_t *data)
{
	gds_hash_map_t *hash = data->hash;
	uint32_t i = data->i;

	while (i < hash->size && hash->map[i] == NULL)
		i++;

	gds_iterator_free(data->rbtree_it);
	data->rbtree_it = NULL;

	if (i < hash->size) {
		data->rbtree_it = gds_rbtree_iterator_new(hash->map[i]);
	}

	data->i = i;
}

int gds_hash_map_iterator_reset(gds_hash_map_iterator_data_t *data)
{
	data->i = 0;
	gds_hash_map_iterator_find_next_rbtree(data);

	return 0;
}

int gds_hash_map_iterator_step(gds_hash_map_iterator_data_t *data)
{
	int rc;

	if (data->rbtree_it == NULL)
		return 1;

	rc = gds_iterator_step(data->rbtree_it);
	if (rc > 0) {
		data->i++;
		gds_hash_map_iterator_find_next_rbtree(data);
		if (data->rbtree_it != NULL) {
			rc = gds_iterator_step(data->rbtree_it);
		}
	}

	return rc;
}

void * gds_hash_map_iterator_get(gds_hash_map_iterator_data_t *data)
{
	void *d = NULL;

	if (data != NULL && data->rbtree_it != NULL) {
		d = gds_iterator_get(data->rbtree_it);
	}

	return d;
}

void * gds_hash_map_iterator_getkey(gds_hash_map_iterator_data_t *data)
{
	void *key = NULL;

	if (data != NULL && data->rbtree_it != NULL) {
		key = gds_iterator_getkey(data->rbtree_it);
	}

	return key;
}

void gds_hash_map_iterator_data_free(gds_hash_map_iterator_data_t *data)
{
	gds_iterator_free(data->rbtree_it);
	free(data);
}

gds_iterator_t * gds_hash_map_iterator_new(gds_hash_map_t *h)
{
	gds_hash_map_iterator_data_t *data;
	gds_iterator_t *it;

	data = malloc(sizeof(gds_hash_map_iterator_data_t));
	if (data == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_hash_map_iterator_data_t));
	}

	data->hash = h;
	data->rbtree_it = NULL;

	it = gds_iterator_new(data, gds_hash_map_iterator_reset,
		gds_hash_map_iterator_step, gds_hash_map_iterator_get,
		gds_hash_map_iterator_getkey, gds_hash_map_iterator_data_free);

	return it;
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
			gds_slist_splice(l, 0, 0, NULL, NULL, list);
			gds_slist_free(list, NULL, NULL);
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
			gds_slist_splice(l, 0, 0, NULL, NULL, list);
			gds_slist_free(list, NULL, NULL);
		}
	}

	return l;
}

gds_rbtree_node_t ** gds_hash_map_build_map(gds_hash_map_t *h, uint32_t size)
{
	gds_rbtree_node_t **map;
	gds_iterator_t *it;
	uint32_t hash;
	void *k, *v;

	map = calloc(size, sizeof(gds_rbtree_node_t *));
	if (map == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_node_t *));
	}
	it = gds_hash_map_iterator_new(h);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it)) {
		v = gds_iterator_get(it);
		k = gds_iterator_getkey(it);
		hash = h->hash_cb(k, size) % size;
		gds_rbtree_add(&(map[hash]), k, v, h->cmpkey_cb);
	}
	gds_iterator_free(it);

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

void gds_hash_map_free(gds_hash_map_t *h)
{
	uint32_t i;
	if (h != NULL) {
		for (i=0; i<h->size; i++) {
			gds_rbtree_free(h->map[i], h->key_free_cb, h->free_cb);
		}
		free(h->map);
		free(h);
	}
}
