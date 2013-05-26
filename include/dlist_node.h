/*
 * Copyright 2010-2013 Julian Maurice
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

/*
 * File              : dlist_node.h
 * Short description : Double linked list node
 */

#ifndef dlist_node_h_included
#define dlist_node_h_included

#include <stdint.h>
#include "inline/dlist.h"
#include "callbacks.h"

typedef struct {
	void *data;
	gds_inline_dlist_node_t inline_dlist_node;
} gds_dlist_node_t;

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
gds_dlist_node_t *
gds_dlist_node_new(
	void *data
);

/*
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
gds_dlist_node_set_data(
	gds_dlist_node_t *node,
	void *data,
	void *callback,
	void *callback_data
);

/*
 * Get the node data
 *
 * Parameters:
 *     node: pointer to the node
 *
 * Returns:
 * pointer to the data */
void *
gds_dlist_node_get_data(
	gds_dlist_node_t *node
);

gds_inline_dlist_node_t *
gds_dlist_node_get_inline(
	gds_dlist_node_t *node
);

gds_dlist_node_t *
gds_dlist_node_get_container_of(
	gds_inline_dlist_node_t *node_inline
);

gds_dlist_node_t *
gds_dlist_node_copy(
	gds_dlist_node_t *node
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
gds_dlist_node_free(
	gds_dlist_node_t *node,
	void *callback,
	void *callback_data
);

#ifdef __cplusplus
}
#endif

#endif /* Not dlist_node_h_included */

