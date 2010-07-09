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
 * Fichier		: container.c                                        *
 * Brève Description	: Conteneur de donnée générique                      *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Un conteneur peut contenir une et une seule donnée générique, représentée *
 * par un pointeur vers la donnée, et le nom du type de la donnée            *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "error.h"
#include "container.h"

container_t *container(const char *type_name, const void *data_ptr)
{
	container_t *g;
	u32 data_size;
	size_t length;

	if(type_name == NULL || data_ptr == NULL){
		Error("Bad parameter value");
		return NULL;
	}

	data_size = type_sizeof(type_name);
	if(data_size == 0){
		ErrorP("Failed to retrieve '%s' size", type_name);
		return NULL;
	}
	length = strlen(type_name);

	g = malloc(sizeof(container_t));
	if(g == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	g->data_ptr = malloc(data_size);
	if(g->data_ptr == NULL){
		free(g);
		Error("Memory allocation error");
		return NULL;
	}
	g->type_name = malloc(length+1);
	if(g->type_name == NULL){
		free(g->data_ptr);
		free(g);
		Error("Memory allocation error");
		return NULL;
	}
	memmove(g->data_ptr, data_ptr, data_size);
	strncpy(g->type_name, type_name, length+1);
	
	return g;
}

s8 container_affect(container_t **g, const char *type_name, const void *data_ptr)
{
	u32 data_size;
	size_t length;

	if(g == NULL || type_name == NULL || data_ptr == NULL){
		Error("Bad parameters");
		return -1;
	}

	data_size = type_sizeof(type_name);
	if(data_size == 0){
		ErrorP("Failed to retrieve '%s' size", type_name);
		return -1;
	}
	length = strlen(type_name);
	
	if(*g == NULL){
		*g = container(type_name, data_ptr);
	}else if(strcmp((*g)->type_name, type_name) == 0){
		/* Pas de réallocation à faire */
		memmove((*g)->data_ptr, data_ptr, data_size);
	}else{
		(*g)->type_name = realloc((*g)->type_name, length);
		strncpy((*g)->type_name, type_name, length);
		if(type_sizeof((*g)->type_name) != data_size){
			(*g)->data_ptr = realloc((*g)->data_ptr, data_size);
			if((*g)->data_ptr == NULL){
				Error("Memory allocation error");
				return -1;
			}
		}
		memmove((*g)->data_ptr, data_ptr, data_size);
	}

	return 0;
}

s8 container_copy(container_t **to, const container_t *from)
{
	return container_affect(to, from->type_name, from->data_ptr);
}

s32 container_cmp(const container_t *g1, const container_t *g2)
{
	func_ptr_t cmp_f;
	u32 size1, size2;
	s32 cmp;
	void *ptr;

	/* S'ils sont tous les 2 à NULL, on dira qu'ils sont égaux */
	if(g1 == NULL && g2 == NULL){
		cmp = 0;
	/* Si l'un des deux est à NULL, c'est l'autre le plus grand */
	}else if(g1 == NULL && g2 != NULL){
		cmp = -1;
	}else if(g1 != NULL && g2 == NULL){
		cmp = 1;
	/* Si aucun des deux n'est à NULL, qu'ils sont du même type,
	 * et qu'ils possèdent une fonction de comparaison, on utilise
	 * cette dernière */
	}else if((cmp_f = type_get_func(g1->type_name, "cmp")) != NULL
	  && strcmp(g1->type_name, g2->type_name) == 0){
		cmp = cmp_f(g1->data_ptr, g2->data_ptr);
	/* Sinon, on compare d'abord la taille,
	 * et ensuite la valeur des données */
	}else{
		size1 = type_sizeof(g1->type_name);
		size2 = type_sizeof(g2->type_name);
		if(size1 == size2){
			cmp = memcmp(g1->data_ptr, g2->data_ptr, size1);
		}else{
			u32 diff = (size1<size2)?(size2-size1):(size1-size2);
			u32 min = size1<size2 ? size1 : size2;
			cmp = memcmp(g1->data_ptr, g2->data_ptr, min);
			ptr = calloc(diff, 1);
			if(ptr){
				if(size1 < size2)
					cmp+=memcmp(ptr,g2->data_ptr+min,diff);
				else
					cmp+=memcmp(g1->data_ptr+min,ptr,diff);
				free(ptr);
			}else{
				cmp += size1 - size2;
			}
		}
	}
	return cmp;
}

u32 container_size(const container_t *g)
{
	u32 data_size = 0;
	if(g != NULL){
		data_size = type_sizeof(g->type_name);
	}
	return data_size;
}

void container_free(container_t *g)
{
	func_ptr_t free_f;

	if(g != NULL){
		free_f = type_get_func(g->type_name, "free");
		if(free_f) free_f(g->data_ptr);
		else free(g->data_ptr);
		free(g->type_name);
		free(g);
	}
}

