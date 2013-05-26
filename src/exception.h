/*
 * Copyright 2012-2013 Julian Maurice
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

#ifndef gds_exception_h_included
#define gds_exception_h_included

#include <libexception/exception.h>
#include "log.h"

#define GDS_THROW(exception, message, ...) \
	{ \
		GDS_LOG_ERROR(message, ##__VA_ARGS__); \
		throw(exception, message, ##__VA_ARGS__); \
	}

#define GDS_THROW_ALLOC_ERROR(size) \
	GDS_THROW(NotEnoughMemoryException, "failed to allocate %d bytes", size)

#define GDS_THROW_EMPTY_LIST() \
	GDS_THROW(EmptyListException, "list is empty")

#endif /* Not gds_exception_h_included */

