/*****************************************************************************
 * Copyright (C) 2010-2012 Julian Maurice                                    *
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
 * Short description : Hash table management                                 *
 *****************************************************************************/

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "exception.h"
#include "check_arg.h"
#include "types.h"
#include "log.h"
#include "hash_map.h"
#include "callbacks.h"

typedef struct gds_hash_map_list_node_s {
	char *key;
	void *data;
	struct gds_hash_map_list_node_s *next;
} gds_hash_map_list_node_t, *gds_hash_map_list_t;

gds_hash_map_list_node_t * gds_hash_map_list_node_new(const char *key,
	void *data, bool copy_data, gds_alloc_cb alloc_cb)
{
	gds_hash_map_list_node_t *node;
	size_t len;

	if(key == NULL || (copy_data && (data == NULL || alloc_cb == NULL))) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	node = malloc(sizeof(gds_hash_map_list_node_t));
	if(node == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}

	len = strlen(key);
	node->key = malloc(len+1);
	if(node->key == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		free(node);
		return NULL;
	}

	strncpy(node->key, key, len+1);
	assert(node->key[len] == '\0');

	if(copy_data) {
		node->data = alloc_cb(data);
		if(node->data == NULL) {
			GDS_LOG_ERROR("Memory allocation error");
			free(node->key);
			free(node);
			return NULL;
		}
	} else {
		node->data = data;
	}
	node->next = NULL;

	return node;
}

int8_t gds_hash_map_list_node_set_data(gds_hash_map_list_node_t *node,
	void *data, bool free_old_data, gds_free_cb free_cb, bool copy_data,
	gds_alloc_cb alloc_cb)
{
	if(node == NULL || (copy_data && (data == NULL || alloc_cb == NULL))) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	if(free_old_data) {
		if(free_cb) {
			free_cb(node->data);
		} else {
			free(node->data);
		}
	}

	if(copy_data) {
		node->data = alloc_cb(data);
		if(node->data == NULL) {
			GDS_LOG_ERROR("Memory allocation error");
			return -1;
		}
	}

	return 0;
}

void *gds_hash_map_list_node_get_data(gds_hash_map_list_node_t *node,
	bool copy_data, gds_alloc_cb alloc_cb)
{
	void *data;

	if(node == NULL || (copy_data && alloc_cb == NULL)) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	if(copy_data) {
		data = alloc_cb(node->data);
		if(data == NULL) {
			GDS_LOG_ERROR("Memory allocation");
			return NULL;
		}
	} else {
		data = node->data;
	}

	return data;
}

int8_t gds_hash_map_list_node_set_key(gds_hash_map_list_node_t *node,
	const char *key)
{
	size_t len;

	if(node == NULL || key == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	len = strlen(key);
	node->key = realloc(node->key, len+1);
	if(node->key == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return -1;
	}
	strncpy(node->key, key, len+1);
	assert(node->key[len] == '\0');

	return 0;
}

char *gds_hash_map_list_node_get_key(gds_hash_map_list_node_t *node)
{
	return (node != NULL) ? (node->key) : (NULL);
}

void gds_hash_map_list_node_free(gds_hash_map_list_node_t *node, bool free_data,
	gds_free_cb free_cb)
{
	if(node) {
		free(node->key);
		if(free_data && free_cb) {
			free_cb(node->data);
		} else if(free_data) {
			GDS_LOG_ERROR("free_data is true but free_cb is NULL, "
				"data not freed.");
		}
		free(node);
	}
}

gds_hash_map_list_node_t *gds_hash_map_list_add(
	gds_hash_map_list_t *hash_map_list, const char *key, void *data,
	bool free_old_data, gds_free_cb free_cb, bool copy_data,
	gds_alloc_cb alloc_cb)
{
	gds_hash_map_list_node_t *cur, *prev = NULL, *newnode = NULL;
	int32_t cmp;

	if( hash_map_list == NULL || key == NULL
	|| (copy_data && (data == NULL || alloc_cb == NULL)) )
	{
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	cur = *hash_map_list;
	while(cur != NULL && (cmp = strcmp(cur->key, key)) <= 0) {
		if(cmp == 0) {
			newnode = cur;
			break;
		}
		prev = cur;
		cur = cur->next;
	}
	if(newnode != NULL) {
		gds_hash_map_list_node_set_data(newnode, data, free_old_data,
			free_cb, copy_data, alloc_cb);
	} else {
		newnode = gds_hash_map_list_node_new(key, data, copy_data,
			alloc_cb);
		if(newnode == NULL) {
			GDS_LOG_ERROR("Failed to create gds_hash_map_list_node_t");
			return NULL;
		}
		newnode->next = cur;
		if(prev != NULL) {
			prev->next = newnode;
		} else {
			*hash_map_list = newnode;
		}
	}

	return newnode;
}

gds_hash_map_list_node_t * gds_hash_map_list_get(
	gds_hash_map_list_t hash_map_list, const char *key)
{
	gds_hash_map_list_node_t *cur, *node = NULL;
	int32_t cmp;

	if(key == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	cur = hash_map_list;
	while(cur != NULL && (cmp = strcmp(cur->key, key)) < 0) {
		cur = cur->next;
	}
	if(cmp == 0 && cur != NULL) {
		node = cur;
	}
	return node;
}

void * gds_hash_map_list_get_data(gds_hash_map_list_t hash_map_list,
	const char *key, bool copy_data, gds_alloc_cb alloc_cb)
{
	gds_hash_map_list_node_t *node;

	node = gds_hash_map_list_get(hash_map_list, key);
	return gds_hash_map_list_node_get_data(node, copy_data, alloc_cb);
}

int8_t gds_hash_map_list_del(gds_hash_map_list_t *hash_map_list,
	const char *key, bool free_data, gds_free_cb free_cb)
{
	gds_hash_map_list_node_t *prev = NULL, *cur;

	if(hash_map_list == NULL || key == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	cur = *hash_map_list;
	while(cur != NULL && strcmp(cur->key, key) != 0) {
		prev = cur;
		cur = cur->next;
	}
	if(cur != NULL) {
		if(prev) {
			prev->next = cur->next;
		} else {
			*hash_map_list = cur->next;
		}
		gds_hash_map_list_node_free(cur, free_data, free_cb);
	}

	return 0;
}

bool gds_hash_map_list_exists(gds_hash_map_list_t hash_map_list,
	const char *key)
{
	gds_hash_map_list_node_t *cur;

	if(key != NULL) {
		cur = hash_map_list;
		while(cur != NULL && strcmp(cur->key, key) != 0) {
			cur = cur->next;
		}
		if(cur != NULL) {
			return true;
		}
	}

	return false;
}

void gds_hash_map_list_free(gds_hash_map_list_t hash_map_list, bool free_data,
	gds_free_cb free_cb)
{
	gds_hash_map_list_node_t *cur, *next;

	cur = hash_map_list;
	while(cur != NULL) {
		next = cur->next;
		gds_hash_map_list_node_free(cur, free_data, free_cb);
		free(cur);
		cur = next;
	}
}

uint32_t gds_hash_map_default_hash_cb(const char *key, uint32_t map_size)
{
	uint32_t i;
	uint32_t hash = 0;
	size_t len;

	if(key == NULL || map_size == 0) {
		GDS_LOG_ERROR("Bad parameters");
		return 0;
	}

	len = strlen(key);
	for(i=0; i<len; i++) {
		hash += key[i];
	}
	hash %= map_size;

	return hash;
}

struct gds_hash_map_s {
	char *type_name;
	uint32_t map_size;
	gds_hash_cb hash_cb;
	gds_hash_map_list_t *map;
};

uint32_t gds_hash_map_hash(gds_hash_map_t *h, const char *key)
{
	uint32_t hash;

	if(h == NULL || key == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return 0;
	}

	hash = h->hash_cb(key, h->map_size);
	hash %= h->map_size;

	return hash;
}

int8_t gds_hash_map_rebuild(gds_hash_map_t *h)
{
	gds_hash_map_list_t *map;
	gds_hash_map_list_node_t *node, *tmp;
	uint32_t i;
	uint32_t hash;
	bool error = false;

	if(h == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

	map = calloc(h->map_size, sizeof(gds_hash_map_list_t));
	if(map == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return -1;
	}

	for(i=0; i<h->map_size; i++) {
		node = h->map[i];
		while(node != NULL) {
			hash = gds_hash_map_hash(h, node->key);
			tmp = gds_hash_map_list_add(&(map[hash]), node->key,
				node->data, false, NULL, false, 0);
			if(tmp == NULL) {
				GDS_LOG_ERROR("Failed to add existing node to"
					" new map");
				error = true;
				break;
			}
			node = node->next;
		}
		if(error) {
			break;
		}
	}

	if(error) {
		GDS_LOG_ERROR("GDS_LOG_ERROR while creating new map, abort");
		return -1;
	}

	for(i=0; i<h->map_size; i++) {
		gds_hash_map_list_free(h->map[i], false, NULL);
	}
	free(h->map);
	h->map = map;

	return 0;
}

gds_hash_map_t * gds_hash_map_new(const char *type_name, uint32_t map_size,
	gds_hash_cb hash_cb)
{
	gds_hash_map_t *hash_map;
	size_t len;

	if(type_name == NULL || map_size == 0) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}
	hash_map = malloc(sizeof(gds_hash_map_t));
	if(hash_map == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	len = strlen(type_name);
	hash_map->type_name = malloc(len+1);
	if(hash_map->type_name == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		free(hash_map);
		return NULL;
	}
	strncpy(hash_map->type_name, type_name, len+1);
	assert(hash_map->type_name[len] == '\0');
	hash_map->map = malloc(map_size*sizeof(gds_hash_map_list_node_t *));
	if(hash_map->map == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		free(hash_map->type_name);
		free(hash_map);
		return NULL;
	}
	hash_map->map_size = map_size;
	if(hash_cb != NULL) {
		hash_map->hash_cb = hash_cb;
	} else {
		hash_map->hash_cb = (gds_hash_cb)gds_hash_map_default_hash_cb;
	}

	return hash_map;
}

char * gds_hash_map_get_type_name(gds_hash_map_t *h)
{
	if(h == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	return h->type_name;
}

int8_t gds_hash_map_set_type_name(gds_hash_map_t *h, const char *type_name)
{
	char *buf;
	size_t len;

	if(h == NULL || type_name == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	len = strlen(type_name);
	buf = malloc(len+1);
	if(buf == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return -1;
	}
	strncpy (buf, type_name, len+1);
	assert(buf[len] == '\0');

	free(h->type_name);
	h->type_name = buf;

	return 0;
}

uint32_t gds_hash_map_get_map_size(gds_hash_map_t *h)
{
	if(h == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return 0;
	}

	return h->map_size;
}

int8_t gds_hash_map_set_map_size(gds_hash_map_t *h, uint32_t map_size)
{
	uint32_t old_map_size;

	if(h == NULL && map_size == 0) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	if(h->map_size == map_size) {
		/* Do nothing */
		return 1;
	}

	old_map_size = h->map_size;
	h->map_size = map_size;
	if(gds_hash_map_rebuild(h) < 0) {
		GDS_LOG_ERROR("Failed to rebuild hash map");
		h->map_size = old_map_size;
		return -1;
	}

	return 0;
}

gds_hash_cb gds_hash_map_get_hash_cb(gds_hash_map_t *h)
{
	if(h == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	return h->hash_cb;
}

int8_t gds_hash_map_set_hash_cb(gds_hash_map_t *h, gds_hash_cb hash_cb)
{
	gds_hash_cb old_hash_cb;

	if(h == NULL || hash_cb == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	if(h->hash_cb == hash_cb) {
		/* Do nothing */
		return 1;
	}

	old_hash_cb = h->hash_cb;
	h->hash_cb = hash_cb;
	if(gds_hash_map_rebuild(h) < 0) {
		GDS_LOG_ERROR("Failed to rebuild hash map");
		h->hash_cb = old_hash_cb;
		return -1;
	}

	return 0;
}

int8_t gds_hash_map_set(gds_hash_map_t *h, const char *key, bool free_old_data,
	void *data, bool copy_data)
{
	uint32_t hash;
	gds_alloc_cb alloc_cb = NULL;
	gds_free_cb free_cb = NULL;
	gds_hash_map_list_node_t *node;

	if(h == NULL || key == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	hash = gds_hash_map_hash(h, key);
	if(free_old_data) {
		free_cb = (gds_free_cb)gds_type_get_func(h->type_name, "free");
	}
	if(copy_data) {
		alloc_cb = (gds_alloc_cb)gds_type_get_func(h->type_name, "alloc");
	}
	node = gds_hash_map_list_add(&(h->map[hash]), key, data, free_old_data,
		free_cb, copy_data, alloc_cb);
	if(node == NULL) {
		GDS_LOG_ERROR("Failed to set data for '%s' key", key);
		return -1;
	}

	return 0;
}

void * gds_hash_map_get(gds_hash_map_t *h, const char *key, bool copy_data)
{
	uint32_t hash;
	gds_alloc_cb alloc_cb = NULL;

	if(h == NULL || key == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	hash = gds_hash_map_hash(h, key);

	if(copy_data) {
		alloc_cb = (gds_alloc_cb)gds_type_get_func(h->type_name, "alloc");
	}

	return gds_hash_map_list_get_data(h->map[hash], key, copy_data, alloc_cb);
}

bool gds_hash_map_exists(gds_hash_map_t *h, const char *key)
{
	uint32_t hash;

	if(h != NULL && key != NULL) {
		hash = gds_hash_map_hash(h, key);
		return gds_hash_map_list_exists(h->map[hash], key);
	}

	return false;
}

int8_t gds_hash_map_set_key(gds_hash_map_t *h, const char *old_key,
	const char *new_key)
{
	void *data;
	uint32_t old_key_hash, new_key_hash;

	if(h == NULL || old_key == NULL || new_key == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

	if(!gds_hash_map_exists(h, old_key)) {
		GDS_LOG_ERROR("Key does not exist in hash: '%s'", old_key);
		return -1;
	}

	old_key_hash = gds_hash_map_hash(h, old_key);
	new_key_hash = gds_hash_map_hash(h, new_key);

	data = gds_hash_map_list_get_data(h->map[old_key_hash], old_key, false, 0);
	if(NULL == gds_hash_map_list_add(&(h->map[new_key_hash]), new_key, data,
		false, NULL, false, 0))
	{
		GDS_LOG_ERROR("Failed to change key '%s'(%p) -> '%s'(%p)",
			old_key, old_key, new_key, new_key);
		return -1;
	}
	if(0 > gds_hash_map_list_del(&(h->map[old_key_hash]), old_key, false, NULL))
	{
		GDS_LOG_ERROR("Failed to change key '%s'(%p) -> '%s'(%p)",
			old_key, old_key, new_key, new_key);
		return -1;
	}

	return 0;
}

int8_t gds_hash_map_del(gds_hash_map_t *h, const char *key, bool free_data)
{
	uint32_t hash;
	gds_free_cb free_cb = NULL;

	if(h == NULL || key == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

	hash = h->hash_cb(key, h->map_size);
	assert(hash < h->map_size);

	if(free_data) {
		free_cb = (gds_free_cb)gds_type_get_func(h->type_name, "free");
	}
	gds_hash_map_list_del(&(h->map[hash]), key, free_data, free_cb);

	return 0;
}

void gds_hash_map_free(gds_hash_map_t *h, bool free_data)
{
	uint32_t i;
	gds_free_cb free_cb = NULL;

	if(h != NULL) {
		if(free_data) {
			free_cb = (gds_free_cb)gds_type_get_func(h->type_name, "free");
		}
		for(i=0; i<h->map_size; i++) {
			gds_hash_map_list_free(h->map[i], free_data, free_cb);
		}
		free(h->map);
		free(h->type_name);
		free(h);
	}
}

typedef struct {
	gds_hash_map_t *h;
	uint32_t cur_hash;
	gds_hash_map_list_node_t *cur;
} gds_hash_map_iterator_t;

int8_t gds_hash_map_iterator_reset(gds_hash_map_iterator_t *it)
{
	if(it == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

	it->cur_hash = 0;
	it->cur = NULL;

	return 0;
}

int8_t gds_hash_map_iterator_step(gds_hash_map_iterator_t *it)
{
	uint32_t i;

	if(it == NULL || it->h == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

	if(it->cur == NULL || it->cur->next == NULL) {
		i = it->cur_hash;
		while(i<it->h->map_size && it->h->map[i] == NULL){
			i++;
		}
		if(i < it->h->map_size) {
			it->cur_hash = i;
			it->cur = it->h->map[i];
		} else {
			return 1;
		}
	} else {
		it->cur = it->cur->next;
	}

	return 0;
}

char * gds_hash_map_iterator_keys_get(gds_hash_map_iterator_t *it,
	bool copy_data)
{
	if(it == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}
	// copy_data not used, prevent compiler warnings
	if(copy_data) {}

	return gds_hash_map_list_node_get_key(it->cur);
}

void * gds_hash_map_iterator_data_get(gds_hash_map_iterator_t *it,
	bool copy_data)
{
	gds_alloc_cb alloc_cb = NULL;

	if(it == NULL || it->h == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	if(copy_data) {
		alloc_cb = (gds_alloc_cb)gds_type_get_func(it->h->type_name, "alloc");
	}

	return gds_hash_map_list_node_get_data(it->cur, copy_data, alloc_cb);
}

gds_iterator_t * gds_hash_map_iterator_keys_new(gds_hash_map_t *h)
{
	gds_hash_map_iterator_t *hit;
	gds_iterator_t *it;

	hit = malloc(sizeof(gds_hash_map_iterator_t));
	if(hit == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}

	hit->h = h;
	hit->cur_hash = 0;
	hit->cur = NULL;

	it = gds_iterator_new(
		hit,
		(gds_iterator_reset_cb) &gds_hash_map_iterator_reset,
		(gds_iterator_step_cb) &gds_hash_map_iterator_step,
		(gds_iterator_get_cb) &gds_hash_map_iterator_keys_get
	);

	return it;
}

gds_iterator_t * gds_hash_map_iterator_data_new(gds_hash_map_t *h)
{
	gds_hash_map_iterator_t *hit;
	gds_iterator_t *it;

	hit = malloc(sizeof(gds_hash_map_iterator_t));
	if(hit == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}

	hit->h = h;
	hit->cur_hash = 0;
	hit->cur = NULL;

	it = gds_iterator_new(
		hit,
		(gds_iterator_reset_cb) &gds_hash_map_iterator_reset,
		(gds_iterator_step_cb) &gds_hash_map_iterator_step,
		(gds_iterator_get_cb) &gds_hash_map_iterator_data_get
	);

	return it;
}
