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
 * File                 : log.c                                              *
 * Short description    : libgends logging system                            *
 *****************************************************************************/

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include "log.h"

uint8_t gds_log_level = 0;
const char * gds_log_level_label[] = {
	"fatal",
	"error",
	"warning",
	"info",
	"debug"
};

int8_t gds_log_init(uint8_t level)
{
	gds_log_level = level;
	return 0;
}

void gds_log_print(uint8_t level, const char *file, const char *func,
	uint32_t line, const char *msg, ...)
{
	va_list va_ptr;
	if(level > 0 && level <= gds_log_level) {
		va_start(va_ptr, msg);
		fprintf(stderr, "[%s]", gds_log_level_label[level-1]);
		if(file) {
			fprintf(stderr, " In file %s:%d", file, line);
		}
		if(func) {
			fprintf(stderr, " (%s)", func);
		}
		if(file || func) {
			fprintf(stderr, " > ");
		}
		vfprintf(stderr, msg, va_ptr);
		fprintf(stderr, "\n");
		va_end(va_ptr);
	}
}
