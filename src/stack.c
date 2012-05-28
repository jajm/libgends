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
 * File              : stack.c                                               *
 * Short description : Stack management (LIFO, Last In First Out)            *
 *****************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "func_list.h"
#include "slist_node.h"
#include "stack.h"
#include "types.h"

gds_stack_t *gds_stack_new(const char *type_name)
{
	gds_stack_t *S;
	size_t len;

	if(type_name == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	S = malloc(sizeof(gds_stack_t));
	if(S == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	len = strlen(type_name);
	S->type_name = malloc(len+1);
	if(S->type_name == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		free(S);
		return NULL;
	}
	strncpy(S->type_name, type_name, len+1);
	assert(S->type_name[len] == '\0');
	S->head = NULL;

	return S;
}

int8_t gds_stack_push(gds_stack_t *S, void *data, bool copy_data)
{
	gds_slist_node_t *newnode;
	gds_alloc_cb alloc_cb = NULL;

	if(S == NULL || data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	if(copy_data) {
		alloc_cb = (gds_alloc_cb)gds_type_get_func(S->type_name, "alloc");
	}

	newnode = gds_slist_node_new(data, copy_data, alloc_cb);
	if(newnode == NULL){
		GDS_LOG_ERROR("Failed to create the node");
		return -1;
	}
	newnode->next = S->head;
	S->head = newnode;

	return 0;
}

void *gds_stack_pop(gds_stack_t *S)
{
	void *data;
	gds_slist_node_t *next;

	if(S == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	if(S->head == NULL){
		GDS_LOG_ERROR("Stack is empty");
		return NULL;
	}

	next = S->head->next;
	data = gds_slist_node_get_data(S->head, false, 0);
	gds_slist_node_free(S->head, false, NULL);
	S->head = next;

	return data;
}

void gds_stack_free(gds_stack_t *S, bool free_data)
{
	gds_slist_node_t *tmp, *tmp2;
	gds_free_cb free_cb = NULL;

	if(S){
		if(free_data) {
			free_cb = (gds_free_cb)gds_type_get_func(S->type_name, "free");
		}
		tmp = S->head;
		while(tmp != NULL){
			tmp2 = tmp->next;
			gds_slist_node_free(tmp, free_data, free_cb);
			tmp = tmp2;
		}
		free(S->type_name);
		free(S);
	}
}

