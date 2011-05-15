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

 /****************************************************************************
  * Fichier		: types.c                                            *
  * Description Brève	: Gestion de types 'utilisateur'                     *
  * Auteur		: Julian Maurice                                     *
  * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Les types 'utilisateur' sont des nouveaux types définis par l'utilisateur.*
 * Ils sont décrits par un nom unique, une taille en octets et une liste de  *
 * fonctions identifiées par leur nom.                                       *
 *****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "error.h"
#include "types.h"

#define DEFAULT_HT_SIZE	1000

typedef struct{
	char *name;
	u32 size;
	funcs_t funcs;
} type_t;

type_t *type_new(const char *name, u32 size)
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

s8 typelist_add(typelist_t *tlist, type_t *type)
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

s8 typelist_del(typelist_t *tlist, const char *name)
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
	u32 size;
};

struct types_t *g_types = NULL;

u32 type_hash(const char *name)
{
	u32 hash = 0;
	u32 i, size;

	if(name == NULL) return 0;

	size = strlen(name);
	for(i=0; i<size; i++){
		hash += name[i] * (i+1);
	}
	hash %= g_types->size;

	return hash;
}

s8 types_init(u32 size)
{
	u32 i;
	
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
	
	/* Types par défaut */
	type_reg("u8", sizeof(u8));
	type_reg("u16", sizeof(u16));
	type_reg("u32", sizeof(u32));
	type_reg("u64", sizeof(u64));
	type_reg("s8", sizeof(s8));
	type_reg("s16", sizeof(s16));
	type_reg("s32", sizeof(s32));
	type_reg("s64", sizeof(s64));
	type_reg("float", sizeof(float));
	type_reg("double", sizeof(double));
	type_reg("char", sizeof(char));

	return 0;
}

s8 type_reg(const char *name, u32 size)
{
	s8 tla;
	u32 hash;
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

s8 type_reg_func(const char *name, const char *func_name, func_ptr_t func_ptr)
{
	u32 hash;
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
	u32 hash;
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

s8 type_unreg(const char *name)
{
	u32 hash;

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

s8 type_unreg_func(const char *name, const char *func_name)
{
	u32 hash;
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

u32 type_sizeof(const char *name)
{
	u32 hash;
	u32 size = 0;
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

	if(size == 0) Error("Type '%s' doesn't exist");

	return size;
}

void types_free(void)
{
	u32 i;

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
	u32 i;
	typelist_node_t *tmp;

	if(g_types == NULL){
		fprintf(stderr, "User types not initialized\n");
		return;
	}

	for(i=0; i<g_types->size; i++){
		tmp = g_types->types[i];
		if(tmp) printf("[%d]", i);
		while(tmp != NULL){
			printf("\tname(\"%s\"), size(\"%d\"), "
				"type_sizeof(\"%d\")\n",
				tmp->type->name, tmp->type->size,
				type_sizeof(tmp->type->name));
			tmp = tmp->next;
		}
	}
}
