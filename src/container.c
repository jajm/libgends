/*****************************************************************************
 * Copyright (C) 2010-2011 Julian Maurice                                    *
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
#include "error.h"
#include "container.h"

container_t *container_new(const char *type_name, void *data_ptr,
	bool copy_data)
{
	container_t *c;
	size_t length;

	if(type_name == NULL || data_ptr == NULL){
		Error("Bad parameter value");
		return NULL;
	}

	c = malloc(sizeof(container_t));
	if(c == NULL){
		Error("Memory allocation error");
		return NULL;
	}

	length = strlen(type_name);
	c->type_name = malloc(length+1);
	if(c->type_name == NULL){
		free(c);
		Error("Memory allocation error");
		return NULL;
	}
	strncpy(c->type_name, type_name, length+1);
	assert(c->type_name[length] == '\0');

	if(copy_data){
		uint32_t data_size = type_sizeof(c->type_name);
		if(data_size == 0){
			ErrorP("Failed to retrieve data size, can't copy data");
			free(c->type_name);
			free(c);
			return NULL;
		}
		c->data_ptr = malloc(data_size);
		if(c->data_ptr == NULL){
			Error("Memory allocation error");
			free(c->type_name);
			free(c);
			return NULL;
		}
		memmove(c->data_ptr, data_ptr, data_size);
	}else{
		c->data_ptr = data_ptr;
	}
	
	return c;
}

container_t * container_new_clone(const container_t *src)
{
	if(src == NULL){
		Error("Bad parameters: NULL pointer");
		return NULL;
	}

	return container_new(src->type_name, src->data_ptr, true);
}

int8_t container_set(container_t *c, const char *type_name, void *data_ptr,
                     bool free_old_data, bool copy_data)
{
	size_t length;
	size_t data_size;
	void *_data_ptr;
	char *_type_name;

	if(c == NULL || type_name == NULL || data_ptr == NULL){
		Error("Bad parameters: NULL pointer(s)");
		return -1;
	}

	if(copy_data){
		data_size = type_sizeof(type_name);
		if(data_size == 0){
			ErrorP("Failed to retrieve '%s' size", type_name);
			return -1;
		}
		_data_ptr = malloc(data_size);
		if(_data_ptr == NULL){
			Error("Memory allocation error");
			return -1;
		}
		memmove(_data_ptr, data_ptr, data_size);
	} else {
		_data_ptr = data_ptr;
	}

	if(strcmp(c->type_name, type_name) != 0){
		length = strlen(type_name);
		_type_name = malloc(length+1);
		if(_type_name == NULL) {
			Error("Memory allocation error");
			return -1;
		}
		strncpy(_type_name, type_name, length+1);
		assert(_type_name[length] == '\0');
		free(c->type_name);
		c->type_name = _type_name;
	}

	if(free_old_data){
		func_ptr_t free_f = type_get_func(c->type_name, "free");
		if(free_f) free_f(c->data_ptr);
		else free(c->data_ptr);
	}

	c->data_ptr = _data_ptr;

	return 0;
}

int8_t container_set_clone(container_t *dst, const container_t *src,
                           bool free_old_data)
{
	if(src == NULL){
		Error("Bad parameters: NULL pointer");
		return -1;
	}

	return container_set(dst, src->type_name, src->data_ptr, free_old_data,
		true);
}

void * container_get(container_t *c, bool copy)
{
	void *data_ptr;

	if(c == NULL) {
		Error("Bad parameters: NULL pointer");
		return NULL;
	}

	if(copy) {
		uint32_t data_size = type_sizeof(c->type_name);
		if(data_size == 0) {
			ErrorP("Failed to retrieve type size : %s", c->type_name);
			return NULL;
		}
		data_ptr = malloc(data_size);
		if(data_ptr == NULL) {
			Error("Memory allocation error");
			return NULL;
		}
		memmove(data_ptr, c->data_ptr, data_size);
	} else {
		data_ptr = c->data_ptr;
	}

	return data_ptr;
}

int32_t container_cmp(const container_t *c1, const container_t *c2)
{
	func_ptr_t cmp_f;
	int32_t cmp;

	if(c1 == NULL && c2 == NULL){
		/* S'ils sont tous les 2 à NULL, on dira qu'ils sont égaux */
		cmp = 0;
	}else if(c1 == NULL && c2 != NULL){
		/* Si l'un des deux est à NULL, c'est l'autre le plus grand */
		cmp = -1;
	}else if(c1 != NULL && c2 == NULL){
		cmp = 1;
	}else if((cmp_f = type_get_func(c1->type_name, "cmp")) != NULL
	  && strcmp(c1->type_name, c2->type_name) == 0){
		/* Si aucun des deux n'est à NULL, qu'ils sont du même type,
		 * et qu'ils possèdent une fonction de comparaison, on utilise
		 * cette dernière */
		cmp = cmp_f(c1->data_ptr, c2->data_ptr);
	}else{
		/* Sinon, on compare d'abord la taille,
		 * et ensuite la valeur des données */
		uint32_t size1 = type_sizeof(c1->type_name);
		uint32_t size2 = type_sizeof(c2->type_name);
		if(size1 == size2){
			cmp = memcmp(c1->data_ptr, c2->data_ptr, size1);
		}else{
			uint32_t min = (size1 < size2) ? (size1) : (size2);
			cmp = memcmp(c1->data_ptr, c2->data_ptr, min);
			if(cmp == 0){
				cmp = size1 - size2;
			}
		}
	}
	return cmp;
}

size_t container_data_size(const container_t *c)
{
	size_t data_size = 0;
	if(c != NULL){
		data_size = type_sizeof(c->type_name);
	}
	return data_size;
}

void container_free(container_t *c, bool free_data)
{

	if(c != NULL){
		if(free_data) {
			func_ptr_t free_f;
			free_f = type_get_func(c->type_name, "free");
			if(free_f) free_f(c->data_ptr);
			else free(c->data_ptr);
		}
		free(c->type_name);
		free(c);
	}
}

