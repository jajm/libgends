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
typedef struct vw_slist_node_t{
	generic_ptr data;
	struct vw_slist_node_t *next;
} *vw_slist_node_ptr;

typedef struct vw_slist_t{
	vw_slist_node_ptr first;	/* Premier nœud */
	vw_slist_node_ptr last;		/* Dernier nœud */
	vw_slist_node_ptr curr;		/* Position courante */
} *vw_slist_ptr;

/* Ajoute un nœud après celui donné en paramètre */
/* Retourne l'adresse du nouveau nœud */
vw_slist_node_ptr vw_slist_node_add(
	vw_slist_node_ptr node,	/* Nœud à faire suivre du nouvel élément */
	generic_ptr data,	/* Donnée à stocker dans le nouvel élément */
	error_ptr *err
);

/* Supprime le nœud suivant celui donné en paramètre */
s8 vw_slist_node_del(
	vw_slist_node_ptr node,	/* Nœud précédant l'élément à supprimer */
	error_ptr *err
);


/* Crée une nouvelle liste et retourne l'adresse de cette liste */
vw_slist_ptr vw_slist_new(
	error_ptr *err
);

/* Réinitialise la position courante au début de la liste */
void vw_slist_begin(
	vw_slist_ptr l		/* Liste à modifier */
);

/* Déplace la position courante vers le nœud suivant */
/* Retourne -1 si le nœeud suivant n'existe pas (fin de liste), 0 sinon */
s8 vw_slist_next(
	vw_slist_ptr l
);

/* ========================================================================= */
/*                             Fonctions d'ajout                             */
/* ========================================================================= */

/* Ajoute un nœud à la liste en première position */
/* Retourne l'adresse du nouveau nœud */
vw_slist_node_ptr vw_slist_add_first(
	vw_slist_ptr l,		/* Liste à modifier */
	generic_ptr data,	/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
	error_ptr *err
);

/* Ajoute un nœud à la liste en dernière position */
/* Retourne l'adresse du nouveau nœud */
vw_slist_node_ptr vw_slist_add_last(
	vw_slist_ptr l,		/* Liste à modifier */
	generic_ptr data,	/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
	error_ptr *err
);

/* Ajoute un nœud à la liste après la position courante */
/* Retourne l'adresse du nouveau nœud */
vw_slist_node_ptr vw_slist_add(
	vw_slist_ptr l,		/* Liste à modifier */
	generic_ptr data,	/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire */
	error_ptr *err
);

/* ========================================================================= */
/*                          Fonctions de suppression                         */
/* ========================================================================= */

/* Supprime le premier nœud de la liste */
s8 vw_slist_del_first(
	vw_slist_ptr l,		/* Liste à modifier */
	error_ptr *err
);

/* Supprime le dernier nœud de la liste */
s8 vw_slist_del_last(
	vw_slist_ptr l,		/* Liste à modifier */
	error_ptr *err
);

/* Supprime le nœud de la liste à la position courante */
s8 vw_slist_del(
	vw_slist_ptr l,		/* Liste à modifier */
	error_ptr *err
);

/* ========================================================================= */
/*                  Fonctions de récupération des données                    */
/* ========================================================================= */

/* Récupère la donnée du premier nœud de la liste */
generic_ptr vw_slist_get_first(
	vw_slist_ptr l,
	error_ptr *err
);

/* Récupère la donnée du dernier nœud de la liste */
generic_ptr vw_slist_get_last(
	vw_slist_ptr l,
	error_ptr *err
);

/* Récupère la donnée du nœud de la liste à la position courante */
generic_ptr vw_slist_get(
	vw_slist_ptr l,
	error_ptr *err
);

/* ========================================================================= */
/*                             Fonctions 'POP'                               */
/*      (récupération des données et suppression du nœud dans la liste)      */
/* ========================================================================= */

/* Supprime le premier nœud de la liste */
/* Retourne la donnée qu'il contenait */
generic_ptr vw_slist_pop_first(
	vw_slist_ptr l,		/* Liste à modifier */
	error_ptr *err
);

/* Supprime le dernier nœud de la liste */
/* Retourne la donnée qu'il contenait */
generic_ptr vw_slist_pop_last(
	vw_slist_ptr l,		/* Liste à modifier */
	error_ptr *err
);

/* Supprime le nœud de la liste à la position courante */
/* Retourne la donnée qu'il contenait */
generic_ptr vw_slist_pop(
	vw_slist_ptr l,		/* Liste à modifier */
	error_ptr *err
);

/* ========================================================================= */
/*                             Autres fonctions                              */
/* ========================================================================= */

/* Vérification de la présence d'un élément dans une liste */
vw_slist_node_ptr vw_slist_chk(
	vw_slist_ptr l,
	generic_ptr data,
	error_ptr *err
);

/* Affiche une liste */
void vw_slist_print(
	vw_slist_ptr l
);

/* Libération de la mémoire */
void vw_slist_free(
	vw_slist_ptr *l
);

#endif /* Not vw_slist_h_included */

