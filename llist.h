#ifndef __llist_h__
#define __llist_h__

#include "types.h"
#include "err_code.h"
#include "generic.h"

/*!
 * \file l_list.h
 * \brief Définition de listes simplement chainées
 * \author Julian Maurice
 * \date 24 Novembre 2009
 */

typedef s16 llist_pos_t;

/*! 
 * \brief Noeud d'une liste simplement chainée à largeur variable
 *
 * Une liste simplement chainée à largeur variable est une liste simplement
 * chainée classique, mais dont la taille des éléments n'est pas fixée.
 * On peut utiliser une telle liste, par exemple, pour stocker des objets
 * (structures) différents dans une même liste.
 * Toutes les fonctions en rapport avec ce type de liste chainée ont le nom
 * commençant par 'vw_llist' (pour variable-width linked-list)
 */
struct vw_llist_node{
	generic_t d;			/*!< La donnée, de type générique */
	struct vw_llist_node *next;	/*!< Noeud suivant */
};

typedef struct vw_llist_node *vw_llist;

/*!
 * \brief Ajout d'un élément dans une liste
 * \param l Adresse d'une liste
 * \param pos Position où insérer le nouvel élément. \a pos peut prendre les
 * valeurs suivantes :
 * - -1: Ajout en fin de liste
 * -  0: Ajout en tête de liste
 * -  x: Ajout devant le x-ième élément ( x-1 -> new_elt -> x )
 * \param data Donnée générique à affecter au nouvel élément
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code vw_llist_add(vw_llist *l, llist_pos_t pos, generic_t data);

/*!
 * \brief Suppression d'un élément d'une liste
 * \param l Adresse d'une liste
 * \param pos Position de l'élément à supprimer. \a pos peut prendre les valeurs
 * suivantes :
 * <ul>
 * <li>-1: Suppression du dernier élément</li>
 * <li>0: Suppression du premier élément</li>
 * <li>x: Suppression du x-ième élément, si x > taille(liste), suppression du
 * dernier élément</li>
 * </ul>
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code vw_llist_del(vw_llist *l, llist_pos_t pos);

/*!
 * \brief Récupération de la donnée d'un élément d'une liste
 * \param l Une liste
 * \param pos Position de l'élément à récupérer
 * \return La donnée de l'élément choisi
 */
generic_t vw_llist_get(vw_llist l, llist_pos_t pos);

/*!
 * \brief Vérification de la présence d'un élément dans une liste
 *
 * Pour vérifier la présence d'un élément, llist_chk regarde le champ data, et
 * le compare à l'argument \a data.
 * \sa generic_cmp
 * \param l Une liste
 * \param data Donnée comparative
 * \return La position du premier élément rencontré qui correspond à la
 * recherche effectuée
 * \return -1 si l'élement recherché n'est pas dans la liste
 */
llist_pos_t vw_llist_chk(vw_llist l, generic_t data);

/*!
 * \brief Affiche une liste
 *
 * Affiche la valeur des pointeurs dans l'ordre de la liste.
 * Fonction test.
 * \param l Une liste
 */
void vw_llist_print(vw_llist l);

/*!
 * \brief Libération de la mémoire
 * \param l Adresse d'une liste
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code vw_llist_free(vw_llist *l);


/************************************************
 * 		Fixed-Width Linked List		*
 * 		TODO : Comment IT		*
 ************************************************/
struct llist_node{
	void *d;
	struct llist_node *next;
};

typedef struct _llist{
	struct llist_node *head;
	u32 width;
} *llist;

llist llist_new(u32 width);
err_code llist_add(llist l, llist_pos_t pos, void *data);
err_code llist_del(llist l, llist_pos_t pos);
void *llist_get(llist l, llist_pos_t pos);
llist_pos_t llist_chk(llist l, void *data);
void llist_print(llist l);
err_code llist_free(llist *l);

#endif /* Not __llist_h__ */
