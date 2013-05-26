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

/* Callback: gds_free_cb */
/* Purpose: Free memory used by an object */
/* 1st parameter: pointer to object */
/* Should returns: nothing */
typedef void (*gds_free_cb)(void *);

/* Callback: gds_getkey_cb */
/* Purpose: Retrieve the key of an object */
/* 1st parameter: pointer to object */
/* Should returns: key of object */
typedef void * (*gds_getkey_cb)(void *);

/* Callback: gds_cmpkey_cb */
/* Purpose: Compare two keys */
/* 1st paramater: a key as returned by gds_getkey_cb (key1)
 * 2nd paramater: a key as returned by gds_getkey_cb (key2) */
/* Should returns:
 *  - if key1 < key2: a negative value
 *  - if key1 = key2: 0 (zero)
 *  - if key1 > key2: a positive value */
typedef int32_t (*gds_cmpkey_cb)(void *, void *);

/* Callback: gds_cmp_cb */
/* Purpose: Compare two objects */
/* 1st parameter: first object to compare (object1)
 * 2nd parameter: second object to compare (object2) */
/* Should returns:
 *  - if object1 < object2: a negative value
 *  - if object1 = object2: 0 (zero)
 *  - if object1 > object2: a positive value */
typedef int32_t (*gds_cmp_cb)(void *, void *);

/* Callback: gds_hash_cb */
/* Purpose: Compute the hash of an object */
/* 1st parameter: object to compute
 * 2nd parameter: size of hash map */
/* Should returns: hash (integer) of the object */
typedef uint32_t (*gds_hash_cb)(const void *, uint32_t);

#endif /* Not callbacks_h_included */
