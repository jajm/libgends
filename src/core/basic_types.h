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
 * Fichier		: basic_types.h                                      *
 * Description Brève	: Liste de 'typedef' pour les types entiers          *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Les types définis ici utilisent les types de C99.                         *
 * Les types sont nommés aXX, où a est soit 'u', soit 's', selon qu'il soit  *
 * respectivement 'Unsigned', ou 'Signed'.                                   *
 * XX est la taille exacte du type, en bits. Par exemple, u32 est le type    *
 * d'un entier non signé qui occupe exactement 32 bits en mémoire.           *
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

