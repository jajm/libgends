/*****************************************************************************
 * Fichier           : vw_sllist.h                                           *
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

#ifndef __vw_sllist_h__
#define __vw_sllist_h__

#include "types.h"
#include "generic.h"
#include "error.h"

/* Noeud d'une liste simplement chainée à largeur variable */
struct vw_sllist_node{
	generic_ptr data;
	struct vw_sllist_node *next;
};

typedef struct _vw_sllist{
	struct vw_sllist_node *first;	/* Premier nœud */
	struct vw_sllist_node *last;	/* Dernier nœud */
	struct vw_sllist_node *curr;	/* Position courante */
} *vw_sllist;


/* Crée une nouvelle liste et retourne l'adresse de cette liste */
vw_sllist vw_sllist_new(
	error_ptr *err
);

/* Réinitialise la position courante au début de la liste */
void vw_sllist_begin(
	vw_sllist l		/* Liste à modifier */
);

/* ========================================================================= */
/*                             Fonctions d'ajout                             */
/* ========================================================================= */

/* Ajoute un nœud à la liste en première position */
/* Retourne l'adresse du nouveau noœud */
struct vw_sllist_node* vw_sllist_add_first(
	vw_sllist l,		/* Liste à modifier */
	generic_ptr data,	/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
	error_ptr *err
);

/* Ajoute un nœud à la liste en dernière position */
/* Retourne l'adresse du nouveau noœud */
struct vw_sllist_node* vw_sllist_add_last(
	vw_sllist l,		/* Liste à modifier */
	generic_ptr data,	/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
	error_ptr *err
);

/* Ajoute un nœud à la liste après la position courante */
/* Retourne l'adresse du nouveau noœud */
struct vw_sllist_node* vw_sllist_add(
	vw_sllist l,		/* Liste à modifier */
	generic_ptr data,	/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire */
	error_ptr *err
);

/* ========================================================================= */
/*                          Fonctions de suppression                         */
/* ========================================================================= */

/* Supprime le premier nœud de la liste */
s8 vw_sllist_del_first(
	vw_sllist l,		/* Liste à modifier */
	error_ptr *err
);

/* Supprime le dernier nœud de la liste */
s8 vw_sllist_del_last(
	vw_sllist l,		/* Liste à modifier */
	error_ptr *err
);

/* Supprime le nœud de la liste à la position courante */
s8 vw_sllist_del(
	vw_sllist l,		/* Liste à modifier */
	error_ptr *err
);

/* ========================================================================= */
/*                             Fonctions 'POP'                               */
/*      (récupération des données et suppression du nœud dans la liste)      */
/* ========================================================================= */

/* Supprime le premier nœud de la liste */
/* Retourne la donnée qu'il contenait */
generic_ptr vw_sllist_pop_first(
	vw_sllist l,		/* Liste à modifier */
	error_ptr *err
);

/* Supprime le dernier nœud de la liste */
/* Retourne la donnée qu'il contenait */
generic_ptr vw_sllist_pop_last(
	vw_sllist l,		/* Liste à modifier */
	error_ptr *err
);

/* Supprime le nœud de la liste à la position courante */
/* Retourne la donnée qu'il contenait */
generic_ptr vw_sllist_pop(
	vw_sllist l,		/* Liste à modifier */
	error_ptr *err
);

/* ========================================================================= */
/*                  Fonctions de récupération des données                    */
/* ========================================================================= */

/* Récupère la donnée du premier nœud de la liste */
generic_ptr vw_sllist_get_first(
	vw_sllist l,
	error_ptr *err
);

/* Récupère la donnée du dernier nœud de la liste */
generic_ptr vw_sllist_get_last(
	vw_sllist l,
	error_ptr *err
);

/* Récupère la donnée du nœud de la liste à la position courante */
generic_ptr vw_sllist_get(
	vw_sllist l,
	error_ptr *err
);

/* ========================================================================= */
/*                             Autres fonctions                              */
/* ========================================================================= */

/* Vérification de la présence d'un élément dans une liste */
struct vw_sllist_node* vw_sllist_chk(
	vw_sllist l,
	generic_ptr data,
	error_ptr *err
);

/* Affiche une liste */
void vw_sllist_print(
	vw_sllist l
);

/* Libération de la mémoire */
void vw_sllist_free(
	vw_sllist *l
);

#endif /* Not __vw_sllist_h__ */

