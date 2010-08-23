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
 * Fichier           : slist_node.h                                          *
 * Description Brève : Nœud d'une liste simplement chainée                   *
 * Auteur            : Julian Maurice                                        *
 * Créé le           : 23/08/2010                                            *
 *****************************************************************************/

#ifndef slist_node_h_included
#define slist_node_h_included

#include "basic_types.h"

/* Noeud d'une liste simplement chainée à largeur variable */
typedef struct slist_node_s{
	void *data;
	struct slist_node_s *next;
} slist_node_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Crée un nouveau nœud */
slist_node_t *slnode_new(void *data);
/* Affecte une donnée à un nœud */
void slnode_set_data(slist_node_t *node, void *data);
/* Retourne la donnée associée au nœud */
void *slnode_data(slist_node_t *node);
/* Détruit un nœud */
void slnode_free(slist_node_t *node);

#ifdef __cplusplus
}
#endif

#endif /* Not slist_node_h_included */

