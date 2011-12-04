/*****************************************************************************
 * Copyright (C) 2010-2011 Julian Maurice                                    *
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
 * File                 : types.h                                            *
 * Short description    : Custom types management                            *
 *****************************************************************************
 * Custom types are user-defined types. They are described by a unique name, *
 * a size in bytes and have a list of functions identified by their name.    *
 *****************************************************************************/

#ifndef types_h_included
#define types_h_included

#include <stdint.h>
#include <stdbool.h>
#include "funcs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* WARNING: This function has to be called before the others functions.
 * Initialize the custom types system. It's implemented with a hash table */
/* size : the width of the hash table,
 *        0 if you don't care (set to default 1024) */
/* Return: Success => 0
 *         Failure => a negative value */
	int8_t
types_init(
	uint32_t size
);

/* Check if custom types are initialized */
/* Return: true => custom types are initialized
 *         false => custom types are NOT initialized */
	bool
types_initialized(void);

/* Check if a type exists */
/* name : Type name to check */
/* Return: true => exists
 *         false => does not exist */
	bool
type_exist(
	const char *name
);

/* Register a new type. */
/* name : Name of the type to register
 * size : Size of the type */
/* Return: 0 on success,
 *         a negative value if an error occurs,
 *         a positive value if type already exists. */
	int8_t
type_reg(
	const char *name,
	size_t size
);

/* Add a function to type's function list. */
/*      name : Type name
 * func_name : Name of the function to register
 *  func_ptr : pointer to the function */
/* Return: 0 on success,
 *         a negative value if an error occurs */
	int8_t
type_reg_func(
	const char *name,
	const char *func_name,
	func_ptr_t func_ptr
);

/* Get function pointer */
/*      name : Type name
 * func_name : Function name */
/* Return: Success => a pointer to the function
 *         Failure => NULL */
	func_ptr_t
type_get_func(
	const char *name,
	const char *func_name
);

/* Unregister an existing type */
/* name : Type name to unregister */
/* Return: Success => 0,
 *         Failure => a negative value */
	int8_t
type_unreg(
	const char *name
);

/* Remove a function from type's function list */
/* name : Type name
 * func_name : Function name */
/* Return: Success => 0,
 *         Failure => a negative value */
	int8_t
type_unreg_func(
	const char *name,
	const char *func_name
);

/* Get type size */
/* name : Type name */
/* Return: Success => size of the type,
 *         Failure => 0 */
	size_t
type_sizeof(
	const char *name
);

/* Free memory */
	void
types_free(void);

/* For debugging purposes */
	void
types_print(void);

#ifdef __cplusplus
}
#endif

#endif /* Not types_h_included */

