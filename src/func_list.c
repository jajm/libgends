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
 * File                 : func_list.c                                        *
 * Short description    : Linked list of generic functions                   *
 *****************************************************************************
 * Linked list of pointer of intptr_t (*ptr)(void *, ...)                    *
 *****************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "func_ptr.h"
#include "func_list.h"

gds_func_list_node_t * gds_func_list_add(gds_func_list_node_t **head,
	const char *name, gds_func_ptr_t ptr)
{
	gds_func_list_node_t *newnode;
	size_t len;

	GDS_CHECK_ARG_NOT_NULL(head);
	GDS_CHECK_ARG_NOT_NULL(name);
	GDS_CHECK_ARG_NOT_NULL(ptr);

	newnode = malloc(sizeof(gds_func_list_node_t));
	if(newnode == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_func_list_node_t));
	}

	len = strlen(name);
	newnode->name = malloc(len+1);
	if(newnode->name == NULL) {
		free(newnode);
		GDS_THROW_ALLOC_ERROR(len+1);
	}
	strncpy(newnode->name, name, len+1);
	assert(newnode->name[len] == '\0');
	newnode->ptr = ptr;
	/* TODO Insert in order according to name */
	newnode->next = *head;
	*head = newnode;

	return newnode;
}

int8_t gds_func_list_del(gds_func_list_node_t **head, const char *name)
{
	gds_func_list_node_t *node, *prev = NULL;

	GDS_CHECK_ARG_NOT_NULL(head);
	GDS_CHECK_ARG_NOT_NULL(name);

	node = *head;
	while(node != NULL){
		if(strcmp(node->name, name) == 0)
			break;
		prev = node;
		node = node->next;
	}

	if(node == NULL){
		GDS_LOG_ERROR("Function %s does not exist", name);
		return -1;
	}

	if(node != NULL){
		if(prev == NULL) {
			*head = node->next;
		} else {
			prev->next = node->next;
		}
		free(node->name);
		free(node);
	}
	return 0;
}

void gds_func_list_free(gds_func_list_node_t *head)
{
	gds_func_list_node_t *node, *next;

	node = head;
	while(node != NULL){
		next = node->next;
		free(node->name);
		free(node);
		node = next;
	}
}

gds_func_ptr_t gds_func_list_get_ptr(gds_func_list_node_t *head,
	const char *name)
{
	gds_func_ptr_t func_ptr = NULL;
	gds_func_list_node_t *node;

	GDS_CHECK_ARG_NOT_NULL(name);

	node = head;
	while(node != NULL) {
		if(strcmp(node->name, name) == 0) {
			func_ptr = node->ptr;
		}
		node = node->next;
	}

	return func_ptr;
}

