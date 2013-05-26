/*
 * Copyright (C) 2012 Julian Maurice
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
 * File              : slist.h
 * Short description : singly linked list
 */

#ifndef gds_slist_h_included
#define gds_slist_h_included

#include "undefined.h"
#include "iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gds_slist_s gds_slist_t;

/*
 * Create a new list
 *
 * Returns:
 *   pointer to the new list
 */
gds_slist_t *
gds_slist_new(void);

/*
 * Create a new list from a data array
 *
 * Parameters:
 *   n: size of data array
 *   data: data array
 *
 * Returns:
 *   pointer to the new list
 */
gds_slist_t *
gds_slist_new_from_array(
	unsigned int n,
	void *data[]
);

/*
 * Create a new list from the list of parameters
 *
 * Extra parameters:
 *   (void *) data to add to the list
 *
 * Examples:
 *   // Creates an empty list
 *   gds_slist_t *list = gds_slist();
 *
 *   // Creates a list with 3 elements
 *   void *a, *b, *c;
 *   gds_slist_t *list = gds_slist(a, b, c);
 *
 * Returns:
 *   (gds_slist_t *) pointer to the new list
 */

#define gds_slist(...) ({                                                     \
	void *gds_va_args[] = {__VA_ARGS__};                                  \
	gds_slist_new_from_array(sizeof(gds_va_args) / sizeof(*gds_va_args),  \
		gds_va_args);                                                 \
})

/*
 * Add new data at beginning of list
 *
 * Parameters:
 *   list: pointer to list
 *   data: pointer to data
 *
 * Returns:
 *   0 in case of success
 *   a negative value otherwise
 */
int
gds_slist_unshift(
	gds_slist_t *list,
	void *data
);

/*
 * Add new data at end of list
 *
 * Parameters:
 *   list: pointer to list
 *   data: pointer to data
 *
 * Returns:
 *   0 in case of success
 *   a negative value otherwise
 */
int
gds_slist_push(
	gds_slist_t *list,
	void *data
);

/*
 * Remove the first element of list and return it
 *
 * Parameters:
 *   list: pointer to list
 *
 * Returns:
 *   data that was just removed from list
 *   UNDEFINED if list is empty
 */
void *
gds_slist_shift(
	gds_slist_t *list
);

/*
 * Remove the last element of list and return it
 *
 * Parameters:
 *   list: pointer to list
 *
 * Returns:
 *   data that was just removed from list
 *   UNDEFINED if list is empty
 */
void *
gds_slist_pop(
	gds_slist_t *list
);

/*
 * Get a value from the list.
 *
 * Parameters:
 *   list: pointer to the list
 *   offset: offset of value to retrieve
 *
 * Returns:
 *   data contained in node at the given offset
 *   UNDEFINED if offset is greater or equal to list size
 *
 * Hints:
 *   To get the last element, use gds_slist_size:
 *     void *last = gds_slist_get(list, gds_slist_size(list)-1);
 */
void *
gds_slist_get(
	gds_slist_t *list,
	unsigned int offset
);

/*
 * Remove a portion of list and replace it by another list.
 *
 * Parameters:
 *   list: Pointer to the list.
 *   offset: Offset of the first node to remove.
 *   length: Number of nodes to remove. 0 to not remove anything
 *           (insertion only)
 *   callback: Function to call on data for removed nodes. Parameters are:
 *             - (void *) data
 *             - (void *) callback_data
 *   callback_data: Data to pass to callback as 2nd parameter.
 *   replacement: Pointer to the list to insert at given offset. This list is
 *                not modified.
 */
void
gds_slist_splice(
	gds_slist_t *list,
	unsigned int offset,
	unsigned int length,
	void *callback,
	void *callback_data,
	gds_slist_t *replacement
);

/*
 * Creates a new list from a portion of another list.
 *
 * Parameters:
 *   list: Pointer to the list.
 *   offset: Offset of the first node to keep for the new list.
 *   length: Number of nodes to copy to the new list.
 *   callback: Function to apply on data. Its return value will be stored into
 *             the new list instead of the original data. Can be NULL.
 *             Parameters are:
 *             - (void *) data
 *             - (void *) callback_data
 *   callback_data: Data to pass to callback as 2nd parameter.
 *
 * Returns:
 *   Pointer to the new list.
 */
gds_slist_t *
gds_slist_slice(
	gds_slist_t *list,
	unsigned int offset,
	unsigned int length,
	void *callback,
	void *callback_data
);

/*
 * Maps a function to data contained in list.
 *
 * Parameters:
 *   list: Pointer to the list.
 *   callback: Function to apply on data. Parameters are:
 *             - (void *) data
 *             - (unsigned int) offset of node being processed
 *             - (void *) callback_data
 *   callback_data: Data to pass to callback as 3rd parameter.
 */
void
gds_slist_map(
	gds_slist_t *list,
	void *callback,
	void *callback_data
);

/*
 * Filter a list to create a new one.
 *
 * Parameters:
 *   list: Pointer to the list.
 *   callback: Function that defines if data is kept or not. If return value is
 *             true (not zero), data is kept for the new list.  Otherwise data
 *             is not kept. Parameters are:
 *             - (void *) data
 *             - (void *) callback_data
 *   callback_data: Data passed to callback as 2nd parameter.
 *
 * Returns:
 *   Pointer to the new list.
 */
gds_slist_t *
gds_slist_filter(
	gds_slist_t *list,
	void *callback,
	void *callback_data
);

/*
 * Reduce a list into a single value.
 *
 * To reduce a list into a single value, a callback function is called on every
 * node's data, with the value of the previous invocation of callback.
 *
 * Parameters:
 *   list: Pointer to the list.
 *   callback: Function to apply on data. Parameters are:
 *             - (void *) return value of previous invocation of callback,
 *               or NULL if this is the first invocation.
 *             - (void *) data of the current node
 *             - (unsigned int) offset of the current node
 *             - (void *) callback_data
 *             This function should reduce 1st and 2nd parameter into a single
 *             value and return this single value.
 *   callback_data: Data passed to callback as 4th parameter.
 *
 * Returns:
 *   Result of list reduction.
 */
void *
gds_slist_reduce(
	gds_slist_t *list,
	void *callback,
	void *callback_data
);

/*
 * Get size (length) of list.
 *
 * Parameters:
 *   list : Pointer to the list.
 *
 * Return:
 *   Number of elements in list.
 */
unsigned int
gds_slist_size(
	gds_slist_t *list
);

/*
 * Create an iterator on list
 *
 * Parameters:
 *   list: pointer to list
 *
 * Returns:
 *   pointer to iterator on success
 *   NULL if an error occured
 */
gds_iterator_t *
gds_slist_iterator_new(
	gds_slist_t *list
);

/*
 * foreach macro for slist
 *
 * Parameters:
 *   var: variable that will contain current data
 *   list: pointer to the list
 *
 * Example:
 *   gds_slist_t *l = gds_slist("1", "2", "3");
 *   char *s;
 *   gds_slist_foreach(s, l) {
 *           printf("%s\n", s);
 *   }
 */
#define gds_slist_foreach(var, list)                                     \
	for (gds_iterator_t *gds_slist_it = gds_slist_iterator_new(list) \
		; gds_slist_it != NULL                                   \
		; gds_iterator_free(gds_slist_it), gds_slist_it = NULL)  \
	while (!gds_iterator_step(gds_slist_it)                          \
		&& ((var = gds_iterator_get(gds_slist_it)) || !var))

/*
 * Free list
 *
 * Parameters:
 *   list: Pointer to list
 *   callback: Callback function that will be called on data before freeing the
 *             node. Prototype is: void callback(void *, void *)
 *             1st parameter is data of the node being freed.
 *             2nd parameter is <callback_data>.
 *   callback_data : Data passed to callback
 */
void
gds_slist_free(
	gds_slist_t *list,
	void *callback,
	void *callback_data
);

#ifdef __cplusplus
}
#endif

#endif /* ! gds_slist_h_included */

