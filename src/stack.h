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

#include "slist.h"

typedef slist_t stack_t;

#define stack_new(type_name) slist_new(type_name)
#define stack_push(stack, data) slist_add_first(stack, data)
#define stack_pop(stack) slist_pop_first(stack)
#define stack_free(stack) slist_free(stack)
#define stack_destroy(stack) slist_destroy(stack)

#endif /* Not stack_h_included */

