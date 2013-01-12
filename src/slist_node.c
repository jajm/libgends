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
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "slist_node.h"
#include "callbacks.h"

gds_slist_node_t *gds_slist_node_new(void *data)
{
	gds_slist_node_t *node;

	GDS_CHECK_ARG_NOT_NULL(data);

	node = malloc(sizeof(gds_slist_node_t));
	if (node == NULL) {
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_slist_node_t));
	}
	node->data = data;
	node->next = NULL;

	return node;
}


int8_t gds_slist_node_set_data(gds_slist_node_t *node, void *data,
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

void *gds_slist_node_get_data(gds_slist_node_t *node)
{
	void *data;

	GDS_CHECK_ARG_NOT_NULL(node);

	data = node->data;

	return data;
}

int8_t gds_slist_node_set_next(gds_slist_node_t *node, gds_slist_node_t *next)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	node->next = next;
	
	return 0;
}

gds_slist_node_t *gds_slist_node_get_next(gds_slist_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->next;
}

void gds_slist_node_free(gds_slist_node_t *node, gds_free_cb free_cb)
{
	if(free_cb != NULL && node != NULL) {
		free_cb(node->data);
	}
	free(node);
}
