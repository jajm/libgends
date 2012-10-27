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
 * File              : typed_stack.h                                         *
 * Short description : Stack management (LIFO, Last In First Out)            *
 *****************************************************************************/

#ifndef gds_typed_stack_h_included
#define gds_typed_stack_h_included

#include <stdbool.h>
#include <stdint.h>
#include "slist_node.h"

typedef struct{
	char *type_name;
	gds_slist_node_t *head;
} gds_typed_stack_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new stack */
/* type_name : type name of stored data (see types.h) */
/* Return: Success => pointer to the newly created stack
 *         Failure => NULL */
	gds_typed_stack_t *
gds_typed_stack_new(
	const char *type_name
);

/* Push into the stack */
/*         S : pointer to the stack
 *      data : data to push
 * copy_data : true => make a copy of the data
 *             false => just take the pointer value */
/* Return: Success => 0
 *         Failure => a negative value */
	int8_t
gds_typed_stack_push(
	gds_typed_stack_t *S,
	void *data,
	bool copy_data
);

/* Pop from the stack */
/* S : pointer to the stack */
/* Return: Success => pointer to the data
 *         Failure => NULL */
	void *
gds_typed_stack_pop(
	gds_typed_stack_t *S
);

/* Free memory */
/* S : pointer to the stack
 * free_data : true => free memory occupied by data (use "free" custom
 *                     function)
 *             false => don't free memory occupied by data */
	void
gds_typed_stack_free(
	gds_typed_stack_t *S,
	bool free_data
);

#ifdef __cplusplus
}
#endif

#endif /* Not gds_typed_stack_h_included */

