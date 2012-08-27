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
 * File              : typed_deque.h                                         *
 * Short description : Double-Ended Queue management                         *
 *****************************************************************************
 * Queue is implemented using a double-linked list.                          *
 * All operations (except for free) are executed in constant time            *
 *****************************************************************************/
#ifndef gds_typed_deque_h_included
#define gds_typed_deque_h_included

#include "typed_dlist.h"

typedef gds_typed_dlist_t gds_typed_deque_t;

#define gds_typed_deque_new(type_name) \
	gds_dlist_new(type_name)

#define gds_typed_deque_push_front(deque, data, copy_data) \
	gds_dlist_add_first(deque, data, copy_data)
#define gds_typed_deque_push_back(deque, data, copy_data) \
	gds_dlist_add_last(deque, data, copy_data)

#define gds_typed_deque_pop_front(deque) \
	gds_dlist_pop_first(deque)
#define gds_typed_deque_pop_back(deque) \
	gds_dlist_pop_last(deque)

#define gds_typed_deque_free(deque, free_data) \
	gds_dlist_free(deque, free_data)

#endif /* Not gds_typed_deque_h_included */

