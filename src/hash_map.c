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

/*****************************************************************************
 * File                 : hash_map.c                                         *
 * Short description    : Hash table management                              *
 * Author               : Julian Maurice                                     *
 * Created on           : 2011-09-18                                         *
 *****************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "basic_types.h"
#include "types.h"
#include "funcs.h"
#include "error.h"
#include "hash_map.h"

hash_map_elt_t * hash_map_elt_new(const char *key, void *value)
{
	hash_map_elt_t *elt;
	size_t len;

	if(key == NULL){
		Error("Bad parameters");
		return NULL;
	}
	elt = malloc(sizeof(hash_map_elt_t));
	if(elt == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	len = strlen(key);
	elt->key = malloc(len+1);
	if(elt->key == NULL) {
		Error("Memory allocation error");
		free(elt);
		return NULL;
	}
	strncpy(elt->key, key, len+1);
	assert(elt->key[len] == '\0');
	elt->value = value;

	return elt;
}

void hash_map_elt_free(hash_map_elt_t *elt, func_ptr_t free_f)
{
	free(elt->key);
	if(free_f) free_f(elt->value);
	free(elt);
}

hash_map_list_node_t * hash_map_list_add(
	hash_map_list_t *hash_map_list,
        const char *key,
	void *value)
{
	hash_map_list_node_t *cur, *prev = NULL, *newnode = NULL;
	hash_map_elt_t *elt;
	s32 cmp;

	if(key == NULL || hash_map_list == NULL){
		Error("Bad parameters");
		return NULL;
	}
	cur = *hash_map_list;
	while(cur != NULL && (cmp = strcmp(cur->elt->key, key)) <= 0) {
		if(cmp == 0) {
			newnode = cur;
		}
		prev = cur;
		cur = cur->next;
	}
	if(newnode != NULL) {
		newnode->elt->value = value;
	} else {		
		elt = hash_map_elt_new(key, value);
		if(elt == NULL) {
			ErrorP("Failed to create hash_map_elt_t");
			return NULL;
		}
		newnode = malloc(sizeof(hash_map_list_node_t));
		if(newnode == NULL) {
			hash_map_elt_free(elt, NULL);
			Error("Memory allocation error");
			return NULL;
		}
		newnode->elt = elt;
		newnode->next = cur;
		if(prev != NULL) {
			prev->next = newnode;
		} else {
			*hash_map_list = newnode;
		}
	}

	return newnode;
}

void * hash_map_list_get(hash_map_list_t hash_map_list, const char *key)
{
	hash_map_list_node_t *cur;

	if(key == NULL){
		Error("Bad parameters");
		return NULL;
	}
	cur = hash_map_list;
	while(cur != NULL) {
		if(cur->elt && strcmp(cur->elt->key, key) == 0) {
			break;
		}
		cur = cur->next;
	}
	if(cur != NULL && cur->elt != NULL) {
		return cur->elt->value;
	}

	return NULL;
}
	

void hash_map_list_del(
	hash_map_list_t *hash_map_list,
	const char *key,
	func_ptr_t free_f)
{
	hash_map_list_node_t *prev = NULL, *cur;

	if(hash_map_list == NULL || key == NULL){
		Error("Bad parameters");
		return;
	}
	cur = *hash_map_list;
	while(cur != NULL) {
		if(cur->elt && strcmp(cur->elt->key, key) == 0) {
			break;
		}
		prev = cur;
		cur = cur->next;
	}
	if(cur != NULL) {
		if(prev) {
			prev->next = cur->next;
		} else {
			*hash_map_list = cur->next;
		}
		hash_map_elt_free(cur->elt, free_f);
		free(cur);
	}
}

u8 hash_map_list_exists(hash_map_list_t hash_map_list, const char *key)
{
	hash_map_list_node_t *cur;

	if(key == NULL){
		Error("Bad parameters");
		return 0;
	}
	cur = hash_map_list;
	while(cur != NULL) {
		if(cur->elt && strcmp(cur->elt->key, key) == 0) {
			break;
		}
		cur = cur->next;
	}
	if(cur != NULL) {
		return 1;
	}

	return 0;
}
	
void hash_map_list_free(hash_map_list_t hash_map_list, func_ptr_t free_f)
{
	hash_map_list_node_t *cur, *next;

	cur = hash_map_list;
	while(cur != NULL) {
		next = cur->next;
		hash_map_elt_free(cur->elt, free_f);
		free(cur);
		cur = next;
	}
}

u32 hash_map_default_hash_func(const char *key, u32 map_size)
{
	u32 i;
	u32 hash = 0;
	size_t len;

	if(key == NULL || map_size == 0) {
		Error("Bad parameters");
		return 0;
	}
	len = strlen(key);
	for(i=0; i<len; i++) {
		hash += key[i];
	}
	hash %= map_size;

	return hash;
}

hash_map_t * hash_map_new(
	const char *type_name,
	u32 map_size,
	hash_func_t hash_func)
{
	hash_map_t *hash_map;
	size_t len;

	if(type_name == NULL || map_size == 0) {
		Error("Bad parameters");
		return NULL;
	}
	hash_map = malloc(sizeof(hash_map_t));
	if(hash_map == NULL) {
		Error("Memory allocation error");
		return NULL;
	}
	len = strlen(type_name);
	hash_map->type_name = malloc(len+1);
	if(hash_map->type_name == NULL) {
		Error("Memory allocation error");
		free(hash_map);
		return NULL;
	}
	strncpy(hash_map->type_name, type_name, len+1);
	assert(hash_map->type_name[len] == '\0');
	hash_map->map = malloc(map_size*sizeof(hash_map_list_node_t *));
	if(hash_map->map == NULL) {
		Error("Memory allocation error");
		free(hash_map->type_name);
		free(hash_map);
		return NULL;
	}
	hash_map->map_size = map_size;
	if(hash_func != NULL) {
		hash_map->hash_func = hash_func;
	} else {
		hash_map->hash_func = &hash_map_default_hash_func;
	}

	return hash_map;
}

hash_map_elt_t * hash_map_set(
	hash_map_t *h,
	const char *key,
	void *value)
{
	u32 hash;
	hash_map_list_node_t *node;

	if(h == NULL || key == NULL) {
		Error("Bad parameters");
		return NULL;
	}
	hash = h->hash_func(key, h->map_size);
	hash = hash % h->map_size;
	node = hash_map_list_add(&(h->map[hash]), key, value);
	if(node == NULL) {
		ErrorP("Failed to set value for '%s' key", key);
		return NULL;
	}

	return node->elt;
}

void * hash_map_get(hash_map_t *h, const char *key)
{
	u32 hash;

	if(h == NULL || key == NULL) {
		Error("Bad parameters");
		return NULL;
	}
	hash = h->hash_func(key, h->map_size);
	hash = hash % h->map_size;
	
	return hash_map_list_get(h->map[hash], key);
}

u8 hash_map_exists(hash_map_t *h, const char *key)
{
	u32 hash;

	if(h == NULL || key == NULL) {
		Error("Bad parameters");
		return 0;
	}
	hash = h->hash_func(key, h->map_size);
	hash = hash % h->map_size;

	return hash_map_list_exists(h->map[hash], key);
}

void hash_map_del(hash_map_t *h, const char *key)
{
	u32 hash;
	func_ptr_t free_f;

	if(h == NULL || key == NULL) {
		Error("Bad parameters");
		return;
	}
	hash = h->hash_func(key, h->map_size);
	hash = hash % h->map_size;

	free_f = type_get_func(h->type_name, "free");
	hash_map_list_del(&(h->map[hash]), key, free_f);
}

void hash_map_free(hash_map_t *h)
{
	u32 i;
	func_ptr_t free_f;

	if(h != NULL) {
		free_f = type_get_func(h->type_name, "free");
		for(i=0; i<h->map_size; i++) {
			hash_map_list_free(h->map[i], free_f);
		}
		free(h->map);
	}
}

void hash_map_print(hash_map_t *h)
{
	func_ptr_t print_f;
	hash_map_list_node_t *cur;
	u32 i;
	size_t len;

	if(h == NULL) {
		Error("Bad parameters");
		return;
	}
	print_f = type_get_func(h->type_name, "print");
	
	for(i=0; i<h->map_size; i++){
		cur = h->map[i];
		while(cur != NULL && cur->elt != NULL){
			len = strlen(cur->elt->key);
			printf("'%s' => ", cur->elt->key);
			if(print_f) {
				print_f(cur->elt->value, len+6);
			} else {
				printf("%p", cur->elt->value);
			}
			printf(",\n");
			cur = cur->next;
		}
	}
}
