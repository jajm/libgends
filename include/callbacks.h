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

#ifndef callbacks_h_included
#define callbacks_h_included

#include <stdint.h>

/* Free memory used by an object
 *
 * Parameters
 *   ptr : pointer to object
 */
typedef void (*gds_free_cb)(void *ptr);

/* Retrieve the key of an object
 *
 * Parameters
 *   ptr : pointer to object
 *
 * Should returns
 *   key of object
 */
typedef void * (*gds_getkey_cb)(const void *ptr);

/* Compare two keys
 *
 * Parameters
 *   key1 : a key as returned by gds_getkey_cb
 *   key2 : a key as returned by gds_getkey_cb
 *
 * Should returns
 *   if key1 < key2: a negative value
 *   if key1 = key2: 0 (zero)
 *   if key1 > key2: a positive value
 */
typedef int (*gds_cmpkey_cb)(const void *key1, const void *key2);

/* Compare two objects
 *
 * Parameters
 *   ptr1 : first object to compare
 *   ptr2 : second object to compare
 *
 * Should returns
 *   if object1 < object2: a negative value
 *   if object1 = object2: 0 (zero)
 *   if object1 > object2: a positive value
 */
typedef int (*gds_cmp_cb)(const void *ptr1, const void *ptr2);

/* Compute the hash of an object
 *
 * Parameters
 *   ptr  : object to compute
 *   size : size of hash map
 *
 * Should returns:
 *   hash (integer) of the object
 */
typedef uint32_t (*gds_hash_cb)(const void *ptr, uint32_t size);

#endif /* Not callbacks_h_included */
