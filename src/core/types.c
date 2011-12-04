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
 * File                 : types.c                                            *
 * Short description    : Custom types management                            *
 *****************************************************************************
 * Custom types are user-defined types. They are described by a unique name, *
 * a size in bytes and have a list of functions identified by their name.    *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "types.h"

#define DEFAULT_HT_SIZE 1024

typedef struct{
	char *name;
	size_t size;
	funcs_t funcs;
} type_t;

type_t *type_new(const char *name, size_t size)
{
	type_t *t = NULL;
	size_t len;

	if(name == NULL || size == 0){
		Error("Bad parameters");
		return NULL;
	}
	
	if( (t = malloc(sizeof(type_t))) == NULL){
		Error("Memory allocation error");
		return NULL;
	}

	len = strlen(name);
	if( (t->name = malloc(len+1)) == NULL){
		free(t);
		Error("Memory allocation error");
		return NULL;
	}
	strncpy(t->name, name, len+1);
	t->size = size;
	t->funcs = NULL;

	return t;
}

void type_free(type_t *type)
{
	if(type){
		free(type->name);
		free(type);
	}
}

typedef struct typelist_node_s{
	type_t *type;
	struct typelist_node_s *next;
} typelist_node_t, *typelist_t;

int8_t typelist_add(typelist_t *tlist, type_t *type)
{
	typelist_node_t *tmp, *tmp2 = NULL;

	if(tlist == NULL || type == NULL){
		Error("Bad parameters");
		return -1;
	}

	tmp = *tlist;
	while(tmp != NULL){
		if(strcmp(tmp->type->name, type->name) == 0){
			Error("Type %s already exists", type->name);
			return 1;
		}
		tmp2 = tmp;
		tmp = tmp->next;
	}
	if( (tmp = malloc(sizeof(typelist_node_t))) == NULL){
		Error("Memory allocation error");
		return -1;
	}
	tmp->type = type;
	if(tmp2){
		tmp->next = NULL;
		tmp2->next = tmp;
	}else{
		tmp->next = *tlist;
		*tlist = tmp;
	}

	return 0;
}

int8_t typelist_del(typelist_t *tlist, const char *name)
{
	typelist_node_t *tmp, *tmp2 = NULL;

	if(tlist == NULL || name == NULL){
		Error("Bad parameters");
		return -1;
	}
	tmp = *tlist;
	while(tmp && strcmp(tmp->type->name, name)){
		tmp2 = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL){
		Error("Type '%s' doesn't exist", name);
		return -1;
	}

	if(tmp2 == NULL){
		*tlist = tmp->next;
		type_free(tmp->type);
		free(tmp);
	}else{
		tmp2->next = tmp->next;
		type_free(tmp->type);
		free(tmp);
	}

	return 0;
}

type_t *typelist_get(typelist_t tlist, const char *name)
{
	typelist_node_t *tmp;
	type_t *t = NULL;
	
	if(tlist == NULL || name == NULL){
		Error("Bad parameters");
		return NULL;
	}

	tmp = tlist;
	while(tmp != NULL){
		if(strcmp(tmp->type->name, name) == 0){
			t = tmp->type;
			break;
		}
		tmp = tmp->next;
	}

	return t;
}

void typelist_free(typelist_t tlist)
{
	typelist_node_t *tmp, *tmp2;

	if(tlist){
		tmp = tlist;
		while(tmp != NULL){
			tmp2 = tmp->next;
			type_free(tmp->type);
			free(tmp);
			tmp = tmp2;
		}
	}
}


struct types_t{
	typelist_t *types;
	uint32_t size;
};

struct types_t *g_types = NULL;

uint32_t type_hash(const char *name)
{
	uint32_t hash = 0;
	uint32_t i;
	size_t len;

	if(name == NULL) return 0;

	len = strlen(name);
	for(i=0; i<len; i++){
		hash += name[i] * (i+1);
	}
	hash %= g_types->size;

	return hash;
}

int8_t types_init(uint32_t size)
{
	uint32_t i;

	if(g_types == NULL){
		g_types = malloc(sizeof(struct types_t));
		if(g_types == NULL){
			Error("Memory allocation error");
			return -1;
		}
	}else{
		Error("User types already initialized");
		return -1;
	}

	g_types->size = (size != 0) ? size : DEFAULT_HT_SIZE;
	g_types->types = malloc(g_types->size*sizeof(typelist_t));
	if(g_types->types == NULL){
		Error("Memory allocation error");
		free(g_types);
		return -1;
	}

	for (i=0; i<g_types->size; i++)
		g_types->types[i] = NULL;

	return 0;
}

bool types_initialized(void)
{
	return (g_types != NULL) ? (true) : (false);
}

bool type_exist(const char *name)
{
	uint32_t hash;

	if(name == NULL) {
		Error("Bad parameters: NULL pointer");
		return false;
	}
	if(g_types == NULL){
		Error("User types not initialized");
		return false;
	}
	hash = type_hash(name);

	return (typelist_get(g_types->types[hash], name) != NULL)
	       ? true
	       : false;
}

int8_t type_reg(const char *name, size_t size)
{
	int8_t tla;
	uint32_t hash;
	type_t *type;

	if(name == NULL){
		Error("Bad parameters");
		return -1;
	}

	if(g_types == NULL){
		Error("User types not initialized");
		return -1;
	}

	if( (type = type_new(name, size)) == NULL){
		ErrorP("Type creation failed");
		return -1;
	}

	hash = type_hash(name);
	tla = typelist_add(&g_types->types[hash], type);
	if(tla != 0){
		ErrorP("Adding type '%s' to the hash table failed", name);
	}

	return tla;
}

int8_t type_reg_func(const char *name, const char *func_name, func_ptr_t func_ptr)
{
	uint32_t hash;
	type_t *type;

	if(name == NULL || func_name == NULL || func_ptr == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(g_types == NULL){
		Error("User types not initialized");
		return -1;
	}

	hash = type_hash(name);
	type = typelist_get(g_types->types[hash], name);
	if(type){
		if(funcs_add(&type->funcs, func_new(func_name, func_ptr)) == NULL){
			ErrorP("Failed to add function '%s' to type '%s'",func_name,name);
			return -1;
		}
	}else{
		ErrorP("Failed to find type '%s'", name);
		return -1;
	}

	return 0;
}

func_ptr_t type_get_func(const char *name, const char *func_name)
{
	uint32_t hash;
	type_t *type;
	func_ptr_t func_ptr = NULL;

	if(name == NULL || func_name == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(g_types == NULL){
		Error("User types not initialized");
		return NULL;
	}

	hash = type_hash(name);
	type = typelist_get(g_types->types[hash], name);
	if(type){
		if(type->funcs)
			func_ptr = funcs_get_ptr(type->funcs, func_name);
		else
			Error("No functions associated with type %s", name);
	}else{
		ErrorP("Failed to find type '%s'", name);
	}

	return func_ptr;
}

int8_t type_unreg(const char *name)
{
	uint32_t hash;

	if(name == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(g_types == NULL){
		Error("User types not initialized");
		return -1;
	}

	hash = type_hash(name);
	if( (typelist_del(&g_types->types[hash], name)) < 0){
		ErrorP("Deleting type '%s' from hash table failed", name);
		return -1;
	}

	return 0;
}

int8_t type_unreg_func(const char *name, const char *func_name)
{
	uint32_t hash;
	type_t *type;

	if(name == NULL || func_name == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(g_types == NULL){
		Error("User types not initialized");
		return -1;
	}

	hash = type_hash(name);
	type = typelist_get(g_types->types[hash], name);
	if(type == NULL){
		ErrorP("Failed to retrieve type %s", name);
		return -1;
	}
	if(funcs_del(&(type->funcs), func_name) < 0){
		ErrorP("Failed to delete function %s for type %s", func_name, name);
		return -1;
	}

	return 0;
}

size_t type_sizeof(const char *name)
{
	uint32_t hash;
	size_t size = 0;
	typelist_node_t *tmp;

	if(name == NULL){
		Error("Parameter is NULL");
		return 0;
	}
	if(g_types == NULL){
		Error("User types not initialized");
		return 0;
	}

	hash = type_hash(name);
	tmp = g_types->types[hash];
	while(tmp != NULL){
		if(strcmp(tmp->type->name, name) == 0){
			size = tmp->type->size;
			break;
		}
		tmp = tmp->next;
	}

	if(size == 0) Error("Type '%s' doesn't exist", name);

	return size;
}

void types_free(void)
{
	uint32_t i;

	if(g_types == NULL) return;

	for(i=0; i<g_types->size; i++){
		typelist_free(g_types->types[i]);
		g_types->types[i] = NULL;
	}
	free(g_types->types);
	free(g_types);
	g_types = NULL;
}

void types_print(void)
{
	uint32_t i;
	typelist_node_t *tmp;

	if(g_types == NULL){
		fprintf(stderr, "User types not initialized\n");
		return;
	}

	for(i=0; i<g_types->size; i++){
		tmp = g_types->types[i];
		if(tmp) printf("[%d]", i);
		while(tmp != NULL){
			printf("\tname(\"%s\"), size(\"%zd\"), "
				"type_sizeof(\"%zd\")\n",
				tmp->type->name, tmp->type->size,
				type_sizeof(tmp->type->name));
			tmp = tmp->next;
		}
	}
}

