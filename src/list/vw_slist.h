/*****************************************************************************
 * Fichier           : vw_slist.h                                           *
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

#ifndef vw_slist_h_included
#define vw_slist_h_included

#include "types.h"
#include "generic.h"
#include "error.h"

/* Noeud d'une liste simplement chainée à largeur variable */
typedef struct vw_slist_node_s{
	generic_t *data;
	struct vw_slist_node_s *next;
} vw_slist_node_t;

typedef struct{
	vw_slist_node_t *first;	/* Premier nœud */
	vw_slist_node_t *last;		/* Dernier nœud */
	vw_slist_node_t *curr;		/* Position courante */
} vw_slist_t;

/* Ajoute un nœud après celui donné en paramètre */
/* Retourne l'adresse du nouveau nœud */
vw_slist_node_t *vw_slist_node_add(
	vw_slist_node_t *node,	/* Nœud à faire suivre du nouvel élément */
	generic_t *data	/* Donnée à stocker dans le nouvel élément */
);

/* Supprime le nœud suivant celui donné en paramètre */
s8 vw_slist_node_del(
	vw_slist_node_t *node	/* Nœud précédant l'élément à supprimer */
);

/* Supprime le nœud suivant celui donné en paramètre */
/* et libère la mémoire occupée par data */
s8 vw_slist_node_free(
	vw_slist_node_t *node
);


/* Crée une nouvelle liste et retourne l'adresse de cette liste */
vw_slist_t *vw_slist_new(void);

/* Réinitialise la position courante au début de la liste */
void vw_slist_begin(
	vw_slist_t *l		/* Liste à modifier */
);

/* Déplace la position courante vers le nœud suivant */
/* Retourne -1 si le nœeud suivant n'existe pas (fin de liste), 0 sinon */
s8 vw_slist_next(
	vw_slist_t *l
);

/* Teste si la liste est vide (0 = non vide, 1 = vide) */
s8 vw_slist_empty(
	vw_slist_t *l
);

/* Teste si la position courante est à la fin de la liste */
/* (après le dernier élément) */
/* Retourne 1 si c'est la fin, 0 sinon */
s8 vw_slist_end(
	vw_slist_t *l
);

/* ========================================================================= */
/*                             Fonctions d'ajout                             */
/* ========================================================================= */

/* Ajoute un nœud à la liste en première position */
/* Retourne l'adresse du nouveau nœud */
vw_slist_node_t *vw_slist_add_first(
	vw_slist_t *l,		/* Liste à modifier */
	generic_t *data		/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
);

/* Ajoute un nœud à la liste en dernière position */
/* Retourne l'adresse du nouveau nœud */
vw_slist_node_t *vw_slist_add_last(
	vw_slist_t *l,		/* Liste à modifier */
	generic_t *data		/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
);

/* Ajoute un nœud à la liste après la position courante */
/* Retourne l'adresse du nouveau nœud */
vw_slist_node_t *vw_slist_add(
	vw_slist_t *l,		/* Liste à modifier */
	generic_t *data		/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire */
);

/* ========================================================================= */
/*                          Fonctions de suppression                         */
/* ========================================================================= */

/* -------- Supprime un nœud sans supprimer la donnée qu'il contient ------- */
/* Supprime le premier nœud de la liste */
s8 vw_slist_del_first(
	vw_slist_t *l		/* Liste à modifier */
);

/* Supprime le dernier nœud de la liste */
s8 vw_slist_del_last(
	vw_slist_t *l		/* Liste à modifier */
);

/* Supprime le nœud de la liste à la position courante */
s8 vw_slist_del(
	vw_slist_t *l		/* Liste à modifier */
);

/* ---------- Supprime un nœud ainsi que la donnée qu'il contient ---------- */
/* Supprime le premier nœud de la liste */
s8 vw_slist_free_first(
	vw_slist_t *l		/* Liste à modifier */
);

/* Supprime le dernier nœud de la liste */
s8 vw_slist_free_last(
	vw_slist_t *l		/* Liste à modifier */
);

/* Supprime le nœud de la liste à la position courante */
s8 vw_slist_free(
	vw_slist_t *l		/* Liste à modifier */
);


/* ========================================================================= */
/*                  Fonctions de récupération des données                    */
/* ========================================================================= */

/* Récupère la donnée du premier nœud de la liste */
generic_t *vw_slist_get_first(
	vw_slist_t *l
);

/* Récupère la donnée du dernier nœud de la liste */
generic_t *vw_slist_get_last(
	vw_slist_t *l
);

/* Récupère la donnée du nœud de la liste à la position courante */
generic_t *vw_slist_get(
	vw_slist_t *l
);

/* ========================================================================= */
/*                             Fonctions 'POP'                               */
/*      (récupération des données et suppression du nœud dans la liste)      */
/* ========================================================================= */

/* Supprime le premier nœud de la liste */
/* Retourne la donnée qu'il contenait */
generic_t *vw_slist_pop_first(
	vw_slist_t *l		/* Liste à modifier */
);

/* Supprime le dernier nœud de la liste */
/* Retourne la donnée qu'il contenait */
generic_t *vw_slist_pop_last(
	vw_slist_t *l		/* Liste à modifier */
);

/* Supprime le nœud de la liste à la position courante */
/* Retourne la donnée qu'il contenait */
generic_t *vw_slist_pop(
	vw_slist_t *l		/* Liste à modifier */
);

/* ========================================================================= */
/*                             Autres fonctions                              */
/* ========================================================================= */

/* Vérification de la présence d'un élément dans une liste */
vw_slist_node_t *vw_slist_chk(
	vw_slist_t *l,
	generic_t *data
);

/* Affiche une liste */
void vw_slist_print(
	vw_slist_t *l
);

/* Libération de la mémoire */
void vw_slist_free_list(
	vw_slist_t *l
);

#endif /* Not vw_slist_h_included */

