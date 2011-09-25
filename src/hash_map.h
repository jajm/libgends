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
 * File                 : hash_map.h                                         *
 * Short description    : Hash table management                              *
 * Author               : Julian Maurice                                     *
 * Created on           : 2011-09-11                                         *
 *****************************************************************************/

#include <stdlib.h>
#include "basic_types.h"
#include "types.h"
#include "error.h"

typedef struct {
	char *key;
	void *value;
} hash_map_elt_t;

typedef struct hash_map_list_node_s {
	hash_map_elt_t *elt;
	struct hash_map_list_node_s *next;
} hash_map_list_node_t, *hash_map_list_t;

typedef u32 (*hash_func_t)(const char *, u32);

typedef struct {
	char *type_name;
	u32 map_size;
	hash_func_t hash_func;
	hash_map_list_node_t **map;
} hash_map_t;

#ifdef __cplusplus
extern "C" {
#endif

hash_map_t * hash_map_new(const char *type_name, u32 map_size, hash_func_t hash_func);
hash_map_elt_t * hash_map_set(hash_map_t *h, const char *key, void *value);
void * hash_map_get(hash_map_t *h, const char *key);
u8 hash_map_exists(hash_map_t *h, const char *key);
void hash_map_del(hash_map_t *h, const char *key);
void hash_map_free(hash_map_t *h);

void hash_map_print(hash_map_t *h);

#ifdef __cplusplus
}
#endif

