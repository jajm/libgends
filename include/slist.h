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
 * File              : slist.h
 * Short description : singly linked list
 */

#ifndef gds_slist_h_included
#define gds_slist_h_included

#include "iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gds_slist_s gds_slist_t;

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
gds_slist_t *
gds_slist_new(
	void *free_cb
);

/* Create a new list from a data array
 *
 * Parameters:
 *   free_cb : free callback
 *             Prototype: void free_cb(void *ptr)
 *             It should free memory used by object referenced by ptr
 *   n       : size of data array
 *   data    : data array
 *
 * Returns:
 *   pointer to the new list
 */
gds_slist_t *
gds_slist_new_from_array(
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
 *   gds_slist_t *list = gds_slist();
 *
 *   // Creates a list with 3 elements
 *   void *a, *b, *c;
 *   gds_slist_t *list = gds_slist(a, b, c);
 *
 * Notes
 *   free_cb is set to NULL.
 *
 * Returns
 *   (gds_slist_t *) pointer to the new list
 */

#define gds_slist(...) ({ \
	void *gds_va_args[] = {__VA_ARGS__}; \
	gds_slist_new_from_array(NULL, \
		sizeof(gds_va_args) / sizeof(*gds_va_args),  gds_va_args); \
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
gds_slist_set_free_callback(
	gds_slist_t *list,
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
gds_slist_get_free_callback(
	gds_slist_t *list
);

/* Add new data at beginning of list
 *
 * Parameters
 *   list : pointer to list
 *   size : size of data array
 *   data : array of data
 *
 * Returns
 *   0 in case of success
 *   a negative value otherwise
 */
int
gds_slist_unshift_array(
	gds_slist_t *list,
	size_t size,
	void *data[]
);

#define gds_slist_unshift(list, ...) ({ \
	void *gds_va_args[] = {__VA_ARGS__}; \
	gds_slist_unshift_array(list, \
		sizeof(gds_va_args) / sizeof(*gds_va_args),  gds_va_args); \
})

/* Add new data at end of list
 *
 * Parameters
 *   list : pointer to list
 *   size : size of data array
 *   data : array of data
 *
 * Returns
 *   0 in case of success
 *   a negative value otherwise
 */
int
gds_slist_push_array(
	gds_slist_t *list,
	size_t size,
	void *data[]
);

#define gds_slist_push(list, ...) ({ \
	void *gds_va_args[] = {__VA_ARGS__}; \
	gds_slist_push_array(list, \
		sizeof(gds_va_args) / sizeof(*gds_va_args),  gds_va_args); \
})

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
 *   NULL if list is empty
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
 *   NULL if offset is greater or equal to list size
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
gds_slist_splice(
	gds_slist_t *list,
	unsigned int offset,
	unsigned int length,
	gds_slist_t *replacement
);

/* Creates a new list from a portion of another list.
 *
 * Parameters
 *   list          : Pointer to the list.
 *   offset        : Offset of the first node to keep for the new list.
 *   length        : Number of nodes to copy to the new list.
 *   callback      : Function to apply on data. Its return value will be stored
 *                   into the new list instead of the original data. Can be
 *                   NULL.
 *                   Prototype: void * callback(void *data)
 *
 * Notes
 *   The new list has the same free_cb as the original list.
 *
 * Returns
 *   Pointer to the new list.
 */
gds_slist_t *
gds_slist_slice(
	gds_slist_t *list,
	unsigned int offset,
	unsigned int length,
	void *callback
);

/* Maps a function to data contained in list.
 *
 * Parameters
 *   list          : Pointer to the list.
 *   callback      : Function to apply on data. Prototype:
 *                   void * callback(void *data)
 *                   - data: data of node being processed
 *                   Return value replaces node's data
 *
 * Example
 *
 *      int a[] = {1, 2, 3};
 *      gds_slist_t *list = gds_slist(&a[0], &a[1], &a[2]);
 *      // list is 1, 2, 3.
 *      gds_slist_map(list, gds_lambda(void *, (int *a) {
 *              *a = *a + *a;
 *              return a;
 *      }));
 *      // list is 2, 4, 6.
 *
 * Returns
 *   0 on success
 *   a negative value on failure
 */
int
gds_slist_map(
	gds_slist_t *list,
	void *callback
);

/* Filter a list to create a new one.
 *
 * Parameters
 *   list          : Pointer to the list.
 *   callback      : Function that defines if data is kept or not. If return
 *                   value is true (not zero), data is kept for the new list.
 *                   Otherwise data is not kept.
 *                   Prototype: int callback(void *data)
 *
 * Notes
 *   The new list has the same free_cb as the original list.
 *
 * Returns
 *   Pointer to the new list.
 */
gds_slist_t *
gds_slist_filter(
	gds_slist_t *list,
	void *callback
);

/* Reduce a list into a single value.
 *
 * To reduce a list into a single value, a callback function is called on every
 * node's data, with the value of the previous invocation of callback.
 *
 * Parameters
 *   list          : Pointer to the list.
 *   callback      : Function to apply on data. Prototype:
 *                   void * callback(void *previous, void *data)
 *                   - previous: return value of previous invocation of
 *                     callback, or NULL if this is the first invocation.
 *                   - data: data of the current node
 *                   This function should reduce 1st and 2nd parameter into a
 *                   single value and return this single value.
 *
 * Returns:
 *   Result of list reduction.
 */
void *
gds_slist_reduce(
	gds_slist_t *list,
	void *callback
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

/* Free list
 *
 * Calls free_cb.
 *
 * Parameters
 *   list : Pointer to list
 */
void
gds_slist_free(
	gds_slist_t *list
);

/* Destroy list
 *
 * This function will free memory occupied by the list, but will not call
 * free_cb on data.
 * Use gds_slist_free if you want to free data too.
 *
 * This is just an alias for
 *   gds_slist_set_free_callback(list, NULL);
 *   gds_slist_free(list);
 *
 * Parameters
 *   list : Pointer to list.
 */
void gds_slist_destroy(
	gds_slist_t *list
);

#ifdef __cplusplus
}
#endif

#endif /* ! gds_slist_h_included */

