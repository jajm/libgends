/*****************************************************************************
 * Copyright (C) 2010-2012 Julian Maurice                                    *
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
 * File                 : iterator.c                                         *
 * Short description    : Iterators management                               *
 *****************************************************************************
 * What this file contains is just an interface for containers which want to *
 * provide an iterator. By using this interface, whatever container is used, *
 * it's always the same API.                                                 *
 * To implement an iterator for your container, you have to:                 *
 *  - define a struct, or whatever you need to store iterator informations   *
 *  - define 3 functions (described below) which will deal with these        *
 *  informations:                                                            *
 *    . int8_t reset(void *data)                                             *
 *    . int8_t step(void *data)                                              *
 *    . void * get(void *data, bool copy_data)                               *
 *  - and provide a way to get an initialized iterator for your container    *
 * See slist.h or dlist.h for examples of implementation                     *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "log.h"
#include "iterator.h"

gds_iterator_t *gds_iterator_new(void *data, gds_iterator_reset_func_t reset_f,
	gds_iterator_step_func_t step_f, gds_iterator_get_func_t get_f)
{
	gds_iterator_t *it;

	if(data == NULL || reset_f == NULL || step_f == NULL || get_f == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	it = malloc(sizeof(gds_iterator_t));
	if(it == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}

	it->data = data;
	it->reset_f = reset_f;
	it->step_f = step_f;
	it->get_f = get_f;

	return it;
}

int8_t gds_iterator_reset(gds_iterator_t *it)
{
	if(it == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	return it->reset_f(it->data);
}

int8_t gds_iterator_step(gds_iterator_t *it)
{
	if(it == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	return it->step_f(it->data);
}

void * gds_iterator_get(gds_iterator_t *it, bool copy_data)
{
	if(it == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	return it->get_f(it->data, copy_data);
}

void gds_iterator_free(gds_iterator_t *it, void free_f(void *data))
{
	if(it){
		if(free_f) {
			free_f(it->data);
		}
		free(it);
	}
}
