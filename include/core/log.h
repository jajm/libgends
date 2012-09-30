/*****************************************************************************
 * Copyright (C) 2012 Julian Maurice                                         *
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
 * File                 : log.h                                              *
 * Short description    : libgends logging system                            *
 *****************************************************************************/

#ifndef log_h_included
#define log_h_included

#include <stdint.h>
#include <lll/lll.h>

#define GDS_LOG_LEVEL_FATAL 1
#define GDS_LOG_LEVEL_ERROR 2
#define GDS_LOG_LEVEL_WARNING 3
#define GDS_LOG_LEVEL_INFO 4
#define GDS_LOG_LEVEL_DEBUG 5

void gds_log_init(uint8_t level);

uint8_t gds_log_get_level(void);
const char * gds_log_get_level_str(uint8_t level);

#define gds_log(level, ...) \
	if (level <= gds_log_get_level()) \
		lll_fprint(stderr, "[%T][libgends]?0|$0:|| %m at $1:$2 ($3)", \
			"%s", gds_log_get_level_str(level), "%s", __FILE__, \
			"%d", __LINE__, "%s", __func__, NULL, NULL, __VA_ARGS__)

#define GDS_LOG_FATAL(...) \
	gds_log(GDS_LOG_LEVEL_FATAL, __VA_ARGS__)

#define GDS_LOG_ERROR(...) \
	gds_log(GDS_LOG_LEVEL_ERROR, __VA_ARGS__)

#define GDS_LOG_WARNING(...) \
	gds_log(GDS_LOG_LEVEL_WARNING, __VA_ARGS__)

#define GDS_LOG_INFO(...) \
	gds_log(GDS_LOG_LEVEL_INFO, __VA_ARGS__)

#define GDS_LOG_DEBUG(...) \
	gds_log(GDS_LOG_LEVEL_DEBUG, __VA_ARGS__)

#endif /* log_h_included */

