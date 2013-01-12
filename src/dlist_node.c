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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "exception.h"
#include "check_arg.h"
#include "callbacks.h"
#include "log.h"
#include "dlist_node.h"

gds_dlist_node_t *gds_dlist_node_new(void *data)
{
	gds_dlist_node_t *newnode;

	GDS_CHECK_ARG_NOT_NULL(data);

	newnode = malloc(sizeof(gds_dlist_node_t));
	if(newnode == NULL){
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_dlist_node_t));
	}
	newnode->data = data;
	newnode->next = NULL;
	newnode->prev = NULL;

	return newnode;
}

int8_t gds_dlist_node_set_data(gds_dlist_node_t *node, void *data,
	gds_free_cb free_cb)
{
	GDS_CHECK_ARG_NOT_NULL(node);
	GDS_CHECK_ARG_NOT_NULL(data);

	if (free_cb != NULL) {
		free_cb(node->data);
	}

	node->data = data;

	return 0;
}

void * gds_dlist_node_get_data(gds_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->data;
}

int8_t gds_dlist_node_set_next(gds_dlist_node_t *node, gds_dlist_node_t *next)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->next = next;
	
	return 0;
}

gds_dlist_node_t *gds_dlist_node_get_next(gds_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->next;
}

int8_t gds_dlist_node_set_prev(gds_dlist_node_t *node, gds_dlist_node_t *prev)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->prev = prev;
	
	return 0;
}

gds_dlist_node_t *gds_dlist_node_get_prev(gds_dlist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->prev;
}

void gds_dlist_node_free(gds_dlist_node_t *node, gds_free_cb free_cb)
{
	if (node) {
		if (free_cb) {
			free_cb(node->data);
		}
		free(node);
	}
}

