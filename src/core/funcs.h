/*****************************************************************************
 * Copyright (C) 2010 Julian Maurice                                         *
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
 * File                 : funcs.h                                            *
 * Short description    : Linked list of generic functions                   *
 * Author               : Julian Maurice                                     *
 * Created on           : 2010-03-13                                         *
 *****************************************************************************
 * Linked list of pointer of intptr_t (*ptr)(void *, ...)                    *
 *****************************************************************************/

#ifndef funcs_h_included
#define funcs_h_included

#include <stdint.h>
#include "basic_types.h"

typedef intptr_t (*func_ptr_t)(void *, ...);

typedef struct {
	char *name;	/* Function name (it will be its identifier) */
	func_ptr_t ptr;	/* Function pointer */
} func_t;

typedef struct func_list_node_s {
	func_t *func;
	struct func_list_node_s *next;
} func_list_node_t, *funcs_t;

#ifdef __cplusplus
extern "C" {
#endif

/* All functions that can return an error (NULL, or a negative value) use
 * error.h functions to store an error message.
 * You can use pError to print this error message.
 */

/* Create new function structure
 * Return a pointer on newly created structure, or NULL if error occurs */
func_t *func_new(const char *func_name, func_ptr_t func_ptr);
/* Get function name
 * Return a string which contains the name of the function, or NULL if an error
 * occurs. */
char *func_get_name(func_t *func);
/* Get function pointer */
/* Return a function pointer, or NULL if an error occurs */
func_ptr_t func_get_ptr(func_t *func);
/* Free memory */
void func_free(func_t *func);

/* Add a function to the list
 * Return a pointer on the newly created node, or NULL if an error occurs */
func_list_node_t *funcs_add(funcs_t *funcs, func_t *func);
/* Remove a function from the list. Search is done using name.
 * Return 0 on succes, a negative value otherwise. */
s8 funcs_del(funcs_t *head, const char *name);
/* Free memory */
void funcs_free(funcs_t head);
/* Get function structure
 * Return a pointer on function structure, or NULL if an error occurs */
func_t *funcs_get(funcs_t head, const char *name);
/* Get function pointer */
/* Return function pointer, or NULL if an error occurs */
func_ptr_t funcs_get_ptr(funcs_t funcs, const char *name);

/* Call 'name' named function, if it exists in 'funcs' list, with
 * following arguments as parameters.
 * Return return value of called function.
 * WARNING: don't use this macro if you're not sure the function exists */
#define funcs_call(funcs, name, arg1, ...) 	\
		(funcs_get_ptr(funcs, name))(arg1, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* Not funcs_h_included */
