/*****************************************************************************
 * Copyright (C) 2010-2011 Julian Maurice                                    *
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

#ifndef container_h_included
#define container_h_included

#include <stdint.h>
#include <stdbool.h>

typedef struct{
	char *type_name;        /* Data type */
	void *data_ptr;         /* Data pointer */
} container_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new container */
/* type_name : name of a registered type (see core/types.h)
 *  data_ptr : pointer to the data
 *      copy : true => copy the data
 *             false => just take the pointer value */
/* Return: Success => pointer to the newly created container
 *         Failure => NULL */
	container_t *
container_new(
	const char *type_name,
	void *data_ptr,
	bool copy_data
);

/* Create a new container from another container */
/* src: pointer to the container to clone */
/* Return: Success => pointer to the newly created container
 *         Failure => NULL */
	container_t *
container_new_clone(
	const container_t *src
);

/* Set a new value to an existing container */
/*             c : pointer to the container
 *     type_name : name of a registered type (see core/types.h)
 *      data_ptr : pointer to the data
 * free_old_data : true => free memory taken by old value
 *                 false => don't free memory
 *     copy_data : true => copy the data
 *                 false => just take the pointer value */
/* Return: Success => 0
 *         Failure => a negative value */
	int8_t
container_set(
	container_t *c,
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
/* Return: Success => 0
 *         Failure => a negative value */
	int8_t
container_set_clone(
	container_t *dst,
	const container_t *src,
	bool free_old_data
);

/* Get the contained data */
/*    c : pointer to the container */
/* copy : true => return a pointer to a copy of the data
 *        false => return a direct pointer to the data */
/* Return: pointer to the data */
	void *
container_get(
	container_t *c,
	bool copy
);

/* Get size in bytes of contained data */
/* c : pointer to the container */
/* Return: Success => the size in bytes of contained data
 *         Failure => 0 */
	size_t
container_data_size(
	const container_t *c
);

/* Free the memory */
/*         c : pointer to the container
 * free_data : true => try to free the data with "free" registered function
 *                     or with standard free() function
 *             false => don't free memory occupied by the data */
/* Return: nothing */
	void
container_free(
	container_t *c,
	bool free_data
);

#ifdef __cplusplus
}
#endif

#endif /* Not container_h_included */

