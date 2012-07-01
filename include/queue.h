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
 * File              : queue.h                                               *
 * Short description : Queue management (FIFO, First In First Out)           *
 *****************************************************************************
 * Queue is implemented using a single-linked list.                          *
 * Elements are added at end of list (in constant time) and removed from     *
 * beginning of list (in constant time too).                                 *
 *****************************************************************************/
#ifndef queue_h_included
#define queue_h_included

#include "slist.h"

typedef gds_slist_t gds_queue_t;

#define gds_queue_new(type_name) \
	gds_slist_new(type_name)

#define gds_queue_push(queue, data, copy_data) \
	gds_slist_add_last(queue, data, copy_data)

#define gds_queue_pop(queue) \
	gds_slist_pop_first(queue)

#define gds_queue_free(queue, free_data) \
	gds_slist_free(queue, free_data)

#endif /* Not queue_h_included */

