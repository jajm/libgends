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

/*
 * File              : hash_functions.h
 * Short description : Common hash algorithms
 */

#ifndef gds_hash_functions_h_included
#define gds_hash_functions_h_included

/* Uses djb2 algorithm
 *
 * http://www.cse.yorku.ca/~oz/hash.html#djb2
 */
unsigned long gds_hash_djb2(const char *s);

/* Uses sdbm algorithm
 *
 * http://www.cse.yorku.ca/~oz/hash.html#sdbm
 */
unsigned long gds_hash_sdbm(const char *s);

#endif /* ! gds_hash_functions_h_included */
