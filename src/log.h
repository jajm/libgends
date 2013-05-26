/*
 * Copyright 2010-2013 Julian Maurice
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
 * File              : log.h
 * Short description : libgends logging system
 */

#ifndef gds_log_h_included
#define gds_log_h_included

#include <stdint.h>
#include <stdlib.h>
#include <lll/lll.h>

#define gds_log(level, level_str, ...) do { \
	char *verbosity_str = getenv("LIBGENDS_VERBOSITY"); \
	int verbosity = verbosity_str ? atoi(verbosity_str) : 0; \
	if (level <= verbosity) { \
		lll_fprint(stderr, "[%T][libgends]?0|$0:|| %m at $1:$2 ($3)", \
			"%s", level_str, "%s", __FILE__, "%d", __LINE__, \
			"%s", __func__, NULL, NULL, __VA_ARGS__); \
	} \
} while(0)

#define GDS_LOG_FATAL(...) \
	gds_log(1, "fatal", __VA_ARGS__)

#define GDS_LOG_ERROR(...) \
	gds_log(2, "error", __VA_ARGS__)

#define GDS_LOG_WARNING(...) \
	gds_log(3, "warning", __VA_ARGS__)

#define GDS_LOG_INFO(...) \
	gds_log(4, "info", __VA_ARGS__)

#define GDS_LOG_DEBUG(...) \
	gds_log(5, "debug", __VA_ARGS__)

#endif /* gds_log_h_included */

