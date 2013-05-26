/*
 * Copyright 2011-2013 Julian Maurice
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

/*
 * File              : foreach.h
 * Short description : 'Fake' foreach using iterators
 */

#ifndef foreach_h_included
#define foreach_h_included

#include <stdbool.h>
#include "iterator.h"

#define foreach(var, iterator) \
	for(bool _loop_ok = !gds_iterator_reset(iterator); _loop_ok; _loop_ok = false) \
	while (!gds_iterator_step(iterator) && ((var = gds_iterator_get(iterator)) || !var))

#endif /* Not foreach_h_included */

