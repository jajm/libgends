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
 * File              : slist_node.h                                          *
 * Short description : Singly linked list node                               *
 *****************************************************************************/

#ifndef slist_node_h_included
#define slist_node_h_included

#include <stdint.h>
#include <stdbool.h>
#include "func_ptr.h"

typedef struct gds_slist_node_s{
	void *data;
	struct gds_slist_node_s *next;
} gds_slist_node_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new node */
/*      data : pointer to the data
 * copy_data : true => copy the data
 *             false => just take the pointer value
 *   alloc_f : function that take a pointer and returns a copy of pointed
 *             data. Unused if copy_data is false */
/* Return: Success => pointer to the newly created node
 *         Failure => NULL */
gds_slist_node_t *
gds_slist_node_new(
	void *data,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

/* Set a new value to an existing node */
/*          node : pointer to the node
 *          data : pointer to the data
 * free_old_data : true => free memory taken by old value
 *                 false => don't free memory
 *        free_f : function that takes a pointer and free pointed data and
 *                 returns nothing. Must not be NULL if free_old_data is true
 *                 Unused if free_old_data is false
 *     copy_data : true => copy the data
 *                 false => just take the pointer value
 *       alloc_f : function that takes a pointer and returns a copy of pointed
 *                 data. Unused if copy_data is false */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_slist_node_set_data(
	gds_slist_node_t *node,
	void *data,
	bool free_old_data,
	gds_func_ptr_t free_f,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

/* Get the node data */
/*      node : pointer to the node
 *      copy : true => return a pointer to a copy of the data
 *             false => return a direct pointer to the data
 *      alloc_f : function that takes a pointer and returns a copy of pointed
 *                 data. Unused if copy is false */
/* Return: pointer to the data */
void *
gds_slist_node_get_data(
	gds_slist_node_t *node,
	bool copy,
	gds_func_ptr_t alloc_f
);

/* Set next node */
/* node : pointer to the node
 * next : pointer to the future next node */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_slist_node_set_next(
	gds_slist_node_t *node,
	gds_slist_node_t *next
);

/* Get the next node */
/* node : pointer to the node */
/* Return: pointer to the next node */
gds_slist_node_t *
gds_slist_node_get_next(
	gds_slist_node_t *node
);

/* Free a node */
/*      node : pointer to the node
 * free_data : true => free data
 *             false => don't free memory occupied by data
 *    free_f : function that takes a pointer and free pointed data and
 *             returns nothing. Must not be NULL if free_old_data is true
 *             Unused if free_old_data is false */
void
gds_slist_node_free(
	gds_slist_node_t *node,
	bool free_data,
	gds_func_ptr_t free_f
);

#ifdef __cplusplus
}
#endif

#endif /* Not slist_node_h_included */

