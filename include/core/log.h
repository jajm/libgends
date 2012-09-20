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

#define GDS_LOG_DOMAIN "libgends"

void
gds_log_init(uint8_t level);

uint8_t
gds_log_get_level(void);

#define GDS_LOG_FATAL(...) \
	lll_log_to_stream(stderr, "[%T][%d]FATAL: %m at %F:%L (%f)", \
		GDS_LOG_DOMAIN, gds_log_get_level(), GDS_LOG_LEVEL_FATAL, \
		__FILE__, __func__, __LINE__, __VA_ARGS__)

#define GDS_LOG_ERROR(...) \
	lll_log_to_stream(stderr, "[%T][%d]ERROR: %m at %F:%L (%f)", \
		GDS_LOG_DOMAIN, gds_log_get_level(), GDS_LOG_LEVEL_ERROR, \
		__FILE__, __func__, __LINE__, __VA_ARGS__)

#define GDS_LOG_WARNING(...) \
	lll_log_to_stream(stderr, "[%T][%d]WARNING: %m at %F:%L (%f)", \
		GDS_LOG_DOMAIN, gds_log_get_level(), GDS_LOG_LEVEL_WARNING, \
		__FILE__, __func__, __LINE__, __VA_ARGS__)

#define GDS_LOG_INFO(...) \
	lll_log_to_stream(stderr, "[%T][%d]INFO: %m at %F:%L (%f)", \
		GDS_LOG_DOMAIN, gds_log_get_level(), GDS_LOG_LEVEL_INFO, \
		__FILE__, __func__, __LINE__, __VA_ARGS__)

#define GDS_LOG_DEBUG(...) \
	lll_log_to_stream(stderr, "[%T][%d]DEBUG: %m at %F:%L (%f)", \
		GDS_LOG_DOMAIN, gds_log_get_level(), GDS_LOG_LEVEL_DEBUG, \
		__FILE__, __func__, __LINE__, __VA_ARGS__)

#endif /* log_h_included */

