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
 * Fichier		: stack.h                                            *
 * Description Brève	: Gestion d'une pile (LIFO, Last In First Out)       *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/06/2010					     *
 *****************************************************************************
 * La pile est implémentée en utilisant une liste chainée simple.            *
 * Les éléments sont ajoutés et retirés en tête de liste, donc en temps      *
 * constant.                                                                 *
 *****************************************************************************/

#ifndef stack_h_included
#define stack_h_included

#include "slist_node.h"

typedef struct{
	char *type_name;
	slist_node_t *head;
} stack_t;

stack_t *stack_new(const char *type_name);
s8 stack_push(stack_t *S, void *data);
void *stack_pop(stack_t *S);
void stack_free(stack_t *S);
void stack_destroy(stack_t *S);

#endif /* Not stack_h_included */

