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
 * File                 : func_list.h                                        *
 * Short description    : Linked list of generic functions                   *
 *****************************************************************************
 * Linked list of pointer of intptr_t (*ptr)(void *, ...)                    *
 *****************************************************************************/

#ifndef func_list_h_included
#define func_list_h_included

#include <stdint.h>
#include "func_ptr.h"

typedef struct gds_func_list_node_s {
	char *name;          /* Function name (it will be its identifier) */
	gds_func_ptr_t ptr;  /* Function pointer */
	struct gds_func_list_node_s *next;
} gds_func_list_node_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Add a function to the list
 * Return a pointer on the newly created node, or NULL if an error occurs */
gds_func_list_node_t *
gds_func_list_add(
	gds_func_list_node_t **head,
	const char *name,
	gds_func_ptr_t ptr
);

/* Remove a function from the list. Search is done using name.
 * Return 0 on succes, a negative value otherwise. */
int8_t
gds_func_list_del(
	gds_func_list_node_t **head,
	const char *name
);

/* Free memory */
void
gds_func_list_free(
	gds_func_list_node_t *head
);

/* Get function pointer */
/* Return function pointer, or NULL if an error occurs */
gds_func_ptr_t
gds_func_list_get_ptr(
	gds_func_list_node_t *head,
	const char *name
);

#ifdef __cplusplus
}
#endif

#endif /* Not func_list_h_included */
