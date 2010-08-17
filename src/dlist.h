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
 * Fichier		: dlist.h                                            *
 * Description Brève	: Gestion d'une liste doublement chainée générique   *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/06/2010                                         *
 *****************************************************************************
 * Une liste chainée générique peut contenir tout type de données, à partir  *
 * du moment où un type utilisateur correspondant existe.                    *
 * Voir types.h pour plus d'informations sur les types utilisateurs          *
 *****************************************************************************/
 
#ifndef dlist_h_included
#define dlist_h_included

#include "basic_types.h"
#include "types.h"
#include "iterator.h"

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

typedef struct {
	char *type_name;
	dlist_node_t *first;
	dlist_node_t *last;
} dlist_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Crée une nouvelle liste et retourne l'adresse de cette liste */
dlist_t *dlist_new(const char *type_name);

/* Teste si la liste est vide (0 = non vide, 1 = vide) */
s8 dlist_empty(dlist_t *l);

/* Retourne l'adresse du premier nœud */
dlist_node_t *dlist_first(dlist_t *l);
/* Retourne l'adresse du dernier nœud */
dlist_node_t *dlist_last(dlist_t *l);

/* Retourne l'adresse du nœud suivant */
dlist_node_t *dlist_next(dlist_t *l, dlist_node_t *node);
/* Retourne l'adresse du nœud précédent */
dlist_node_t *dlist_prev(dlist_t *l, dlist_node_t *node);

/* Retourne 1 si node == NULL, 0 sinon */
/* Utilisée par les itérateurs */
s8 dlist_end(dlist_t *l, dlist_node_t *node);


/* ========================================================================= */
/*                             Fonctions d'ajout                             */
/* ========================================================================= */

dlist_node_t *dlist_add_after(dlist_t *l, dlist_node_t *node, void *data);
dlist_node_t *dlist_add_before(dlist_t *l, dlist_node_t *node, void *data);

/* Ajoute un nœud à la liste en première position */
/* Retourne l'adresse du nouveau nœud */
dlist_node_t *dlist_add_first(dlist_t *l, void *data);

/* Ajoute un nœud à la liste en dernière position */
/* Retourne l'adresse du nouveau nœud */
dlist_node_t *dlist_add_last(dlist_t *l, void *data);

/* Ajoute un nœud à la liste après l'itérateur */
dlist_node_t *dlist_it_add_after(iterator_t *it, void *data);
/* Ajoute un nœud à la liste avant l'itérateur */
dlist_node_t *dlist_it_add_before(iterator_t *it, void *data);


/* ========================================================================= */
/*                          Fonctions de suppression                         */
/* ========================================================================= */

/* --------- Supprime un nœud et renvoie la donnée qu'il contient ---------- */
void *dlist_pop(dlist_t *l, dlist_node_t *node);

/* Supprime le premier nœud de la liste */
void *dlist_pop_first(dlist_t *l);

/* Supprime le dernier nœud de la liste */
void *dlist_pop_last(dlist_t *l);

/* Supprime le nœud sur lequel pointe l'itérateur */
void *dlist_it_pop(iterator_t *it);


/* ---------- Supprime un nœud ainsi que la donnée qu'il contient ---------- */
s8 dlist_del(dlist_t *l, dlist_node_t *node);

/* Supprime le premier nœud de la liste */
s8 dlist_del_first(dlist_t *l);

/* Supprime le dernier nœud de la liste */
s8 dlist_del_last(dlist_t *l);

/* Supprimer le nœud sur lequel pointe l'itérateur */
s8 dlist_it_del(iterator_t *it);


/* ========================================================================= */
/*                  Fonctions de récupération des données                    */
/* ========================================================================= */

/* Récupère la donnée du nœud passé en paramètre */
void *dlist_get(dlist_t *l, dlist_node_t *node);

/* Récupère la donnée du premier nœud de la liste */
void *dlist_get_first(dlist_t *l);

/* Récupère la donnée du dernier nœud de la liste */
void *dlist_get_last(dlist_t *l);

void *dlist_it_get(iterator_t *it);

/* ========================================================================= */
/*                               Itérateurs                                  */
/* ========================================================================= */

/* Crée un itérateur qui va du début à la fin de la liste (ordre normal) */
iterator_t *dlist_iterator_new(dlist_t *l);
/* Crée un itérateur qui va de la fin au début de la liste (ordre inverse) */
iterator_t *dlist_reverse_iterator_new(dlist_t *l);


/* ========================================================================= */
/*                             Autres fonctions                              */
/* ========================================================================= */

/* Vérification de la présence d'un élément dans une liste */
dlist_node_t *dlist_chk(dlist_t *l, void *data);

/* Libération de la mémoire */
void dlist_free(dlist_t *l);

void dlist_destroy(dlist_t *l);

#ifdef __cplusplus
}
#endif

#endif /* Not dlist_h_included */

