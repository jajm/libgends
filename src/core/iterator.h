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
 * File                 : iterator.h                                         *
 * Short description    : Iterators management                               *
 *****************************************************************************
 * To create an iterator on a custom type, you have to:                      *
 *  - create a custom type (see types.h)                                     *
 *  - create 4 functions for this type :                                     *
 *    . void *first(void *container)  // return a pointer on 1st element     *
 *    . void *next(void *container, void *pointer) // return a pointer on    *
 *                                                 // the next element       *
 *    . void *get(void *container, void *pointer) // return a pointer on data*
 *    . bool has_next(void *container, void *pointer) // return 1 if it      *
 *                                          // remains elements, 0 otherwise *
 *  - and then, create the iterator, with it_new.                            *
 *****************************************************************************/

#ifndef iterator_h_included
#define iterator_h_included

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	char *type_name;    /* Type on what we'll iterate */
	void *container;    /* Container on which we'll iterate */
	void *pointer;      /* Current position of iterator */
} iterator_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new iterator. You will have to use it_reset before to use it
 * Return a pointer on the new iterator, or NULL if an error occurs */
iterator_t *it_new(const char *type_name, void *container);
/* (Re)set iterator to point on the first element
 * Return 0 on success, or a negative value if an error occurs */
int8_t it_reset(iterator_t *it);
/* Set iterator to point on the next element
 * Return 0 on success, or a negative value if an error occurs */
int8_t it_next(iterator_t *it);
/* Get data pointed by iterator */
/* Return NULL if an error occurs */
void *it_get(iterator_t *it);
/* Get iterator status
 * Return 1 if it remains elements, 0 otherwise */
bool it_has_next(iterator_t *it);
/* Free memory */
void it_free(iterator_t *it);

#ifdef __cplusplus
}
#endif

#endif /* Not iterator_h_included */

