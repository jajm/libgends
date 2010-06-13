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
 * Fichier		: queue.h                                            *
 * Description Brève	: Gestion d'une file (FIFO, First In First Out)      *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/06/2010					     *
 *****************************************************************************
 * La file est implémentée en utilisant une liste chainée simple.            *
 * Les éléments sont ajoutés en fin de liste (temps constant) et retirés en  *
 * tête de liste (temps constant).                                           *
 *****************************************************************************/
#ifndef queue_h_included
#define queue_h_included

#include "slist.h"

typedef slist_t queue_t;

#define queue_new(type_name) slist_new(type_name)
#define queue_push(queue, data) slist_add_last(queue, data)
#define queue_pop(queue) slist_pop_first(queue)
#define queue_free(queue) slist_free(queue)
#define queue_destroy(queue) slist_destroy(queue)

#endif /* Not queue_h_included */

