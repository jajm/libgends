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
 * File              : iterator.h
 * Short description : Iterators management
 *
 * What this file contains is just an interface for containers which want to
 * provide an iterator. By using this interface, whatever container is used,
 * it's always the same API.
 * To implement an iterator for your container, you have to:
 *  - define a struct, or whatever you need to store iterator informations
 *  - define 4 functions (described below) which will deal with these
 *  informations:
 *    . int reset(void *data)
 *    . int step(void *data)
 *    . void * get(void *data)
 *    . void free(void *data)
 *  - and provide a way to get an initialized iterator for your container
 * See slist.h or dlist.h for examples of implementation
 */

#ifndef iterator_h_included
#define iterator_h_included

typedef struct {
	/* Used to store iterator-specific information
	 * It will be passed to the four following functions */
	void *data;

	/* This function must reset the iterator to beginning
	 * It must take one argument (data) and return 0 on success,
	 * a negative value otherwise */
	int (*reset_cb)(void *data);

	/* This function must move iterator to the next element
	 * It must take one argument (data) and return 0 on success,
	 * a positive value if iterator has reached the end, and a negative
	 * value otherwise */
	int (*step_cb)(void *data);

	/* This fuction must return data of element pointed by iterator
	 * It must take one argument (data) and return a valid pointer on
	 * success, or NULL otherwise */
	void * (*get_cb)(void *data);

	/* This function must return key of element pointed by iterator, if any.
	 * It must take one argument (data) and return a valid pointer on
	 * success, or NULL otherwise. */
	void * (*getkey_cb)(void *data);

	/* This function must free data referenced by 'data' member.
	 * It must take one argument (data).
	 * It can be NULL, in this case data won't be freed. */
	void (*free_cb)(void *data);
} gds_iterator_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new iterator and call reset_cb.
 * This function only allocate memory for the structure and affect parameters
 * to corresponding members.
 * Return a pointer on the new iterator, or NULL if an error occurs */
gds_iterator_t *
gds_iterator_new(
	void *data,
	void *reset_cb,
	void *step_cb,
	void *get_cb,
	void *getkey_cb,
	void *free_cb
);

/* Alias for it->reset_cb(it->data) */
int
gds_iterator_reset(
	gds_iterator_t *it
);

/* Alias for it->step_cb(it->data) */
int
gds_iterator_step(
	gds_iterator_t *it
);

/* Alias for it->get_cb(it->data) */
void *
gds_iterator_get(
	gds_iterator_t *it
);

/* Alias for it->getkey_cb(it->data) */
void *
gds_iterator_getkey(
	gds_iterator_t *it
);

/* Alias for it->free_cb(it->data) */
void
gds_iterator_free(
	gds_iterator_t *it
);

#ifdef __cplusplus
}
#endif

#endif /* Not iterator_h_included */

