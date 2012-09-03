/*****************************************************************************
 * Copyright (C) 2012 Julian Maurice                                         *
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
 * File              : slist.h                                               *
 * Short description : Low-level singly linked list                          *
 *****************************************************************************/

#ifndef gds_slist_h_included
#define gds_slist_h_included

#include "core/callbacks.h"
#include "core/iterator.h"
#include "slist_node.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Add new data at beginning of list */
/* head     : pointer to first node of list
 * data     : pointer to data
 * alloc_cb : callback to allocation function, if you want the data to be
 *            copied. NULL otherwise. */
/* Return : pointer to the newly created node, or NULL if operation failed */
gds_slist_node_t *
gds_slist_add_first(
	gds_slist_node_t *head,
	void *data,
	gds_alloc_cb alloc_cb
);

/* Add new data at end of list */
/* head     : pointer to first node of list
 * data     : pointer to data
 * alloc_cb : callback to allocation function, if you want the data to be
 *            copied. NULL otherwise. */
/* Return : pointer to the newly created node, or NULL if operation failed */
gds_slist_node_t *
gds_slist_add_last(
	gds_slist_node_t *head,
	void *data,
	gds_alloc_cb alloc_cb
);

/* Add new data after a given node */
/* node     : pointer to the node that will precede the new node
 * data     : pointer to data
 * alloc_cb : callback to allocation function, if you want the data to be
 *            copied. NULL otherwise. */
/* Return : pointer to the newly created node, or NULL if operation failed */
gds_slist_node_t *
gds_slist_add_after(
	gds_slist_node_t *node,
	void *data,
	gds_alloc_cb alloc_cb
);

void
gds_slist_add_list_first(
	gds_slist_node_t **head,
	gds_slist_node_t *list
);

void
gds_slist_add_list_last(
	gds_slist_node_t **head,
	gds_slist_node_t *list
);

void
gds_slist_add_list_after(
	gds_slist_node_t *node,
	gds_slist_node_t *list
);

/* Get last node of list */
/* head : pointer to first node of list */
/* Return : pointer to last node of list, or NULL if list is empty */
gds_slist_node_t *
gds_slist_get_last(
	gds_slist_node_t *head
);

/* Delete first node of list */
/* head    : pointer to first node of list */
/* free_cb : callback to free function, if you want the data to be freed.
 *           NULL otherwise. */
/* Return : pointer to first node of list (after deletion). Can return NULL if
 *          it removed the last node or if list was empty */
gds_slist_node_t *
gds_slist_del_first(
	gds_slist_node_t *head,
	gds_free_cb free_cb
);

/* Delete last node of list */
/* head    : pointer to first node of list
 * free_cb : callback to free function, if you want the data to be freed.
 *           NULL otherwise. */
/* Return : pointer to first node of list (after deletion). Can return NULL if
 *          it removed the last node or if list was empty */
gds_slist_node_t *
gds_slist_del_last(
	gds_slist_node_t *head,
	gds_free_cb free_cb
);

/* Delete node after a given node */
/* node    : pointer to the node that precedes the node you want to remove
 * free_cb : callback to free function, if you want the data to be freed.
 *           NULL otherwise. */
/* Return : a negative value if it was not able to remove the node.
 *          a positive value if node is the last node.
 *          0 otherwise. */
int8_t
gds_slist_del_after(
	gds_slist_node_t *node,
	gds_free_cb free_cb
);

/* Create an iterator on list */
/* head : pointer to first node of list */
/* Return : pointer to iterator, or NULL if an error occured */
gds_iterator_t *
gds_slist_iterator_new(
	gds_slist_node_t *head
);

/* Free iterator */
/* it: pointer to iterator */
void
gds_slist_iterator_free(
	gds_iterator_t *it
);

/* Free list */
/* head    : pointer to first node of list */
/* free_cb : callback to free function, if you want the data to be freed.
 *           NULL otherwise. */
void
gds_slist_free(
	gds_slist_node_t *head,
	gds_free_cb free_cb
);

#ifdef __cplusplus
}
#endif

#endif /* Not gds_slist_h_included */

