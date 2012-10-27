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
#include "callbacks.h"

typedef struct gds_slist_node_s{
	void *data;
	struct gds_slist_node_s *next;
} gds_slist_node_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new node */
/*     data : pointer to the data
 * alloc_cb : function that take a pointer and returns a copy of pointed
 *            data. The copy is stored in the node. NULL to disable copy */
/* Return: Success => pointer to the newly created node
 *         Failure => NULL */
gds_slist_node_t *
gds_slist_node_new(
	void *data,
	gds_alloc_cb alloc_cb
);

/* Set a new value to an existing node */
/*     node : pointer to the node
 *     data : pointer to the data
 *  free_cb : function that takes a pointer and free pointed data.
 *            NULL to disable freeing old data
 * alloc_cb : function that takes a pointer and returns a copy of pointed
 *            data. The copy is stored in the node. NULL to disable copy */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_slist_node_set_data(
	gds_slist_node_t *node,
	void *data,
	gds_free_cb free_cb,
	gds_alloc_cb alloc_cb
);

/* Get the node data */
/*     node : pointer to the node
 * alloc_cb : function that takes a pointer and returns a copy of pointed
 *             data. The copy is returned. NULL to disable copy */
/* Return: pointer to the data */
void *
gds_slist_node_get_data(
	gds_slist_node_t *node,
	gds_alloc_cb alloc_cb
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
/*    node : pointer to the node
 * free_cb : function that takes a pointer and free pointed data.
 *           NULL to disable freeing data */
void
gds_slist_node_free(
	gds_slist_node_t *node,
	gds_free_cb free_cb
);

#ifdef __cplusplus
}
#endif

#endif /* Not slist_node_h_included */

