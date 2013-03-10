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
#include "inline/slist.h"
#include "callbacks.h"

typedef struct {
	void *data;
	gds_inline_slist_node_t slist_node_inline;
} gds_slist_node_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a new node
 *
 * Parameters
 *   data : pointer to the data
 *
 * Return
 *   Success => pointer to the newly created node
 *   Failure => NULL
 */
gds_slist_node_t *
gds_slist_node_new(
	void *data
);

/**
 * Set a new value to an existing node
 *
 * Parameters:
 *   node          : pointer to the node
 *   data          : pointer to the data
 *   callback      : Function called on data before setting the new data.
 *                   Can be NULL. Prototype is void callback(void *, void *)
 *   callback_data : Data passed to callback as 2nd parameter.
 *
 * Return:
 *   Success => 0
 *   Failure => a negative value
 */
int8_t
gds_slist_node_set_data(
	gds_slist_node_t *node,
	void *data,
	void *callback,
	void *callback_data
);

/* Get the node data */
/*     node : pointer to the node */
/* Return: pointer to the data */
void *
gds_slist_node_get_data(
	gds_slist_node_t *node
);

gds_inline_slist_node_t *
gds_slist_node_get_inline(
	gds_slist_node_t *node
);

gds_slist_node_t *
gds_slist_node_get_container_of(
	gds_inline_slist_node_t *node_inline
);

gds_slist_node_t *
gds_slist_node_copy(
	gds_slist_node_t *node
);

/**
 * Free a node
 *
 * Parameters:
 *   node          : Pointer to the node.
 *   callback      : Function called on data before freeing the node.
 *                   Can be NULL. Prototype is void callback(void *, void *)
 *   callback_data : Data passed to callback as 2nd parameter.
 */
void
gds_slist_node_free(
	gds_slist_node_t *node,
	void *callback,
	void *callback_data
);

#ifdef __cplusplus
}
#endif

#endif /* Not slist_node_h_included */

