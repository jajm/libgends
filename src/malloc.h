/*
 * Copyright 2014 Julian Maurice
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

#ifndef gds_malloc_h_included
#define gds_malloc_h_included

#include <stdlib.h>

void * gds_malloc(size_t size);
void * gds_calloc(size_t nmemb, size_t size);
void * gds_realloc(void *ptr, size_t size);

#endif /* ! gds_malloc_h_included */
