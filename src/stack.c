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
 * Fichier		: stack.c                                            *
 * Description Brève	: Gestion d'une pile (LIFO, Last In First Out)       *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/06/2010					     *
 *****************************************************************************
 * La pile est implémentée en utilisant une liste chainée simple.            *
 * Les éléments sont ajoutés et retirés en tête de liste, donc en temps      *
 * constant.                                                                 *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack.h"
#include "types.h"
#include "error.h"

stack_t *stack_new(const char *type_name)
{
	stack_t *S;
	size_t len;

	assert(type_name != NULL);

	S = malloc(sizeof(stack_t));
	if(S == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	len = strlen(type_name);
	S->type_name = malloc(len+1);
	if(S->type_name == NULL){
		Error("Memory allocation error");
		free(S);
		return NULL;
	}
	strncpy(S->type_name, type_name, len+1);
	S->head = NULL;

	return S;
}

s8 stack_push(stack_t *S, void *data)
{
	slist_node_t *newnode;

	assert(S != NULL);
	assert(data != NULL);
	
	newnode = slnode_new(data);
	if(newnode == NULL){
		ErrorP("Failed to create the node");
		return -1;
	}
	newnode->next = S->head;
	S->head = newnode;
	
	return 0;
}

void *stack_pop(stack_t *S)
{
	void *data;
	slist_node_t *next;

	assert(S != NULL);
	if(S->head == NULL){
		Error("Stack is empty");
		return NULL;
	}

	next = S->head->next;
	data = slnode_data(S->head);
	slnode_free(S->head);
	S->head = next;

	return data;
}

void stack_free(stack_t *S)
{
	slist_node_t *tmp, *tmp2;
	func_ptr_t free_f;

	if(S){
		free_f = type_get_func(S->type_name, "free");
		free(S->type_name);
		tmp = S->head;
		while(tmp != NULL){
			tmp2 = tmp->next;
			if(free_f) free_f(slnode_data(tmp));
			slnode_free(tmp);
			tmp = tmp2;
		}
	}
}

