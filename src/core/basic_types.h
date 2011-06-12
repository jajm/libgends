/*****************************************************************************
 * Copyright (C) 2010 Julian Maurice                                         *
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
 * File                 : basic_types.h                                      *
 * Short description    : List of typedefs for integer types                 *
 * Author               : Julian Maurice                                     *
 * Created on           : 2010-03-01                                         *
 *****************************************************************************
 * Types defined here use C99 types.                                         *
 * Types are named aXX, where 'a' is 'u' if type is unsigned, or 's' (signed)*
 * otherwise.                                                                *
 * 'XX' is the exact size of type, in bits. For example, u32 is an unsigned  *
 * integer which is 32 bits long.                                            *
 *****************************************************************************/

#ifndef basic_types_h_included
#define basic_types_h_included

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#endif /* Not basic_types_h_included */

