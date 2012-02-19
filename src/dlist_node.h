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
 * File              : dlist_node.h                                          *
 * Short description : Double linked list node management                    *
 *****************************************************************************/

#ifndef dlist_node_h_included
#define dlist_node_h_included

#include <stdbool.h>
#include <stdint.h>
#include "func_ptr.h"

typedef struct gds_dlist_node_s {
	void *data;
	struct gds_dlist_node_s *prev;
	struct gds_dlist_node_s *next;
} gds_dlist_node_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new node */
/*      data : pointer to the data
 * copy_data : true => make a copy of the data,
 *             false => just take the pointer value
 *   alloc_f : function that takes a pointer and returns a copy of pointed
 *             data. Unused if copy_data is false */
/* Return: Success => pointer to the newly created node
 *         Failure => NULL*/
	gds_dlist_node_t *
gds_dlist_node_new(
	void *data,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

/* Set a new value to an existing node */
/*          node : pointer to the node
 *          data : pointer to the data
 * free_old_data : true => free memory taken by old value
 *                 false => don't free memory
 *        free_f : pointer to a free function, can be NULL. In this case and
 *                 if free_old_data is true, call standard free() function.
 *     copy_data : true => copy the data
 *                 false => just take the pointer value
 *       alloc_f : function that takes a pointer and return a copy of pointed
 *                 data. Unused if copy_data is false */
/* Return: Success => 0
 *         Failure => a negative value */
	int8_t
gds_dlist_node_set_data(
	gds_dlist_node_t *node,
	void *data,
	bool free_old_data,
	gds_func_ptr_t free_f,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

/* Get the node data */
/*      node : pointer to the node
 * copy_data : true => return a pointer to a copy of the data
 *             false => return a direct pointer to the data
 *   alloc_f : function that takes a pointer and returns a copy of pointed
 *             data. Unused if copy_data is false */
/* Return: Success => pointer to the data
 *         Failure => NULL */
	void *
gds_dlist_node_get_data(
	gds_dlist_node_t *node,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

/* Set next node */
/* node : pointer to the node
 * next : pointer to the future next node */
/* Return: Success => 0
 *         Failure => a negative value */
	int8_t
gds_dlist_node_set_next(
	gds_dlist_node_t *node,
	gds_dlist_node_t *next
);

/* Get next node */
/* node : pointer to the node */
/* Return: pointer to the next node */
	gds_dlist_node_t *
gds_dlist_node_get_next(
	gds_dlist_node_t *node
);

/* Set previous node */
/* node : pointer to the node
 * prev : pointer to the future previous node */
/* Return: Success => 0
 *         Failure => a negative value */
	int8_t
gds_dlist_node_set_prev(
	gds_dlist_node_t *node,
	gds_dlist_node_t *prev
);

/* Get previous node */
/* node : pointer to the node */
/* Return: pointer to the previous node */
	gds_dlist_node_t *
gds_dlist_node_get_prev(
	gds_dlist_node_t *node
);

/* Free a node */
/*      node : pointer to the node
 * free_data : true => free data
 *             false => don't free memory occupied by data
 *    free_f : pointer to a free function, can be NULL. In this case and if
 *             free_data is true, standard free() function is called. */
	void
gds_dlist_node_free(
	gds_dlist_node_t *node,
	bool free_data,
	gds_func_ptr_t free_f
);

#ifdef __cplusplus
}
#endif

#endif /* Not dlist_node_h_included */
