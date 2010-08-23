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
 * Fichier           : slist_node.c                                          *
 * Description Brève : Nœud d'une liste simplement chainée                   *
 * Auteur            : Julian Maurice                                        *
 * Créé le           : 23/08/2010                                            *
 *****************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "slist_node.h"
#include "error.h"

slist_node_t *slnode_new(void *data)
{
	slist_node_t *node;

	assert(data != NULL);

	node = malloc(sizeof(slist_node_t));
	if(node){
		node->data = data;
		node->next = NULL;
	}else{
		Error("Memory allocation error");
	}

	return node;
}


void slnode_set_data(slist_node_t *node, void *data)
{
	assert(node != NULL);
	assert(data != NULL);

	node->data = data;
}

void *slnode_data(slist_node_t *node)
{
	assert(node != NULL);

	return node->data;
}


void slnode_free(slist_node_t *node)
{
	free(node);
}
