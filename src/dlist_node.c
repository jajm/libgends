
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
 * Fichier		: dlist_node.c                                       *
 * Description Brève	: Nœud d'une liste doublement chainée                *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 23/08/2010                                         *
 *****************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "dlist_node.h"
#include "error.h"

dlist_node_t *dlnode_new(void *data)
{
	dlist_node_t *newnode;

	assert(data != NULL);

	newnode = malloc(sizeof(dlist_node_t));
	if(newnode == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	newnode->data = data;
	newnode->next = NULL;
	
	return newnode;
}

void dlnode_set_data(dlist_node_t *node, void *data)
{
	assert(node != NULL);
	assert(data != NULL);

	node->data = data;
}

void *dlnode_data(dlist_node_t *node)
{
	assert(node != NULL);

	return node->data;
}

void dlnode_free(dlist_node_t *node)
{
	free(node);
}

