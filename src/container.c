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
 * File              : container.c                                           *
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
#include "types.h"
#include "log.h"
#include "container.h"

gds_container_t * gds_container_new(const char *type_name, void *data_ptr,
	bool copy_data)
{
	gds_container_t *c;
	size_t length;

	if(type_name == NULL || data_ptr == NULL){
		GDS_LOG_ERROR("Bad parameter value");
		return NULL;
	}

	c = malloc(sizeof(gds_container_t));
	if(c == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}

	length = strlen(type_name);
	c->type_name = malloc(length+1);
	if(c->type_name == NULL){
		free(c);
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	strncpy(c->type_name, type_name, length+1);
	assert(c->type_name[length] == '\0');

	if(copy_data) {
		gds_func_ptr_t alloc_f = gds_type_get_func(c->type_name, "alloc");
		if(alloc_f == NULL) {
			GDS_LOG_ERROR("Failed to retrieve function 'alloc' for"
				"type %s", c->type_name);
			free(c->type_name);
			free(c);
			return NULL;
		}
		c->data_ptr = (void *)alloc_f(data_ptr);
		assert(c->data_ptr != NULL);
	} else {
		c->data_ptr = data_ptr;
	}
	
	return c;
}

gds_container_t * gds_container_new_clone(const gds_container_t *src)
{
	if(src == NULL){
		GDS_LOG_ERROR("Bad parameters: NULL pointer");
		return NULL;
	}

	return gds_container_new(src->type_name, src->data_ptr, true);
}

int8_t gds_container_set(gds_container_t *c, const char *type_name,
	void *data_ptr, bool free_old_data, bool copy_data)
{
	size_t length;
	void *_data_ptr;
	char *_type_name;

	if(c == NULL || type_name == NULL || data_ptr == NULL){
		GDS_LOG_ERROR("Bad parameters: NULL pointer(s)");
		return -1;
	}

	if(copy_data){
		gds_func_ptr_t alloc_f = gds_type_get_func(type_name, "alloc");
		if(alloc_f == NULL) {
			GDS_LOG_ERROR("Failed to retrieve function 'alloc' for"
				"type %s", type_name);
			return -1;
		}
		_data_ptr = (void *)alloc_f(data_ptr);
		assert(_data_ptr != NULL);
	} else {
		_data_ptr = data_ptr;
	}

	if(strcmp(c->type_name, type_name) != 0){
		length = strlen(type_name);
		_type_name = malloc(length+1);
		if(_type_name == NULL) {
			GDS_LOG_ERROR("Memory allocation error");
			return -1;
		}
		strncpy(_type_name, type_name, length+1);
		assert(_type_name[length] == '\0');
		free(c->type_name);
	} else {
		_type_name = c->type_name;
	}

	if(free_old_data){
		gds_func_ptr_t free_f = gds_type_get_func(c->type_name, "free");
		if(free_f) {
			free_f(c->data_ptr);
		} else {
			GDS_LOG_WARNING("Can't retrieve 'free' function for type"
				" %s", c->type_name);
		}
	}

	c->type_name = _type_name;
	c->data_ptr = _data_ptr;

	return 0;
}

int8_t gds_container_set_clone(gds_container_t *dst, const gds_container_t *src,
                           bool free_old_data)
{
	if(src == NULL){
		GDS_LOG_ERROR("Bad parameters: NULL pointer");
		return -1;
	}

	return gds_container_set(dst, src->type_name, src->data_ptr, free_old_data,
		true);
}

void * gds_container_get(gds_container_t *c, bool copy)
{
	void *data_ptr = NULL;

	if(c == NULL) {
		GDS_LOG_ERROR("Bad parameters: NULL pointer");
		return NULL;
	}

	if(copy) {
		gds_func_ptr_t alloc_f = gds_type_get_func(c->type_name, "alloc");
		if(alloc_f) {
			data_ptr = (void *)alloc_f(c->data_ptr);
		} else {
			GDS_LOG_WARNING("Failed to retrieve 'alloc' function "
				"for type '%s'.", c->type_name);
		}
	} else {
		data_ptr = c->data_ptr;
	}

	return data_ptr;
}

int32_t gds_container_cmp(const gds_container_t *c1, const gds_container_t *c2)
{
	gds_func_ptr_t cmp_f;
	int32_t cmp;

	if(c1 != NULL && c2 != NULL
	  && strcmp(c1->type_name, c2->type_name) == 0
	  && (cmp_f = gds_type_get_func(c1->type_name, "cmp")) != NULL) {
		cmp = cmp_f(c1->data_ptr, c2->data_ptr);
	} else {
		GDS_LOG_WARNING("Can't compare containers, 'cmp' function "
			"doesn't exist or containers are of different type");
		/* We cannot compare them, not sure what to return in this case */
		cmp = -1;
	}
	return cmp;
}

void gds_container_free(gds_container_t *c, bool free_data)
{
	if(c != NULL){
		if(free_data) {
			gds_func_ptr_t free_f;
			free_f = gds_type_get_func(c->type_name, "free");
			if(free_f) {
				free_f(c->data_ptr);
			} else {
				GDS_LOG_WARNING("Can't retrieve 'free' function"
				" for type '%s'. Impossible to free data",
				c->type_name);
			}
		}
		free(c->type_name);
		free(c);
	}
}

