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

#include <stdio.h>
#include <stdlib.h>
#include "slist.h"
#include "malloc.h"
#include "assert.h"
#include "log.h"
#include "rbtree_fast.h"
#include "hash_map_fast.h"

struct gds_hash_map_fast_s {
	unsigned long size;
	gds_rbtree_fast_node_t **map;
	unsigned long (*hash_cb)(const void *ptr, unsigned long size);
	int (*cmpkey_cb)(const void *key1, const void *key2);
	void * (*key_alloc_cb)(const void *key);
	void (*key_free_cb)(void *key);
	void (*free_cb)(void *ptr);
};

gds_hash_map_fast_t * gds_hash_map_fast_new(unsigned long size, void *hash_cb,
	void *cmpkey_cb, void *key_alloc_cb, void *key_free_cb, void *free_cb)
{
	gds_hash_map_fast_t *h;

	gds_assert(size > 0, NULL);
	gds_assert(hash_cb != NULL, NULL);
	gds_assert(cmpkey_cb != NULL, NULL);

	h = gds_malloc(sizeof(gds_hash_map_fast_t));
	h->map = gds_calloc(size, sizeof(gds_rbtree_fast_node_t *));

	h->size = size;
	h->hash_cb = hash_cb;
	h->cmpkey_cb = cmpkey_cb;
	h->key_alloc_cb = key_alloc_cb;
	h->key_free_cb = key_free_cb;
	h->free_cb = free_cb;

	return h;
}

int gds_hash_map_fast_set_free_callback(gds_hash_map_fast_t *h, void *free_cb)
{
	gds_assert(h != NULL, -1);

	h->free_cb = free_cb;
	return 0;
}

void * gds_hash_map_fast_get_free_callback(gds_hash_map_fast_t *h)
{
	gds_assert(h != NULL, NULL);

	return h->free_cb;
}

unsigned long gds_hash_map_fast_hash(gds_hash_map_fast_t *h, const void *key)
{
	return h->hash_cb(key, h->size) % h->size;
}

int gds_hash_map_fast_set(gds_hash_map_fast_t *h, void *key, void *data)
{
	unsigned long hash;
	int rc;

	gds_assert(h != NULL, -1);

	if (h->key_alloc_cb != NULL) {
		key = h->key_alloc_cb(key);
	}

	hash = gds_hash_map_fast_hash(h, key);
	rc = gds_rbtree_fast_set(&(h->map[hash]), key, data, h->cmpkey_cb,
		h->key_free_cb, h->free_cb);

	return rc;
}

void * gds_hash_map_fast_get(gds_hash_map_fast_t *h, const void *key)
{
	unsigned long hash;
	void *data;

	gds_assert(h != NULL, NULL);

	hash = gds_hash_map_fast_hash(h, key);
	data = gds_rbtree_fast_get(h->map[hash], key, h->cmpkey_cb);
	return data;
}

int gds_hash_map_fast_unset(gds_hash_map_fast_t *h, const void *key)
{
	unsigned long hash;
	int rv;

	gds_assert(h != NULL, -1);

	hash = gds_hash_map_fast_hash(h, key);
	rv = gds_rbtree_fast_del(&(h->map[hash]), key, h->cmpkey_cb,
		h->key_free_cb, h->free_cb);

	return rv;
}

void * gds_hash_map_fast_pop(gds_hash_map_fast_t *h, const void *key)
{
	unsigned long hash;
	void *data;

	gds_assert(h != NULL, NULL);

	hash = gds_hash_map_fast_hash(h, key);
	data = gds_rbtree_fast_pop(&(h->map[hash]), key, h->cmpkey_cb,
		h->key_free_cb);

	return data;
}

typedef struct {
	gds_hash_map_fast_t *hash;
	unsigned long i;
	gds_iterator_t *rbtree_fast_it;
} gds_hash_map_fast_iterator_data_t;

void gds_hash_map_fast_iterator_find_next_rbtree_fast(gds_hash_map_fast_iterator_data_t *data)
{
	gds_hash_map_fast_t *hash = data->hash;
	unsigned long i = data->i;

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

	data = gds_malloc(sizeof(gds_hash_map_fast_iterator_data_t));

	data->hash = h;
	data->rbtree_fast_it = NULL;

	it = gds_iterator_new(data, gds_hash_map_fast_iterator_reset,
		gds_hash_map_fast_iterator_step, gds_hash_map_fast_iterator_get,
		gds_hash_map_fast_iterator_getkey,
		gds_hash_map_fast_iterator_data_free);

	return it;
}

gds_slist_t * gds_hash_map_fast_keys(gds_hash_map_fast_t *h)
{
	gds_slist_t *l = gds_slist_new(NULL);
	gds_slist_t *list;
	unsigned long i;

	gds_assert(h != NULL, NULL);

	for (i = h->size; i > 0; i--) {
		list = gds_rbtree_fast_keys(h->map[i-1]);
		if (list != NULL) {
			gds_slist_splice(l, 0, 0, list);
			gds_slist_free(list);
		}
	}

	return l;
}

gds_slist_t * gds_hash_map_fast_values(gds_hash_map_fast_t *h)
{
	gds_slist_t *l = gds_slist_new(NULL);
	gds_slist_t *list;
	unsigned long i;

	gds_assert(h != NULL, NULL);

	for (i = h->size; i > 0; i--) {
		list = gds_rbtree_fast_values(h->map[i-1]);
		if (list != NULL) {
			gds_slist_splice(l, 0, 0, list);
			gds_slist_free(list);
		}
	}

	return l;
}

gds_rbtree_fast_node_t ** gds_hash_map_fast_build_map(gds_hash_map_fast_t *h, unsigned long size)
{
	gds_rbtree_fast_node_t **map;
	gds_iterator_t *it;
	unsigned long hash;
	void *k, *v;

	map = gds_calloc(size, sizeof(gds_rbtree_fast_node_t *));
	it = gds_hash_map_fast_iterator_new(h);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it)) {
		v = gds_iterator_get(it);
		k = gds_iterator_getkey(it);
		hash = h->hash_cb(k, size) % size;
		gds_rbtree_fast_add(&(map[hash]), k, v, h->cmpkey_cb);
	}
	gds_iterator_free(it);

	return map;
}

int gds_hash_map_fast_change_size(gds_hash_map_fast_t *h, unsigned long new_size)
{
	gds_rbtree_fast_node_t **map;

	gds_assert(h != NULL, -1);
	gds_assert(new_size > 0, -1);

	map = gds_hash_map_fast_build_map(h, new_size);
	for (unsigned long i = 0; i < h->size; i++) {
		gds_rbtree_fast_free(h->map[i], NULL, NULL);
	}
	free(h->map);
	h->map = map;
	h->size = new_size;

	return 0;
}

void gds_hash_map_fast_free(gds_hash_map_fast_t *h)
{
	unsigned long i;
	if (h != NULL) {
		for (i=0; i<h->size; i++) {
			gds_rbtree_fast_free(h->map[i], h->key_free_cb,
				h->free_cb);
		}
		free(h->map);
		free(h);
	}
}

void gds_hash_map_fast_destroy(gds_hash_map_fast_t *h)
{
	if (h != NULL) {
		gds_hash_map_fast_set_free_callback(h, NULL);
		gds_hash_map_fast_free(h);
	}
}
