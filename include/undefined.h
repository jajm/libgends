/*
 * Copyright 2013-2014 Julian Maurice
 *
 * This file is part of libgends
 *
 * libgends is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libgends is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef gds_undefined_h_included
#define gds_undefined_h_included

#define UNDEFINED ((void *) -1)

#define isdef(ptr) ((ptr) != UNDEFINED)

#define isset(ptr) ({ \
	void *gds_ptr = (ptr); \
	((gds_ptr != NULL) && (gds_ptr != UNDEFINED)); \
})

#endif /* ! gds_undefined_h_included */
