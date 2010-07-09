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
 * Fichier		: container.h                                        *
 * Brève Description	: Conteneur de donnée générique                      *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Un conteneur peut contenir une et une seule donnée générique, représentée *
 * par un pointeur vers la donnée, et le nom du type de la donnée            *
 *****************************************************************************/

#ifndef container_h_included
#define container_h_included

#include "basic_types.h"

typedef struct{
	char *type_name;	/* Type de la donnée */
	void *data_ptr;		/* Pointeur vers la donnée */
} container_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Crée une donnée générique à partir des arguments */
container_t *container(const char *type_name, const void *data_ptr);

/* Affecte une valeur à une donnée générique */
s8 container_affect(container_t **g, const char *type_name, const void *data_ptr);

/* Copie de donnée générique */
s8 container_copy(container_t **to, const container_t *from);

/* Comparaison de deux données génériques */
s32 container_cmp(const container_t *g1, const container_t *g2);

/* Donne la taille d'une donnée générique */
u32 container_size(const container_t *g);

/* Libère la mémoire */
void container_free(container_t *g);

#ifdef __cplusplus
}
#endif

#endif /* Not container_h_included */

