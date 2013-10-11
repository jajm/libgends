/*
 * Copyright 2010-2013 Julian Maurice
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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "iterator.h"
#include "callbacks.h"

gds_iterator_t *gds_iterator_new(void *data, gds_iterator_reset_cb reset_cb,
	gds_iterator_step_cb step_cb, gds_iterator_get_cb get_cb,
	gds_iterator_getkey_cb getkey_cb, gds_free_cb free_cb)
{
	gds_iterator_t *it;

	GDS_CHECK_ARG_NOT_NULL(data);
	GDS_CHECK_ARG_NOT_NULL(reset_cb);
	GDS_CHECK_ARG_NOT_NULL(step_cb);
	GDS_CHECK_ARG_NOT_NULL(get_cb);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);

	it = malloc(sizeof(gds_iterator_t));
	if (it == NULL) {
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_iterator_t));
	}

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
	GDS_CHECK_ARG_NOT_NULL(it);

	return it->reset_cb(it->data);
}

int gds_iterator_step(gds_iterator_t *it)
{
	GDS_CHECK_ARG_NOT_NULL(it);

	return it->step_cb(it->data);
}

void * gds_iterator_get(gds_iterator_t *it)
{
	GDS_CHECK_ARG_NOT_NULL(it);

	return it->get_cb(it->data);
}

void * gds_iterator_getkey(gds_iterator_t *it)
{
	GDS_CHECK_ARG_NOT_NULL(it);

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
