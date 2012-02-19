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
 * File                 : types.c                                            *
 * Short description    : Custom types management                            *
 *****************************************************************************
 * Custom types are user-defined types. They are described by a unique name  *
 * and have a list of functions identified by their name.                    *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "func_list.h"
#include "types.h"

#define GDS_TYPES_DEFAULT_HT_SIZE 1024

typedef struct{
	char *name;
	gds_func_list_node_t *func_list;
} gds_type_t;

gds_type_t * gds_type_new(const char *name)
{
	gds_type_t *t = NULL;
	size_t len;

	if(name == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}
	
	if( (t = malloc(sizeof(gds_type_t))) == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}

	len = strlen(name);
	if( (t->name = malloc(len+1)) == NULL){
		free(t);
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	strncpy(t->name, name, len+1);
	t->func_list = NULL;

	return t;
}

void gds_type_free(gds_type_t *type)
{
	if(type){
		free(type->name);
		gds_func_list_free(type->func_list);
		free(type);
	}
}

typedef struct gds_type_list_node_s{
	gds_type_t *type;
	struct gds_type_list_node_s *next;
} gds_type_list_node_t, *gds_type_list_t;

int8_t gds_type_list_add(gds_type_list_t *type_list, gds_type_t *type)
{
	gds_type_list_node_t *tmp, *tmp2 = NULL;

	if(type_list == NULL || type == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

	tmp = *type_list;
	while(tmp != NULL){
		if(strcmp(tmp->type->name, type->name) == 0){
			GDS_LOG_ERROR("Type %s already exists", type->name);
			return 1;
		}
		tmp2 = tmp;
		tmp = tmp->next;
	}
	if( (tmp = malloc(sizeof(gds_type_list_node_t))) == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		return -1;
	}
	tmp->type = type;
	if(tmp2){
		tmp->next = NULL;
		tmp2->next = tmp;
	}else{
		tmp->next = *type_list;
		*type_list = tmp;
	}

	return 0;
}

int8_t gds_type_list_del(gds_type_list_t *type_list, const char *name)
{
	gds_type_list_node_t *tmp, *tmp2 = NULL;

	if(type_list == NULL || name == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	tmp = *type_list;
	while(tmp && strcmp(tmp->type->name, name)){
		tmp2 = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL){
		GDS_LOG_ERROR("Type '%s' doesn't exist", name);
		return -1;
	}

	if(tmp2 == NULL){
		*type_list = tmp->next;
	}else{
		tmp2->next = tmp->next;
	}
	gds_type_free(tmp->type);
	free(tmp);

	return 0;
}

gds_type_t *gds_type_list_get(gds_type_list_t type_list, const char *name)
{
	gds_type_list_node_t *tmp;
	gds_type_t *t = NULL;
	
	if(type_list == NULL || name == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	tmp = type_list;
	while(tmp != NULL){
		if(strcmp(tmp->type->name, name) == 0){
			t = tmp->type;
			break;
		}
		tmp = tmp->next;
	}

	return t;
}

void gds_type_list_free(gds_type_list_t type_list)
{
	gds_type_list_node_t *tmp, *tmp2;

	if(type_list){
		tmp = type_list;
		while(tmp != NULL){
			tmp2 = tmp->next;
			gds_type_free(tmp->type);
			free(tmp);
			tmp = tmp2;
		}
	}
}


struct gds_types_t{
	gds_type_list_t *type_list;
	uint32_t size;
};

struct gds_types_t *g_types = NULL;

uint32_t gds_type_hash(const char *name)
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

int8_t gds_types_init(uint32_t size)
{
	uint32_t i;

	if(g_types == NULL){
		g_types = malloc(sizeof(struct gds_types_t));
		if(g_types == NULL){
			GDS_LOG_ERROR("Memory allocation error");
			return -1;
		}
	}else{
		GDS_LOG_ERROR("User types already initialized");
		return -1;
	}

	g_types->size = (size != 0) ? size : GDS_TYPES_DEFAULT_HT_SIZE;
	g_types->type_list = malloc(g_types->size*sizeof(gds_type_list_t));
	if(g_types->type_list == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		free(g_types);
		return -1;
	}

	for (i=0; i<g_types->size; i++)
		g_types->type_list[i] = NULL;

	return 0;
}

bool gds_types_initialized(void)
{
	return (g_types != NULL) ? (true) : (false);
}

bool gds_type_exist(const char *name)
{
	uint32_t hash;

	if(name == NULL) {
		GDS_LOG_ERROR("Bad parameters: NULL pointer");
		return false;
	}
	if(g_types == NULL){
		GDS_LOG_ERROR("User types not initialized");
		return false;
	}
	hash = gds_type_hash(name);

	return (gds_type_list_get(g_types->type_list[hash], name) != NULL)
	       ? true
	       : false;
}

int8_t gds_type_register(const char *name)
{
	int8_t tla;
	uint32_t hash;
	gds_type_t *type;

	if(name == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

	if(g_types == NULL) {
		GDS_LOG_ERROR("User types not initialized");
		return -1;
	}

	if( (type = gds_type_new(name)) == NULL ) {
		GDS_LOG_ERROR("Type creation failed");
		return -1;
	}

	hash = gds_type_hash(name);
	tla = gds_type_list_add(&g_types->type_list[hash], type);
	if(tla != 0){
		gds_type_free(type);
		GDS_LOG_ERROR("Adding type '%s' to the hash table failed", name);
	}

	return tla;
}

int8_t gds_type_register_func(const char *name, const char *func_name,
	gds_func_ptr_t func_ptr)
{
	uint32_t hash;
	gds_type_t *type;

	if(name == NULL || func_name == NULL || func_ptr == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	if(g_types == NULL){
		GDS_LOG_ERROR("User types not initialized");
		return -1;
	}

	hash = gds_type_hash(name);
	type = gds_type_list_get(g_types->type_list[hash], name);
	if(type){
		if(gds_func_list_add(&type->func_list, func_name, func_ptr) == NULL){
			GDS_LOG_ERROR("Failed to add function '%s' to type '%s'",func_name,name);
			return -1;
		}
	}else{
		GDS_LOG_ERROR("Failed to find type '%s'", name);
		return -1;
	}

	return 0;
}

gds_func_ptr_t gds_type_get_func(const char *name, const char *func_name)
{
	uint32_t hash;
	gds_type_t *type;
	gds_func_ptr_t func_ptr = NULL;

	if(name == NULL || func_name == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}
	if(g_types == NULL){
		GDS_LOG_ERROR("User types not initialized");
		return NULL;
	}

	hash = gds_type_hash(name);
	type = gds_type_list_get(g_types->type_list[hash], name);
	if(type){
		if(type->func_list)
			func_ptr = gds_func_list_get_ptr(type->func_list, func_name);
		else
			GDS_LOG_ERROR("No functions associated with type %s", name);
	}else{
		GDS_LOG_ERROR("Failed to find type '%s'", name);
	}

	return func_ptr;
}

int8_t gds_type_unregister(const char *name)
{
	uint32_t hash;

	if(name == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	if(g_types == NULL){
		GDS_LOG_ERROR("User types not initialized");
		return -1;
	}

	hash = gds_type_hash(name);
	if( (gds_type_list_del(&g_types->type_list[hash], name)) < 0){
		GDS_LOG_ERROR("Deleting type '%s' from hash table failed", name);
		return -1;
	}

	return 0;
}

int8_t gds_type_unregister_func(const char *name, const char *func_name)
{
	uint32_t hash;
	gds_type_t *type;

	if(name == NULL || func_name == NULL){
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	if(g_types == NULL){
		GDS_LOG_ERROR("User types not initialized");
		return -1;
	}

	hash = gds_type_hash(name);
	type = gds_type_list_get(g_types->type_list[hash], name);
	if(type == NULL){
		GDS_LOG_ERROR("Failed to retrieve type %s", name);
		return -1;
	}
	if(gds_func_list_del(&(type->func_list), func_name) < 0){
		GDS_LOG_ERROR("Failed to delete function %s for type %s", func_name, name);
		return -1;
	}

	return 0;
}

void gds_types_free(void)
{
	uint32_t i;

	if(g_types == NULL) return;

	for(i=0; i<g_types->size; i++){
		gds_type_list_free(g_types->type_list[i]);
		g_types->type_list[i] = NULL;
	}
	free(g_types->type_list);
	free(g_types);
	g_types = NULL;
}

void gds_types_print(void)
{
	uint32_t i;
	gds_type_list_node_t *tmp;

	if(g_types == NULL){
		fprintf(stderr, "User types not initialized\n");
		return;
	}

	for(i=0; i<g_types->size; i++){
		tmp = g_types->type_list[i];
		if(tmp) printf("[%d]", i);
		while(tmp != NULL){
			printf("\tname(\"%s\")\n", tmp->type->name);
			tmp = tmp->next;
		}
	}
}

