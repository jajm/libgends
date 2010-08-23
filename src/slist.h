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
 * Fichier           : slist.h                                               *
 * Description Brève : Gestion d'une liste simplement chainée générique      *
 * Auteur            : Julian Maurice                                        *
 * Créé le           : 01/03/2010                                            *
 *****************************************************************************
 * Une liste chainée générique peut contenir tout type de données, à partir  *
 * du moment où un type utilisateur correspondant existe.                    *
 * Voir types.h pour plus d'informations sur les types utilisateurs          *
 *****************************************************************************/

#ifndef slist_h_included
#define slist_h_included

#include "slist_node.h"
#include "types.h"
#include "iterator.h"

typedef struct{
	char *type_name;
	slist_node_t *first;	/* Premier nœud */
	slist_node_t *last;	/* Dernier nœud */
} slist_t;

#ifdef __cplusplus
extern "C" { 
#endif

/* Crée une nouvelle liste et retourne l'adresse de cette liste */
slist_t *slist_new(const char *type_name);

/* Teste si la liste est vide (0 = non vide, 1 = vide) */
s8 slist_empty(slist_t *l);

/* Retourne l'adresse du premier nœud */
slist_node_t *slist_first(slist_t *l);

/* Retourne l'adresse du nœud suivant */
slist_node_t *slist_next(slist_t *l, slist_node_t *node);

/* Retourne 1 si node == NULL, 0 sinon */
/* Utilisée par les itérateurs */
s8 slist_end(slist_t *l, slist_node_t *node);


/* ========================================================================= */
/*                             Fonctions d'ajout                             */
/* ========================================================================= */

/* Ajoute un nœud à la liste l, après le nœud node */
/* Si l->first==NULL, crée une nouvelle liste (node ignoré) */
/* Si node==NULL, ajoute le nœud en première position */
/* Échec si l==NULL ou data==NULL */
/* Retourne l'adresse du nouveau nœud, ou NULL si échec */
slist_node_t *slist_add(slist_t *l, slist_node_t *node, void *data);

/* Ajoute un nœud à la liste en première position */
/* Retourne l'adresse du nouveau nœud */
slist_node_t *slist_add_first(slist_t *l, void *data);

/* Ajoute un nœud à la liste en dernière position */
/* Retourne l'adresse du nouveau nœud */
slist_node_t *slist_add_last(slist_t *l, void *data);

/* Ajoute un nœud à la liste après l'itérateur */
slist_node_t *slist_it_add(iterator_t *it, void *data);


/* ========================================================================= */
/*                          Fonctions de suppression                         */
/* ========================================================================= */


/* --------- Supprime un nœud et renvoie la donnée qu'il contenait --------- */
/* Supprime le nœud node dans la liste l */
void *slist_pop(slist_t *l, slist_node_t *node);

/* Supprime le premier nœud de la liste */
void *slist_pop_first(slist_t *l);

/* Supprime le dernier nœud de la liste */
void *slist_pop_last(slist_t *l);

/* Supprime le nœud sur lequel pointe l'itérateur */
void *slist_it_pop(iterator_t *it);


/* ---------- Supprime un nœud ainsi que la donnée qu'il contient ---------- */
/* Supprime le nœud node dans la liste l */
s8 slist_del(slist_t *l, slist_node_t *node);

/* Supprime le premier nœud de la liste */
s8 slist_del_first(slist_t *l);

/* Supprime le dernier nœud de la liste */
s8 slist_del_last(slist_t *l);

/* Supprime le nœud sur lequel pointe l'itérateur */
s8 slist_it_del(iterator_t *it);

/* ========================================================================= */
/*                  Fonctions de récupération des données                    */
/* ========================================================================= */
/* Récupère la donnée du nœud passé en paramètre */
void *slist_get(slist_t *l, slist_node_t *node);

/* Récupère la donnée du premier nœud de la liste */
void *slist_get_first(slist_t *l);

/* Récupère la donnée du dernier nœud de la liste */
void *slist_get_last(slist_t *l);

/* Récupère la donnée du nœud sur lequel pointe l'itérateur */
void *slist_it_get(iterator_t *it);

/* ========================================================================= */
/*                                Itérateurs                                 */
/* ========================================================================= */

/* Crée un itérateur */
iterator_t *slist_iterator_new(slist_t *l);


/* ========================================================================= */
/*                             Autres fonctions                              */
/* ========================================================================= */

/* Vérification de la présence d'un élément dans une liste */
slist_node_t *slist_chk(slist_t *l, void *data);

/* Libération de la mémoire */
void slist_free(slist_t *l);

/* Détruit la liste sans libérer la mémoire des données contenues dedans */
void slist_destroy(slist_t *l);

#ifdef __cplusplus
}
#endif

#endif /* Not slist_h_included */

