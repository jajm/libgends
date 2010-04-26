/*****************************************************************************
 * Fichier           : slist.h                                           *
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

/* Noeud d'une liste simplement chainée à largeur variable */
typedef struct slist_node_s{
	void *data;
	struct slist_node_s *next;
} slist_node_t;

typedef struct{
	char *type_name;
	slist_node_t *first;	/* Premier nœud */
	slist_node_t *last;		/* Dernier nœud */
	slist_node_t *curr;		/* Position courante */
} slist_t;

/* Crée un nouveau nœud et retourne son adresse */
slist_node_t *slist_node_new(
	void *data	/* Donnée à stocker dans le nouvel élément */
);

/* Libère la mémoire occupée par un nœud */
/* ATTENTION: Cette fonction ne met pas à jour le pointeur du nœud précédent */
void slist_node_free(
	slist_node_t *node,
	free_func_t free_f
);

/* Crée une nouvelle liste et retourne l'adresse de cette liste */
slist_t *slist_new(const char *type_name);

/* Réinitialise la position courante au début de la liste */
void slist_begin(
	slist_t *l		/* Liste à modifier */
);

/* Déplace la position courante vers le nœud suivant */
/* Retourne -1 si le nœeud suivant n'existe pas (fin de liste), 0 sinon */
s8 slist_next(
	slist_t *l
);

/* Teste si la liste est vide (0 = non vide, 1 = vide) */
s8 slist_empty(
	slist_t *l
);

/* Teste si la position courante est à la fin de la liste */
/* (après le dernier élément) */
/* Retourne 1 si c'est la fin, 0 sinon */
s8 slist_end(
	slist_t *l
);

/* ========================================================================= */
/*                             Fonctions d'ajout                             */
/* ========================================================================= */

/* Ajoute un nœud à la liste en première position */
/* Retourne l'adresse du nouveau nœud */
slist_node_t *slist_add_first(
	slist_t *l,		/* Liste à modifier */
	void *data		/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
);

/* Ajoute un nœud à la liste en dernière position */
/* Retourne l'adresse du nouveau nœud */
slist_node_t *slist_add_last(
	slist_t *l,		/* Liste à modifier */
	void *data		/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
);

/* Ajoute un nœud à la liste après la position courante */
/* Retourne l'adresse du nouveau nœud */
slist_node_t *slist_add(
	slist_t *l,		/* Liste à modifier */
	void *data		/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire */
);

/* ========================================================================= */
/*                          Fonctions de suppression                         */
/* ========================================================================= */

/* --------- Supprime un nœud et renvoie la donnée qu'il contient ---------- */
/* Supprime le premier nœud de la liste */
void *slist_pop_first(
	slist_t *l		/* Liste à modifier */
);

/* Supprime le dernier nœud de la liste */
void *slist_pop_last(
	slist_t *l		/* Liste à modifier */
);

/* Supprime le nœud de la liste à la position courante */
void *slist_pop(
	slist_t *l		/* Liste à modifier */
);

/* ---------- Supprime un nœud ainsi que la donnée qu'il contient ---------- */
/* Supprime le premier nœud de la liste */
s8 slist_free_first(
	slist_t *l		/* Liste à modifier */
);

/* Supprime le dernier nœud de la liste */
s8 slist_free_last(
	slist_t *l		/* Liste à modifier */
);

/* Supprime le nœud de la liste à la position courante */
s8 slist_free(
	slist_t *l		/* Liste à modifier */
);


/* ========================================================================= */
/*                  Fonctions de récupération des données                    */
/* ========================================================================= */

/* Récupère la donnée du premier nœud de la liste */
void *slist_get_first(
	slist_t *l
);

/* Récupère la donnée du dernier nœud de la liste */
void *slist_get_last(
	slist_t *l
);

/* Récupère la donnée du nœud de la liste à la position courante */
void *slist_get(
	slist_t *l
);


/* ========================================================================= */
/*                             Autres fonctions                              */
/* ========================================================================= */

/* Vérification de la présence d'un élément dans une liste */
slist_node_t *slist_chk(
	slist_t *l,
	void *data
);

/* Affiche une liste */
void slist_print(
	slist_t *l
);

/* Libération de la mémoire */
void slist_free_list(
	slist_t *l
);

#endif /* Not slist_h_included */

