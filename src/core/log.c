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

#include <stdint.h>
#include "log.h"

uint8_t gds_log_level = 0;

void gds_log_init(uint8_t level)
{
	gds_log_level = level;
}

uint8_t gds_log_get_level(void)
{
	return gds_log_level;
}
