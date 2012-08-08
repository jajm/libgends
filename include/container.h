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
 * File              : container.h                                           *
 * Short description : Generic data container                                *
 *****************************************************************************
 * A 'container' can contain one and only one generic data, represented by a *
 * pointer to the data, and the data type name                               *
 *****************************************************************************/

/*****************************************************************************
 * Custom functions used:                                                    *
 * - alloc:                                                                  *
 *   Prototype: void * alloc(void *data);                                    *
 *   Takes a pointer to data in parameter and should return a pointer to a   *
 *   copy of data.                                                           *
 *   Used in gds_container_new, gds_container_set, and gds_container_get     *
 *   if parameter copy_data is true.                                         *
 * - free:                                                                   *
 *   Prototype: void free(void *data);                                       *
 *   Takes a pointer to data and free it.                                    *
 *   Used in gds_container_set and gds_container_free if parameter free_data *
 *   (or free_old_data) is true.                                             *
 * - cmp:                                                                    *
 *   Prototype: int32_t cmp(void *data1, void *data2);                       *
 *   Compare two data, should return 0 if data are equals, a positive value  *
 *   if data1 is greater than data2, or a negative value if data1 is lesser  *
 *   than data2.                                                             *
 *   Used in gds_container_cmp.                                              *
 *****************************************************************************/

#ifndef container_h_included
#define container_h_included

#include <stdint.h>
#include <stdbool.h>

typedef struct{
	char *type_name;        /* Data type */
	void *data_ptr;         /* Data pointer */
} gds_container_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new container */
/* type_name : name of a registered type
 *  data_ptr : pointer to the data
 * copy_data : true => copy the data
 *             false => just take the pointer value */
/* Return: a pointer to the newly created container */
gds_container_t *
gds_container_new(
	const char *type_name,
	void *data_ptr,
	bool copy_data
);

/* Create a new container from another container */
/* src: pointer to the container to clone */
/* Return: pointer to the newly created container */
gds_container_t *
gds_container_new_clone(
	const gds_container_t *src
);

/* Set a new value to an existing container */
/*             c : pointer to the container
 *     type_name : name of a registered type (see core/types.h)
 *      data_ptr : pointer to the data
 * free_old_data : true => free memory taken by old value
 *                 false => don't free memory
 *     copy_data : true => copy the data
 *                 false => just take the pointer value */
void
gds_container_set(
	gds_container_t *c,
	const char *type_name,
	void *data_ptr,
	bool free_old_data,
	bool copy_data
);

/* Set a new value to an existing container, from another container */
/*           dst : pointer to the container to modify
 *           src : pointer to the container to copy value from
 * free_old_data : true => free memory taken by old value
 *                 false => don't free memory */
void
gds_container_set_clone(
	gds_container_t *dst,
	const gds_container_t *src,
	bool free_old_data
);

/* Get the contained data */
/*         c : pointer to the container */
/* copy_data : true => return a pointer to a copy of the data
 *             false => return a direct pointer to the data */
/* Return: pointer to the data */
void *
gds_container_get(
	gds_container_t *c,
	bool copy_data
);

/* Compare two containers, needs that both containers are of the same type,
 * and function 'cmp' is registered for this type */
/* c1 : first container
 * c2 : second container */
/* Return: Success => the value of 'cmp' function
 *         Failure => a value different than 0 (actually you can't know
 *                    if this function succeed or not) */
int32_t
gds_container_cmp(
	const gds_container_t *c1,
	const gds_container_t *c2
);

/* Free the memory */
/*         c : pointer to the container
 * free_data : true => try to free the data with "free" registered function
 *                     or with standard free() function
 *             false => don't free memory occupied by the data */
void
gds_container_free(
	gds_container_t *c,
	bool free_data
);

#ifdef __cplusplus
}
#endif

#endif /* Not container_h_included */

