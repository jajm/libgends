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
 * Fichier		: deque.h                                            *
 * Description Brève	: Gestion d'une 'double-ended queue'                 *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/06/2010					     *
 *****************************************************************************
 * La file est implémentée en utilisant une liste chainée double.            *
 * Toutes les opérations (mises à part free et destroy) sont en temps        *
 * constant.                                                                 *
 *****************************************************************************/
#ifndef deque_h_included
#define deque_h_included

#include "dlist.h"

typedef dlist_t deque_t;

#define deque_new(type_name) \
	dlist_new(type_name)

#define deque_push_front(deque, data) \
	dlist_add_first(deque, data)
#define deque_push_back(deque, data) \
	dlist_add_last(deque, data)

#define deque_pop_front(deque, data) \
	dlist_pop_first(deque, data)
#define deque_pop_back(deque, data) \
	dlist_pop_last(deque, data)

#define deque_free(deque) \
	dlist_free(deque)
#define deque_destroy(deque) \
	dlist_destroy(deque)

#endif /* Not deque_h_included */

