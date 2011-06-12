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
 * File                 : types.h                                            *
 * Short description    : Custom types management                            *
 * Author               : Julian Maurice                                     *
 * Created on           : 2010-03-01                                         *
 *****************************************************************************
 * Custom types are user-defined types. They are described by a unique name, *
 * a size in bytes and have a list of functions identified by their name.    *
 *****************************************************************************/

#ifndef types_h_included
#define types_h_included

#include "basic_types.h"
#include "funcs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* WARNING: This function has to be called before the others functions.
 * Initialize the custom types hash table. size is the width of hash table.
 * Return 0 on success, or a negative value if an error occurs. */
s8 types_init(u32 size);
/* Register a new type.
 * Return 0 on success,
 *        a negative value if an error occurs,
 *        or a positive value if type already exists. */
s8 type_reg(const char *name, u32 size);
/* Add a function to type's function list.
 * Return 0 on success, or a negative value if an error occurs */
s8 type_reg_func(const char *name, const char *func_name, func_ptr_t func_ptr);
/* Get function pointer
 * Return a pointer of 'name' named function if it exists, NULL otherwise */
func_ptr_t type_get_func(const char *name, const char *func_name);
/* Unregister an existing type
 * Return 0 on success, or a negative value if an error occurs */
s8 type_unreg(const char *name);
/* Remove a function from type's function list
 * Return 0 on success, or a negative value if an error occurs */
s8 type_unreg_func(const char *name, const char *func_name);
/* Get type size */
/* Return the size of 'name' named type, or 0 if an error occurs */
u32 type_sizeof(const char *name);

/* Destroy all types */
void types_free(void);

/* For debugging purposes */
void types_print(void);

#ifdef __cplusplus
}
#endif

#endif /* Not types_h_included */

