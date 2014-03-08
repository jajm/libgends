/*
 * Copyright 2010-2014 Julian Maurice
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
 * File              : dlist.h
 * Short description : double linked list
 */

#ifndef gds_dlist_h_included
#define gds_dlist_h_included

#include "iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gds_dlist_s gds_dlist_t;

/* Create a new list
 *
 * Parameters
 *   free_cb : free callback
 *             Prototype: void free_cb(void *ptr)
 *             It should free memory used by object referenced by ptr
 *
 * Returns
 *   pointer to the new list
 */
gds_dlist_t *
gds_dlist_new(
	void *free_cb
);

/* Create a new list from a data array
 *
 * Parameters
 *   free_cb : free callback
 *             Prototype: void free_cb(void *ptr)
 *             It should free memory used by object referenced by ptr
 *   n       : size of data array
 *   data    : data array
 *
 * Returns
 *   pointer to the new list
 */
gds_dlist_t *
gds_dlist_new_from_array(
	void *free_cb,
	unsigned int n,
	void *data[]
);

/* Create a new list from the list of parameters
 *
 * Extra parameters
 *   (void *) data to add to the list
 *
 * Examples
 *   // Creates an empty list
 *   gds_dlist_t *list = gds_dlist();
 *
 *   // Creates a list with 3 elements
 *   void *a, *b, *c;
 *   gds_dlist_t *list = gds_dlist(a, b, c);
 *
 * Notes
 *   free_cb is set to NULL.
 *
 * Returns
 *   (gds_dlist_t *) pointer to the new list
 */

#define gds_dlist(...) ({ \
	void *gds_va_args[] = {__VA_ARGS__}; \
	gds_dlist_new_from_array(NULL, \
		sizeof(gds_va_args) / sizeof(*gds_va_args), gds_va_args); \
})

/* Set free callback.
 *
 * Parameters
 *   list    : Pointer to list.
 *   free_cb : free callback
 *             Prototype: void free_cb(void *ptr)
 *             It should free memory used by object referenced by ptr
 *
 * Returns
 *   0 on success
 *   a negative value on failure
 */
int
gds_dlist_set_free_callback(
	gds_dlist_t *list,
	void *free_cb
);

/* Get free callback.
 *
 * Parameters
 *   list : Pointer to list.
 *
 * Returns
 *   Pointer to free callback.
 */
void *
gds_dlist_get_free_callback(
	gds_dlist_t *list
);

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
gds_dlist_unshift(
	gds_dlist_t *list,
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
gds_dlist_push(
	gds_dlist_t *list,
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
 *   NULL if list is empty
 */
void *
gds_dlist_shift(
	gds_dlist_t *list
);

/*
 * Remove the last element of list and return it
 *
 * Parameters:
 *   list: pointer to list
 *
 * Returns:
 *   data that was just removed from list
 *   NULL if list is empty
 */
void *
gds_dlist_pop(
	gds_dlist_t *list
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
 *   NULL if offset is greater or equal to list size
 *
 * Hints:
 *   To get the last element, use gds_dlist_size:
 *     void *last = gds_dlist_get(list, gds_dlist_size(list)-1);
 */
void *
gds_dlist_get(
	gds_dlist_t *list,
	unsigned int offset
);

/* Remove a portion of list and replace it by another list.
 *
 * Calls free_cb.
 *
 * Parameters
 *   list        : Pointer to the list.
 *   offset      : Offset of the first node to remove.
 *   length      : Number of nodes to remove. 0 to not remove anything
 *                 (insertion only)
 *   replacement : Pointer to the list to insert at given offset. This list is
 *                 not modified.
 *
 * Returns
 *   0 on success
 *   a negative value on failure
 */
int
gds_dlist_splice(
	gds_dlist_t *list,
	unsigned int offset,
	unsigned int length,
	gds_dlist_t *replacement
);

/* Creates a new list from a portion of another list.
 *
 * Parameters:
 *   list          : Pointer to the list.
 *   offset        : Offset of the first node to keep for the new list.
 *   length        : Number of nodes to copy to the new list.
 *   callback      : Function to apply on data. Its return value will be stored
 *                   into the new list instead of the original data. Can be
 *                   NULL.
 *                   Prototype: void * callback(void *data, void *callback_data)
 *   callback_data : Data to pass to callback as 2nd parameter.
 *
 * Notes
 *   The new list has the same free_cb as the original list.
 *
 * Returns
 *   Pointer to the new list.
 */
gds_dlist_t *
gds_dlist_slice(
	gds_dlist_t *list,
	unsigned int offset,
	unsigned int length,
	void *callback,
	void *callback_data
);

/* Maps a function to data contained in list.
 *
 * Parameters
 *   list          : Pointer to the list.
 *   callback      : Function to apply on data. Parameters are:
 *                   - (void *) data
 *                   - (unsigned int) offset of node being processed
 *                   - (void *) callback_data
 *   callback_data : Data to pass to callback as 3rd parameter.
 */
int
gds_dlist_map(
	gds_dlist_t *list,
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
gds_dlist_t *
gds_dlist_filter(
	gds_dlist_t *list,
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
 * Notes
 *   The new list has the same free_cb as the original list.
 *
 * Returns:
 *   Result of list reduction.
 */
void *
gds_dlist_reduce(
	gds_dlist_t *list,
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
gds_dlist_size(
	gds_dlist_t *list
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
gds_dlist_iterator_new(
	gds_dlist_t *list
);

/*
 * foreach macro for dlist
 *
 * Parameters:
 *   var: variable that will contain current data
 *   list: pointer to the list
 *
 * Example:
 *   gds_dlist_t *l = gds_dlist("1", "2", "3");
 *   char *s;
 *   gds_dlist_foreach(s, l) {
 *           printf("%s\n", s);
 *   }
 */
#define gds_dlist_foreach(var, list)                                     \
	for (gds_iterator_t *gds_dlist_it = gds_dlist_iterator_new(list) \
		; gds_dlist_it != NULL                                   \
		; gds_iterator_free(gds_dlist_it), gds_dlist_it = NULL)  \
	while (!gds_iterator_step(gds_dlist_it)                          \
		&& ((var = gds_iterator_get(gds_dlist_it)) || !var))

/* Free list
 *
 * Calls free_cb.
 *
 * Parameters
 *   list : Pointer to list
 */
void
gds_dlist_free(
	gds_dlist_t *list
);

/* Destroy list
 *
 * This function will free memory occupied by the list, but will not call
 * free_cb on data.
 * Use gds_dlist_free if you want to free data too.
 *
 * This is just an alias for
 *   gds_dlist_set_free_callback(list, NULL);
 *   gds_dlist_free(list);
 *
 * Parameters
 *   list : Pointer to list.
 */
void gds_dlist_destroy(
	gds_dlist_t *list
);

#ifdef __cplusplus
}
#endif

#endif /* ! gds_dlist_h_included */

