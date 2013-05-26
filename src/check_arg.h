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

#ifndef check_arg_h_included
#define check_arg_h_included

#include "exception.h"

#define GDS_CHECK_ARG_NOT_NULL(arg) \
	if ((arg) == NULL) \
		GDS_THROW(BadArgumentException, "argument '%s' is NULL", #arg)

#define GDS_CHECK_ARG_NOT_ZERO(arg) \
	if ((arg) == 0) \
		GDS_THROW(BadArgumentException, "argument '%s' == 0", #arg)

#endif /* Not check_arg_h_included */

