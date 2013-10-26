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

/*
 * File              : hash_map_keyin_fast.h
 * Short description : hash map management
 */

#ifndef gds_hash_map_keyin_fast_h_included
#define gds_hash_map_keyin_fast_h_included

#include <stdint.h>
#include "callbacks.h"
#include "rbtree_keyin_fast.h"

struct gds_hash_map_keyin_fast_s {
	uint32_t size;
	gds_rbtree_keyin_fast_node_t **map;
	gds_hash_cb hash_cb;
	gds_getkey_cb getkey_cb;
	gds_cmpkey_cb cmpkey_cb;
};
typedef struct gds_hash_map_keyin_fast_s gds_hash_map_keyin_fast_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new hash map */
/* size: the number of buckets
 *   hash_cb : hash callback
 * getkey_cb : getkey callback
 * cmpkey_cb : cmpkey callback */
/* Return: a pointer to the new hash map */
gds_hash_map_keyin_fast_t *
gds_hash_map_keyin_fast_new(
	uint32_t size,
	gds_hash_cb hash_cb,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb
);

/* Set a key/value pair in the hash map */
/* If key already exists, old value is replaced by the new one */
/*         h : pointer to the hash map
 *      data : data to insert */
/* Return: 0: key was already in the hash map
 *         1: key was just added */
int
gds_hash_map_keyin_fast_set(
	gds_hash_map_keyin_fast_t *h,
	void *data,
	gds_free_cb free_cb
);

/* Get a value given its key */
/*         h : pointer to the hash map
 *       key : key to unset */
/* Return: pointer to the data, or NULL if key doesn't exist */
void *
gds_hash_map_keyin_fast_get(
	gds_hash_map_keyin_fast_t *h,
	const void *key
);

/* Unset a key/value pair in the hash map */
/*         h : pointer to the hash map
 *       key : key to unset
 *   free_cb : free callback */
/* Return: 0: key was correctly unset
 *         1: key was not in the hash map */
int
gds_hash_map_keyin_fast_unset(
	gds_hash_map_keyin_fast_t *h,
	const void *key,
	gds_free_cb free_cb
);

/* Create a new iterator.
 *
 * Parameters
 *   h : Pointer to hash
 *
 * Returns
 *   Pointer to the newly created iterator.
 */
gds_iterator_t *
gds_hash_map_keyin_fast_iterator_new(
	gds_hash_map_keyin_fast_t *h
);

#define gds_hash_map_keyin_fast_foreach(key, val, hash) \
	for (gds_iterator_t *gds_hash_map_keyin_fast_it = gds_hash_map_keyin_fast_iterator_new(hash) \
		; gds_hash_map_keyin_fast_it != NULL \
		; gds_iterator_free(gds_hash_map_keyin_fast_it), gds_hash_map_keyin_fast_it = NULL) \
	while (!gds_iterator_step(gds_hash_map_keyin_fast_it) \
		&& ((key = gds_iterator_getkey(gds_hash_map_keyin_fast_it)) || !key) \
		&& ((val = gds_iterator_get(gds_hash_map_keyin_fast_it)) || !val))

/* Return values contained in the hash map as a list */
/*         h : pointer to the hash map */
/* Return: pointer to the list */
gds_slist_t *
gds_hash_map_keyin_fast_values(
	gds_hash_map_keyin_fast_t *h
);

/* Change the number of buckets of hash map */
/*         h : pointer to the hash map
 *  new_size : New size (number of buckets) */
void
gds_hash_map_keyin_fast_change_size(
	gds_hash_map_keyin_fast_t *h,
	uint32_t new_size
);

/* Free hash map */
/*       h : pointer to the hash map
 * free_cb : free callback */
void
gds_hash_map_keyin_fast_free(
	gds_hash_map_keyin_fast_t *h,
	gds_free_cb free_cb
);

#ifdef __cplusplus
}
#endif

#endif /* Not gds_hash_map_keyin_fast_h_included */
