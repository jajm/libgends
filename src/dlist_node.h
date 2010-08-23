
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
 * Fichier		: dlist_node.h                                       *
 * Description Brève	: Nœud d'une liste doublement chainée                *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 23/08/2010                                         *
 *****************************************************************************/

#ifndef dlist_node_h_included
#define dlist_node_h_included

#include "basic_types.h"

typedef struct dlist_node_s {
	void *data;
	struct dlist_node_s *prev;
	struct dlist_node_s *next;
} dlist_node_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Crée un nouveau nœud */
dlist_node_t *dlnode_new(void *data);
/* Affecte une donnée à un nœud */
void dlnode_set_data(dlist_node_t *node, void *data);
/* Retourne la donnée associée au nœud */
void *dlnode_data(dlist_node_t *node);
/* Détruit un nœud */
void dlnode_free(dlist_node_t *node);

#ifdef __cplusplus
}
#endif

#endif /* Not dlist_node_h_included */
