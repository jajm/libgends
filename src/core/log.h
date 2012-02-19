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

#define GDS_LOG_LEVEL_FATAL 1
#define GDS_LOG_LEVEL_ERROR 2
#define GDS_LOG_LEVEL_WARNING 3
#define GDS_LOG_LEVEL_INFO 4
#define GDS_LOG_LEVEL_DEBUG 5

int8_t
gds_log_init(uint8_t level);

void
gds_log_print(
	uint8_t level,
	const char *file,
	const char *func,
	uint32_t line,
	const char *msg,
	...
);

#define GDS_LOG_FATAL(msg, ...) \
	gds_log_print(GDS_LOG_LEVEL_FATAL, __FILE__, __func__, \
	__LINE__, msg, ##__VA_ARGS__)

#define GDS_LOG_ERROR(msg, ...) \
	gds_log_print(GDS_LOG_LEVEL_ERROR, __FILE__, __func__, \
	__LINE__, msg, ##__VA_ARGS__)

#define GDS_LOG_WARNING(msg, ...) \
	gds_log_print(GDS_LOG_LEVEL_WARNING, __FILE__, __func__, \
	__LINE__, msg, ##__VA_ARGS__)

#define GDS_LOG_INFO(msg, ...) \
	gds_log_print(GDS_LOG_LEVEL_INFO, __FILE__, __func__, \
	__LINE__, msg, ##__VA_ARGS__)

#define GDS_LOG_DEBUG(msg, ...) \
	gds_log_print(GDS_LOG_LEVEL_DEBUG, __FILE__, __func__, \
	__LINE__, msg, ##__VA_ARGS__)

#endif /* log_h_included */

