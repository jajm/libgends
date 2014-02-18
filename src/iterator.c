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
 * File              : iterator.c
 * Short description : Iterators management
 *
 * What this file contains is just an interface for containers which want to
 * provide an iterator. By using this interface, whatever container is used,
 * it's always the same API.
 * To implement an iterator for your container, you have to:
 *  - define a struct, or whatever you need to store iterator informations
 *  - define 5 functions (described below) which will deal with these
 *  informations:
 *    . int reset(void *data)
 *    . int step(void *data)
 *    . void * get(void *data)
 *    . void * getkey(void *data)
 *    . void free(void *data)
 *  - and provide a way to get an initialized iterator for your container
 * See slist.h or dlist.h for examples of implementation
 */

#include <stdlib.h>
#include "malloc.h"
#include "assert.h"
#include "log.h"
#include "iterator.h"

gds_iterator_t *gds_iterator_new(void *data, void *reset_cb, void *step_cb,
	void *get_cb, void *getkey_cb, void *free_cb)
{
	gds_iterator_t *it;

	gds_assert(data != NULL, NULL);
	gds_assert(reset_cb != NULL, NULL);
	gds_assert(step_cb != NULL, NULL);
	gds_assert(get_cb != NULL, NULL);
	gds_assert(getkey_cb != NULL, NULL);

	it = gds_malloc(sizeof(gds_iterator_t));

	it->data = data;
	it->reset_cb = reset_cb;
	it->step_cb = step_cb;
	it->get_cb = get_cb;
	it->getkey_cb = getkey_cb;
	it->free_cb = free_cb;

	gds_iterator_reset(it);

	return it;
}

int gds_iterator_reset(gds_iterator_t *it)
{
	gds_assert(it != NULL, -1);

	return it->reset_cb(it->data);
}

int gds_iterator_step(gds_iterator_t *it)
{
	gds_assert(it != NULL, -1);

	return it->step_cb(it->data);
}

void * gds_iterator_get(gds_iterator_t *it)
{
	gds_assert(it != NULL, NULL);

	return it->get_cb(it->data);
}

void * gds_iterator_getkey(gds_iterator_t *it)
{
	gds_assert(it != NULL, NULL);

	return it->getkey_cb(it->data);
}

void gds_iterator_free(gds_iterator_t *it)
{
	if (it != NULL) {
		if (it->free_cb != NULL) {
			it->free_cb(it->data);
		}
		free(it);
	}
}
