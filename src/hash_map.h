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
 * File              : hash_map.h                                            *
 * Short description : Hash map management                                   *
 *****************************************************************************/

/*****************************************************************************
 * Custom functions used:                                                    *
 * - alloc:                                                                  *
 *   Prototype: void * alloc(void *data);                                    *
 *   Takes a pointer to data in parameter and should return a pointer to a   *
 *   copy of data.                                                           *
 *   Used in gds_hash_map_set and gds_hash_map_get if parameter copy_data is *
 *   true.                                                                   *
 * - free:                                                                   *
 *   Prototype: void free(void *data);                                       *
 *   Takes a pointer to data and free it.                                    *
 *   Used in gds_hash_map_set, gds_hash_map_del, and gds_hash_map_free if    *
 *   parameter free_data (or free_old_data) is true.                         *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "iterator.h"

/* Hash function pointer type */
/* Hash functions take a string (the hash key) and an integer (the map size)
 * and must return an unsigned integer strictly inferior to map_size */
typedef uint32_t (*gds_hash_cb)(const char *, uint32_t);
typedef struct gds_hash_map_s gds_hash_map_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new hash map */
/* type_name : Name of data type
 *  map_size : Hash map size
 * hash_cb : Hash function pointer, as described above */
/* Return: Success => pointer to the newly created hash map
 *         Failure => NULL */
gds_hash_map_t *
gds_hash_map_new(
	const char *type_name,
	uint32_t map_size,
	gds_hash_cb hash_cb
);

/* Get type name */
/* h : Pointer to the hash map */
/* Return: Success => type name
 *         Failure => NULL */
char *
gds_hash_map_get_type_name(
	gds_hash_map_t *h
);

/* Set type_name */
/* WARNING: changing the type name does not affect stored data at all.
 * It may cause dangerous behavior in operations that involve custom functions
 * like 'freeing memory' operations */
/*         h : Pointer to the hash map
 * type_name : Name of data type */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_hash_map_set_type_name(
	gds_hash_map_t *h,
	const char *type_name
);

/* Get map size */
/* h : pointer to the hash map */
/* Return: Success => map size
 *         Failure => 0 */
uint32_t
gds_hash_map_get_map_size(
	gds_hash_map_t *h
);

/* Set map size */
/* This operation can take some time, depending on the number of elements in
 * the hash map, since it create a new map and replace all elements into it */
/*        h : pointer to the hash map
 * map_size : new size */
/* Return: Success => 0
 *         Failure => a negative value
 *         Other => 1 if the new size is equal to the actual size */
int8_t
gds_hash_map_set_map_size(
	gds_hash_map_t *h,
	uint32_t map_size
);

/* Get hash function pointer */
/* h : pointer to the hash map */
/* Return: Success => the function pointer
 *         Failure => NULL */
gds_hash_cb
gds_hash_map_get_hash_cb(
	gds_hash_map_t *h
);

/* Set hash function */
/*         h : pointer to the hash map
 * hash_cb : function pointer */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_hash_map_set_hash_cb(
	gds_hash_map_t *h,
	gds_hash_cb hash_cb
);

/* Add or change an element in a hash */
/*             h : pointer to the hash map
 *           key : hash key of element to add or change
 * free_old_data : true => free memory occupied by data before changing it,
 *                 false => don't free memory occupied by data
 *                 This parameter has no effect if there is no previous data
 *          data : pointer to data to put into the hash
 *     copy_data : true => make a copy of data before inserting it into the hash
 *                 false => only take the pointer value */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_hash_map_set(
	gds_hash_map_t *h,
	const char *key,
	bool free_old_data,
	void *data,
	bool copy_data
);

/* Get data from a hash element */
/*         h : pointer to the hash map
 *       key : hash key of element
 * copy_data : make a copy of data before returning it */
/* Return: Success => pointer to the data
 *         Failure => NULL */
void *
gds_hash_map_get(
	gds_hash_map_t *h,
	const char *key,
	bool copy_data
);

/* Check if an element exists in a hash */
/*   h : pointer to the hash map
 * key : hash key of element */
/* Return: true => element exists
 *         false => element does not exist */
bool
gds_hash_map_exists(
	gds_hash_map_t *h,
	const char *key
);

/* Modify key of a hash element */
/*       h : pointer to the hash map
 * old_key : actual key of element to modify
 * new_key : replacement key */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_hash_map_set_key(
	gds_hash_map_t *h,
	const char *old_key,
	const char *new_key
);

/* Remove an element from a hash */
/*         h : pointer to the hash map
 *       key : hash key of element
 * free_data : true => free memory occupied by data
 *             false => don't free memory used by data */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_hash_map_del(
	gds_hash_map_t *h,
	const char *key,
	bool free_data
);

/* Create an iterator on hash keys */
/* Do not modify key directly, it can cause inconsistencies, data loss,
 * and other unexpected behaviour. To modify only the key, use
 * hash_map_set_key */
gds_iterator_t *
gds_hash_map_iterator_keys_new(
	gds_hash_map_t *h
);

/* Create an iterator on hash data */
gds_iterator_t *
gds_hash_map_iterator_data_new(
	gds_hash_map_t *h
);

/* Free memory used by hash map */
/*         h : pointer to the hash map
 * free_data : true => free memory used by data
 *             false => don't free memory used by data */
void
gds_hash_map_free(
	gds_hash_map_t *h,
	bool free_data
);

#ifdef __cplusplus
}
#endif

