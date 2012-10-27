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
 * File              : hash_map.h                                            *
 * Short description : Low-level hash map management                         *
 *****************************************************************************/

#ifndef gds_hash_map_h_included
#define gds_hash_map_h_included

#include <stdint.h>
#include "callbacks.h"
#include "rbtree.h"

struct gds_hash_map_s {
	uint32_t size;
	gds_rbtree_node_t **map;
	gds_hash_cb hash_cb;
	gds_cmpkey_cb cmpkey_cb;
};
typedef struct gds_hash_map_s gds_hash_map_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new hash map */
/* size: the number of buckets
 *   hash_cb : hash callback
 * cmpkey_cb : cmpkey callback */
/* Return: a pointer to the new hash map */
gds_hash_map_t *
gds_hash_map_new(
	uint32_t size,
	gds_hash_cb hash_cb,
	gds_cmpkey_cb cmpkey_cb
);

/* Set a key/value pair in the hash map */
/* If key already exists, old value is replaced by the new one */
/*            h : pointer to the hash map
 *          key : key to insert
 *         data : data to insert
 * key_alloc_cb : alloc callback for key
 *      free_cb : free callback for data
 *     alloc_cb : alloc callback for data */
/* Return: 0: key was already in the hash map
 *         1: key was just added */
int8_t
gds_hash_map_set(
	gds_hash_map_t *h,
	void *key,
	void *data,
	gds_alloc_cb key_alloc_cb,
	gds_free_cb free_cb,
	gds_alloc_cb alloc_cb
);

/* Get a value given its key */
/*         h : pointer to the hash map
 *       key : key to unset
 *  alloc_cb : alloc callback */
/* Return: pointer to the data, or NULL if key doesn't exist */
void *
gds_hash_map_get(
	gds_hash_map_t *h,
	void *key,
	gds_alloc_cb alloc_cb
);

/* Unset a key/value pair in the hash map */
/*           h : pointer to the hash map
 *         key : key to unset
 * key_free_cb : free callback for key
 *     free_cb : free callback for data */
/* Return: 0: key was correctly unset
 *         1: key was not in the hash map */
int8_t
gds_hash_map_unset(
	gds_hash_map_t *h,
	void *key,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

/* Return keys contained in the hash map as a list */
/*            h : pointer to the hash map
 * key_alloc_cb : alloc callback */
/* Return: pointer to the list */
gds_slist_node_t *
gds_hash_map_keys(
	gds_hash_map_t *h,
	gds_alloc_cb key_alloc_cb
);

/* Return values contained in the hash map as a list */
/*        h : pointer to the hash map
 * alloc_cb : alloc callback */
/* Return: pointer to the list */
gds_slist_node_t *
gds_hash_map_values(
	gds_hash_map_t *h,
	gds_alloc_cb alloc_cb
);

/* Return key/value pairs contained in hash map as a list of gds_key_value_t */
/*            h : pointer to the hash map */
/* Return: pointer to the list */
gds_slist_node_t *
gds_hash_map_keys_values(
	gds_hash_map_t *h
);

/* Change the number of buckets of hash map */
/*        h : pointer to the hash map
 * new_size : New size (number of buckets) */
void
gds_hash_map_change_size(
	gds_hash_map_t *h,
	uint32_t new_size
);

/* Free hash map */
/*           h : pointer to the hash map
 * key_free_cb : free callback for key
 *     free_cb : free callback for data */
void
gds_hash_map_free(
	gds_hash_map_t *h,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

#ifdef __cplusplus
}
#endif

#endif /* Not gds_hash_map_h_included */

