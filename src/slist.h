/*****************************************************************************
 * Fichier           : slist.h                                               *
 * Description Brève : Gestion d'une liste simplement chainée à largeur      *
 *                     variable                                              *
 * Auteur            : Julian Maurice                                        *
 * Créé le           : 01/03/2010                                            *
 *****************************************************************************
 * Une liste chainée à largeur variable est une liste chainée dont les       *
 * nœuds contiennent des données de différentes tailles.                     *
 * C'est utile pour stocker dans une même structure des données de types     *
 * différents.                                                               *
 * Voir les fichiers "generic.h" et "generic.c" pour savoir comment ceci est *
 * implémenté, ainsi que les fichiers "types.h" et "types.c".                *
 *****************************************************************************/

#ifndef slist_h_included
#define slist_h_included

#include "basic_types.h"
#include "types.h"
#include "iterator.h"

/* Noeud d'une liste simplement chainée à largeur variable */
typedef struct slist_node_s{
	void *data;
	struct slist_node_s *next;
} slist_node_t;

/* Ajoute un nœud contenant data après node
 * Si node == NULL, crée un nouveau nœud seul
 * Retourne l'adresse du nouveau nœud */
slist_node_t *slist_node_add_after(slist_node_t *node, void *data);
/* Retourne un pointeur vers la donnée de node */
void *slist_node_get(slist_node_t *node);
/* Supprime le nœud node dans la liste head et fait pointer data vers la donnée
 * Retourne l'adresse du nœud précédant node (NULL pour le premier nœud) */
slist_node_t *slist_node_pop(slist_node_t **head, slist_node_t *node,
	void **data);
/* Supprime le nœud node dans la liste head et libère la mémoire occupée par
 * la donnée à l'aide de free_f. Si free_f == NULL, utilise free()
 * Retourne l'adresse du nœud précédant node (NULL pour le premier nœud) */
slist_node_t *slist_node_del(slist_node_t **head, slist_node_t *node,
	func_ptr_t free_f);


typedef struct{
	char *type_name;
	slist_node_t *first;	/* Premier nœud */
	slist_node_t *last;	/* Dernier nœud */
} slist_t;


/* Crée une nouvelle liste et retourne l'adresse de cette liste */
slist_t *slist_new(const char *type_name);

/* Teste si la liste est vide (0 = non vide, 1 = vide) */
s8 slist_empty(slist_t *l);

/* Retourne l'adresse du premier nœud */
slist_node_t *slist_first(slist_t *l);

/* Retourne l'adresse du nœud suivant */
slist_node_t *slist_next(slist_t *l, slist_node_t *node);

/* Retourne 1 si node == NULL, 0 sinon */
s8 slist_end(slist_t *l, slist_node_t *node);


/* ========================================================================= */
/*                             Fonctions d'ajout                             */
/* ========================================================================= */

/* Ajoute un nœud à la liste en première position */
/* Retourne l'adresse du nouveau nœud */
slist_node_t *slist_add_first(slist_t *l, void *data);

/* Ajoute un nœud à la liste en dernière position */
/* Retourne l'adresse du nouveau nœud */
slist_node_t *slist_add_last(slist_t *l, void *data);

/* Ajoute un nœud à la liste après l'itérateur */
slist_node_t *slist_add(iterator_t *it, void *data);


/* ========================================================================= */
/*                          Fonctions de suppression                         */
/* ========================================================================= */

/* --------- Supprime un nœud et renvoie la donnée qu'il contient ---------- */
/* Supprime le premier nœud de la liste */
void *slist_pop_first(slist_t *l);

/* Supprime le dernier nœud de la liste */
void *slist_pop_last(slist_t *l);

/* Supprime le nœud sur lequel pointe l'itérateur */
void *slist_pop(iterator_t *it);


/* ---------- Supprime un nœud ainsi que la donnée qu'il contient ---------- */
/* Supprime le premier nœud de la liste */
s8 slist_del_first(slist_t *l);

/* Supprime le dernier nœud de la liste */
s8 slist_del_last(slist_t *l);

/* Supprime le nœud sur lequel pointe l'itérateur */
s8 slist_del(iterator_t *it);

/* ========================================================================= */
/*                  Fonctions de récupération des données                    */
/* ========================================================================= */

/* Récupère la donnée du premier nœud de la liste */
void *slist_get_first(slist_t *l);

/* Récupère la donnée du dernier nœud de la liste */
void *slist_get_last(slist_t *l);

/* Récupère la donnée du nœud passé en paramètre */
void *slist_get(slist_t *l, slist_node_t *node);


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

#endif /* Not slist_h_included */

