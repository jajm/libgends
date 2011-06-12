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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "error.h"
#include "types.h"
#include "iterator.h"

iterator_t *it_new(const char *type_name, void *container)
{
	iterator_t *it;
	size_t len;

	assert(type_name != NULL);
	assert(container != NULL);

	it = malloc(sizeof(iterator_t));
	if(it){
		len = strlen(type_name);
		it->type_name = malloc(len+1);
		if(it->type_name){
			strncpy(it->type_name, type_name, len+1);
			it->container = container;
			it->pointer = NULL;
		}else{
			Error("Memory allocation error");
			free(it);
			it = NULL;
		}
	}else{
		Error("Memory allocation error");
	}

	return it;
}

s8 it_reset(iterator_t *it)
{
	func_ptr_t first;

	assert(it != NULL);
	
	first = type_get_func(it->type_name, "first");
	if(first){
		it->pointer = (void *)first(it->container);
	}else{
		ErrorP("Failed to retrieve function 'first' for type '%s'",
			it->type_name);
		return -1;
	}

	return 0;
}

s8 it_next(iterator_t *it)
{
	func_ptr_t next;

	assert(it != NULL);

	next = type_get_func(it->type_name, "next");
	if(next){
		it->pointer = (void *)next(it->container, it->pointer);
	}else{
		ErrorP("Failed to retrieve function 'next' for type '%s'",
			it->type_name);
		return -1;
	}

	return 0;
}

void *it_get(iterator_t *it)
{
	func_ptr_t get;

	assert(it != NULL);

	get = type_get_func(it->type_name, "get");
	if(get == NULL){
		ErrorP("Failed to retrieve function 'get' for type '%s'",
			it->type_name);
		return NULL;
	}

	return (void *)get(it->container, it->pointer);
}

s8 it_has_next(iterator_t *it)
{
	func_ptr_t has_next;

	assert(it != NULL);

	has_next = type_get_func(it->type_name, "has_next");
	if(has_next == NULL){
		ErrorP("Failed to retrieve function 'has_next' for type '%s'",
			it->type_name);
		return -1;
	}

	return has_next(it->container, it->pointer);
}

void it_free(iterator_t *it)
{
	if(it){
		free(it->type_name);
		free(it);
	}
}
