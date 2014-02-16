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

#include "hash_functions.h"

unsigned long gds_hash_djb2(const char *s)
{
	unsigned long hash = 5381;
	int c;

	if (!s) return 0;

	while ((c = *s++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

unsigned long gds_hash_sdbm(const char *s)
{
	unsigned long hash = 0;
	int c;

	if (!s) return 0;

	while ((c = *s++))
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}
