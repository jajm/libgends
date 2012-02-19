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
 * File              : slist_node.c                                          *
 * Short description : Singly linked list node                               *
 *****************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "func_ptr.h"
#include "log.h"
#include "slist_node.h"

gds_slist_node_t *gds_slist_node_new(void *data, bool copy_data, gds_func_ptr_t alloc_f)
{
	gds_slist_node_t *node;

	if(data == NULL || (copy_data && alloc_f == NULL)) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	node = malloc(sizeof(gds_slist_node_t));
	if(node == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	if(copy_data) {
		node->data = (void *)alloc_f(data);
		if(node->data == NULL) {
			GDS_LOG_ERROR("Memory allocation error");
			free(node);
			return NULL;
		}
	} else {
		node->data = data;
	}
	node->next = NULL;

	return node;
}


int8_t gds_slist_node_set_data(gds_slist_node_t *node, void *data, bool free_old_data,
	gds_func_ptr_t free_f, bool copy_data, gds_func_ptr_t alloc_f)
{
	if(node == NULL || data == NULL || (copy_data && alloc_f == NULL)
	|| (free_old_data && free_f == NULL)) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	if(free_old_data) {
		free_f(node->data);
	}

	if(copy_data) {
		node->data = (void *)alloc_f(data);
		if(node->data == NULL) {
			GDS_LOG_ERROR("Memory allocation error");
			return -1;
		}
	} else {
		node->data = data;
	}

	return 0;
}

void *gds_slist_node_get_data(gds_slist_node_t *node, bool copy, gds_func_ptr_t alloc_f)
{
	void *data;

	if(node == NULL || (copy && alloc_f == NULL)) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	if(copy) {
		data = (void *)alloc_f(node->data);
		if(data == NULL) {
			GDS_LOG_ERROR("Memory allocation error");
			return NULL;
		}
	} else {
		data = node->data;
	}

	return data;
}

int8_t gds_slist_node_set_next(gds_slist_node_t *node, gds_slist_node_t *next)
{
	if(node == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	node->next = next;
	
	return 0;
}

gds_slist_node_t *gds_slist_node_get_next(gds_slist_node_t *node)
{
	if(node == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	return node->next;
}

void gds_slist_node_free(gds_slist_node_t *node, bool free_data, gds_func_ptr_t free_f)
{
	if(node && free_data) {
		if(free_f) {
			free_f(node->data);
		} else {
			GDS_LOG_ERROR("free_data is true but free_f is NULL");
		}
	}
	free(node);
}
