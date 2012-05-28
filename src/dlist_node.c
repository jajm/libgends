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
 * File              : dlist_node.c                                          *
 * Short description : Double linked list node management                    *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "callbacks.h"
#include "log.h"
#include "dlist_node.h"

gds_dlist_node_t *gds_dlist_node_new(void *data, bool copy_data,
	gds_alloc_cb alloc_cb)
{
	gds_dlist_node_t *newnode;

	if(data == NULL || (copy_data && alloc_cb == NULL)) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	newnode = malloc(sizeof(gds_dlist_node_t));
	if(newnode == NULL){
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	if(copy_data) {
		newnode->data = alloc_cb(data);
		if(newnode->data == NULL) {
			GDS_LOG_ERROR("Memory allocation error");
			return NULL;
		}
	} else {
		newnode->data = data;
	}
	newnode->next = NULL;
	newnode->prev = NULL;

	return newnode;
}

int8_t gds_dlist_node_set_data(gds_dlist_node_t *node, void *data,
	bool free_old_data, gds_free_cb free_cb, bool copy_data,
	gds_alloc_cb alloc_cb)
{
	if(node == NULL || data == NULL || (copy_data && alloc_cb == NULL)) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	if(free_old_data) {
		if(free_cb) {
			free_cb(node->data);
		} else {
			free(node->data);
		}
	}

	if(copy_data) {
		node->data = alloc_cb(data);
		if(node->data == NULL) {
			GDS_LOG_ERROR("Memory allocation error");
			return -1;
		}
	} else {
		node->data = data;
	}

	return 0;
}

void *gds_dlist_node_get_data(gds_dlist_node_t *node, bool copy_data,
	gds_alloc_cb alloc_cb)
{
	void *data;

	if(node == NULL || (copy_data && alloc_cb == NULL)) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	if(copy_data) {
		data = alloc_cb(node->data);
		if(data == NULL) {
			GDS_LOG_ERROR("Memory allocation error");
			return NULL;
		}
	} else {
		data = node->data;
	}

	return data;
}

int8_t gds_dlist_node_set_next(gds_dlist_node_t *node, gds_dlist_node_t *next)
{
	if(node == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	node->next = next;
	
	return 0;
}

gds_dlist_node_t *gds_dlist_node_get_next(gds_dlist_node_t *node)
{
	if(node == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	return node->next;
}

int8_t gds_dlist_node_set_prev(gds_dlist_node_t *node, gds_dlist_node_t *prev)
{
	if(node == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	node->prev = prev;
	
	return 0;
}

gds_dlist_node_t *gds_dlist_node_get_prev(gds_dlist_node_t *node)
{
	if(node == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	return node->prev;
}

void gds_dlist_node_free(gds_dlist_node_t *node, bool free_data,
	gds_free_cb free_cb)
{
	if(node) {
		if(free_data && free_cb) {
			free_cb(node->data);
		} else if (free_data) {
			GDS_LOG_ERROR("free_data is true but free_cb is NULL");
		}

		free(node);
	}
}

