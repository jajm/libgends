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
 * File              : typed_container.c                                     *
 * Short description : Generic data container                                *
 *****************************************************************************
 * A 'container' can contain one and only one generic data, represented by a *
 * pointer to the data, and the data type name                               *
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "exception.h"
#include "types.h"
#include "log.h"
#include "check_arg.h"
#include "typed_container.h"
#include "callbacks.h"

gds_typed_container_t * gds_typed_container_new(const char *type_name, void *data_ptr,
	bool copy_data)
{
	gds_typed_container_t *c;
	size_t length;

	GDS_CHECK_ARG_NOT_NULL(type_name);
	GDS_CHECK_ARG_NOT_NULL(data_ptr);

	c = malloc(sizeof(gds_typed_container_t));
	if(c == NULL){
		GDS_THROW(NotEnoughMemoryException, "Failed to allocate %d "
			"bytes", sizeof(gds_typed_container_t));
	}

	length = strlen(type_name);
	c->type_name = malloc(length+1);
	if(c->type_name == NULL){
		free(c);
		GDS_THROW(NotEnoughMemoryException, "Failed to allocate %d "
			"bytes", length+1);
	}
	strncpy(c->type_name, type_name, length+1);
	assert(c->type_name[length] == '\0');

	if(copy_data) {
		gds_alloc_cb alloc_cb = (gds_alloc_cb)gds_type_get_func(
			c->type_name, "alloc");
		if(alloc_cb == NULL) {
			free(c->type_name);
			free(c);
			GDS_THROW(FunctionNotFoundException, "Failed to "
				"retrieve function 'alloc' for type %s",
				c->type_name);
		}
		c->data_ptr = alloc_cb(data_ptr);
	} else {
		c->data_ptr = data_ptr;
	}
	
	return c;
}

gds_typed_container_t * gds_typed_container_new_clone(const gds_typed_container_t *src)
{
	GDS_CHECK_ARG_NOT_NULL(src);

	return gds_typed_container_new(src->type_name, src->data_ptr, true);
}

void gds_typed_container_set(gds_typed_container_t *c, const char *type_name,
	void *data_ptr, bool free_old_data, bool copy_data)
{
	size_t length;
	void *_data_ptr;
	char *_type_name;

	GDS_CHECK_ARG_NOT_NULL(c);
	GDS_CHECK_ARG_NOT_NULL(type_name);
	GDS_CHECK_ARG_NOT_NULL(data_ptr);

	if(copy_data){
		gds_alloc_cb alloc_cb = (gds_alloc_cb)gds_type_get_func(type_name, "alloc");
		if(alloc_cb == NULL) {
			GDS_THROW(FunctionNotFoundException, "Failed to "
				"retrieve function 'alloc' for type %s",
				type_name);
		}
		_data_ptr = alloc_cb(data_ptr);
		assert(_data_ptr != NULL);
	} else {
		_data_ptr = data_ptr;
	}

	if(free_old_data){
		gds_free_cb free_cb = (gds_free_cb)gds_type_get_func(c->type_name, "free");
		if(free_cb) {
			free_cb(c->data_ptr);
		} else {
			GDS_THROW(FunctionNotFoundException, "Failed to "
				"retrieve 'free' function for type %s",
				c->type_name);
		}
	}

	if(strcmp(c->type_name, type_name) != 0){
		length = strlen(type_name);
		_type_name = malloc(length+1);
		if(_type_name == NULL) {
			GDS_THROW(NotEnoughMemoryException, "Failed to allocate"
				" %d bytes", length+1);
		}
		strncpy(_type_name, type_name, length+1);
		assert(_type_name[length] == '\0');
		free(c->type_name);
	} else {
		_type_name = c->type_name;
	}

	c->type_name = _type_name;
	c->data_ptr = _data_ptr;
}

void gds_typed_container_set_clone(gds_typed_container_t *dst, const gds_typed_container_t *src,
                           bool free_old_data)
{
	GDS_CHECK_ARG_NOT_NULL(src);

	gds_typed_container_set(dst, src->type_name, src->data_ptr, free_old_data,
		true);
}

void * gds_typed_container_get(gds_typed_container_t *c, bool copy)
{
	void *data_ptr = NULL;

	GDS_CHECK_ARG_NOT_NULL(c);

	if(copy) {
		gds_alloc_cb alloc_cb = (gds_alloc_cb)gds_type_get_func(c->type_name, "alloc");
		if(alloc_cb) {
			data_ptr = alloc_cb(c->data_ptr);
		} else {
			GDS_THROW(FunctionNotFound, "Failed to retrieve 'alloc'"
				" function for type %s", c->type_name);
		}
	} else {
		data_ptr = c->data_ptr;
	}

	return data_ptr;
}

int32_t gds_typed_container_cmp(const gds_typed_container_t *c1, const gds_typed_container_t *c2)
{
	gds_cmp_cb cmp_cb;

	GDS_CHECK_ARG_NOT_NULL(c1);
	GDS_CHECK_ARG_NOT_NULL(c2);

	if (strcmp(c1->type_name, c2->type_name) != 0) {
		GDS_THROW(BadTypeException, "can't compare containers of "
			"different types (%s,%s)", c1->type_name,
			c2->type_name);
	}

	cmp_cb = (gds_cmp_cb)gds_type_get_func(c1->type_name, "cmp");
	if (cmp_cb == NULL) {
		GDS_THROW(FunctionNotFoundException, "Failed to retrieve "
			"function 'cmp' for type %s", c1->type_name);
	}

	return cmp_cb(c1->data_ptr, c2->data_ptr);
}

void gds_typed_container_free(gds_typed_container_t *c, bool free_data)
{
	if(c != NULL){
		if(free_data) {
			gds_free_cb free_cb;
			free_cb = (gds_free_cb)gds_type_get_func(c->type_name, "free");
			if(free_cb) {
				free_cb(c->data_ptr);
			} else {
				GDS_THROW(FunctionNotFoundException, "Failed to"
					" retrieve function 'cmp' for type %s",
					c->type_name);
			}
		}
		free(c->type_name);
		free(c);
	}
}

