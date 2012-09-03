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
 * File              : typed_slist.h                                         *
 * Short description : Generic singly linked list management                 *
 *****************************************************************************
 * A generic singly linked list can contain whatever type of data, since a   *
 * corresponding custom types exists (see core/types.h).                     *
 *****************************************************************************/

/*****************************************************************************
 * Custom functions used:                                                    *
 * - alloc:                                                                  *
 *   Prototype: void * alloc(void *data);                                    *
 *   Takes a pointer to data in parameter and should return a pointer to a   *
 *   copy of data.                                                           *
 *   Used in gds_typed_slist_add, gds_typed_slist_add_*,                     *
 *   gds_typed_slist_it_add, gds_typed_slist_get_* and gds_typed_slist_it_get*
 *   if parameter copy_data is true.                                         *
 * - free:                                                                   *
 *   Prototype: void free(void *data);                                       *
 *   Takes a pointer to data and free it.                                    *
 *   Used in gds_typed_slist_del, gds_typed_slist_del_*,                     *
 *   gds_typed_slist_it_del and  gds_typed_slist_free if parameter free_data *
 *   is true.                                                                *
 * - cmp:                                                                    *
 *   Prototype: int32_t cmp(void *data1, void *data2);                       *
 *   Compare two data, should return 0 if data are equals, a positive value  *
 *   if data1 is greater than data2, or a negative value if data1 is lesser  *
 *   than data2.                                                             *
 *   Used in gds_typed_slist_chk.                                            *
 *****************************************************************************/

#ifndef gds_typed_slist_h_included
#define gds_typed_slist_h_included

#include <stdint.h>
#include <stdbool.h>
#include "slist_node.h"
#include "core/iterator.h"

typedef struct{
	char *type_name;
	gds_slist_node_t *first;
	gds_slist_node_t *last;
} gds_typed_slist_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new list */
/* type_name : name of data type (see core/types.h) */
/* Return: a pointer to the newly created list */
gds_typed_slist_t *
gds_typed_slist_new(
	const char *type_name
);

/* Check if list is empty */
/* l : pointer to the list */
/* Return: true => the list is empty
 *         false => not empty */
bool
gds_typed_slist_empty(
	gds_typed_slist_t *l
);

/* Get pointer to the first node of the list */
/* l : pointer to the list */
/* Return: Success => a pointer to the first node of the list
 *         Failure => NULL */
gds_slist_node_t *
gds_typed_slist_first(
	gds_typed_slist_t *l
);

/* Get pointer to the next node */
/*    l : pointer to the list
 * node : pointer to a node in the list */
/* Return: a pointer to the following node */
gds_slist_node_t *
gds_typed_slist_next(
	gds_typed_slist_t *l,
	gds_slist_node_t *node
);

/* Check if node is followed by another node */
/*    l : pointer to the list
 * node : pointer to a node in the list */
/* Return: true => node is followed by another node
 *         false => node is the last node of the list */
bool
gds_typed_slist_has_next(
	gds_typed_slist_t *l,
	gds_slist_node_t *node
);


/* ========================================================================= */
/*                              'Add' functions                              */
/* ========================================================================= */


/* Add a node to the list, just after a given node */
/*         l : pointer to the list
 *      node : pointer to a node in the list. The new node will be added after
 *             it. If NULL, the new node is added in first position.
 *      data : pointer to the data
 * copy_data : true => make a copy of the data
 *             false => just take the pointer value */
/* Return: Success => pointer to the newly created node
 *         Failure => NULL */
gds_slist_node_t *
gds_typed_slist_add(
	gds_typed_slist_t *l,
	gds_slist_node_t *node,
	void *data,
	bool copy_data
);

/* Add a new node at beginning of the list */
/*         l : pointer to the list
 *      data : pointer to the data
 * copy_data : true => make a copy of the data
 *             false => just take the pointer value */
/* Return: Success => pointer to the newly created node
 *         Failure => NULL */
gds_slist_node_t *
gds_typed_slist_add_first(
	gds_typed_slist_t *l,
	void *data,
	bool copy_data
);

/* Add a new node at end of the list */
/*         l : pointer to the list
 *      data : pointer to the data
 * copy_data : true => make a copy of the data
 *             false => just take the pointer value */
/* Return: Success => pointer to the newly created node
 *         Failure => NULL */
gds_slist_node_t *
gds_typed_slist_add_last(
	gds_typed_slist_t *l,
	void *data,
	bool copy_data
);

/* Add a new node after the node pointed by the iterator */
/*        it : pointer to the iterator
 *      data : pointer to the data
 * copy_data : true => make a copy of the data
 *             false => just take the pointer value */
/* Return: Success => pointer to the newly created node
 *         Failure => NULL */
gds_slist_node_t *
gds_typed_slist_it_add(
	gds_iterator_t *it,
	void *data,
	bool copy_data
);


/* ========================================================================= */
/*                            'Remove' functions                             */
/* ========================================================================= */


/* ---------- Remove a node and returns previously contained data ---------- */
/* Remove given node in the list */
/*    l : pointer to the list
 * node : pointer to the node to remove */
/* Return: Success => pointer to previously contained data
 *         Failure => NULL */
void *
gds_typed_slist_pop(
	gds_typed_slist_t *l,
	gds_slist_node_t *node
);

/* Remove the first node of the list */
/* l : pointer to the list */
/* Return: Success => pointer to previously contained data
 *         Failure => NULL */
void *
gds_typed_slist_pop_first(
	gds_typed_slist_t *l
);

/* Remove the last node of the list */
/* l : pointer to the list */
/* Return: Success => pointer to previously contained data
 *         Failure => NULL */
void *
gds_typed_slist_pop_last(
	gds_typed_slist_t *l
);

/* Remove node pointed by iterator and move iterator to the next node */
/* it : pointer to iterator */
/* Return: Success => pointer to previously contained data
 *         Failure => NULL */
void *
gds_typed_slist_it_pop(
	gds_iterator_t *it
);


/* ----------------- Remove a node without returning data ------------------ */
/* Remove given node from the list */
/*         l : pointer to the list
 *      node : pointer to the node to remove
 * free_data : true => free memory occupied by data (use "free" custom
 *                     function)
 *             false => don't free the memory */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_typed_slist_del(
	gds_typed_slist_t *l,
	gds_slist_node_t *node,
	bool free_data
);

/* Remove the first node of the list */
/*         l : pointer to the list
 * free_data : true => free memory occupied by data (use "free" custom
 *                     function)
 *             false => don't free the memory */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_typed_slist_del_first(
	gds_typed_slist_t *l,
	bool free_data
);

/* Remove the last node of the list */
/*         l : pointer to the list
 * free_data : true => free memory occupied by data (use "free" custom
 *                     function)
 *             false => don't free the memory */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_typed_slist_del_last(
	gds_typed_slist_t *l,
	bool free_data
);

/* Remove node pointed by iterator and move iterator to the next node */
/* it : pointer to iterator
 * free_data : true => free memory occupied by data (use "free" custom
 *                     function)
 *             false => don't free the memory */
/* Return: Success => 0
 *         Failure => a negative value */
int8_t
gds_typed_slist_it_del(
	gds_iterator_t *it,
	bool free_data
);

/* ========================================================================= */
/*                           'Get' functions                                 */
/* ========================================================================= */
/* Get the data of given node */
/*         l : pointer to the list
 *      node : pointer to the node
 * copy_data : true => returns a pointer to a copy of the data
 *             false => returns a pointer to the real data */
/* Return: Success => pointer to the data
 *          Failure => NULL */
void *
gds_typed_slist_get(
	gds_typed_slist_t *l,
	gds_slist_node_t *node,
	bool copy_data
);

/* Get the data of first node */
/*         l : pointer to the list
 * copy_data : true => returns a pointer to a copy of the data
 *             false => returns a pointer to the real data */
/* Return: Success => pointer to the data
 *          Failure => NULL */
void *
gds_typed_slist_get_first(
	gds_typed_slist_t *l,
	bool copy_data
);

/* Get the data of last node */
/*         l : pointer to the list
 * copy_data : true => returns a pointer to a copy of the data
 *             false => returns a pointer to the real data */
/* Return: Success => pointer to the data
 *          Failure => NULL */
void *
gds_typed_slist_get_last(
	gds_typed_slist_t *l,
	bool copy_data
);

/* Get the data of the node pointed by iterator */
/*        it : pointer to the iterator
 * copy_data : true => returns a pointer to a copy of the data
 *             false => returns a pointer to the real data */
/* Return: Success => pointer to the data
 *          Failure => NULL */
void *
gds_typed_slist_it_get(
	gds_iterator_t *it,
	bool copy_data
);

/* ========================================================================= */
/*                                Iterators                                  */
/* ========================================================================= */

/* Create a new iterator */
/* l : pointer to the list */
/* Return: Success => a pointer to the newly created iterator
 *          Failure => NULL */
gds_iterator_t *
gds_typed_slist_iterator_new(
	gds_typed_slist_t *l
);

/* Free iterator */
/* it : pointer to the iterator */
void
gds_typed_slist_iterator_free(
	gds_iterator_t *it
);


/* ========================================================================= */
/*                             Others functions                              */
/* ========================================================================= */

/* Check if data is in the list
 * (use "cmp" custom function if defined, memcmp otherwise) */
/*    l : pointer to the list
 * data : pointer to the data */
/* Return: Success => pointer to the node which contain searched data
 *         Failure => NULL */
gds_slist_node_t *
gds_typed_slist_chk(
	gds_typed_slist_t *l,
	void *data
);

/* Free memory occupied by the list */
/*         l : pointer to the list
 * free_data : true => free memory occupied by data (use "free" custom
 *                     function)
 *             false => don't free memory occupied by data */
void
gds_typed_slist_free(
	gds_typed_slist_t *l,
	bool free_data
);

#ifdef __cplusplus
}
#endif

#endif /* Not gds_typed_slist_h_included */

