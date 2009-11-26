#ifndef __l_list_h__
#define __l_list_h__

#include "types.h"
#include "err_code.h"
#include "generic.h"

/*!
 * \file l_list.h
 * \brief Définition de listes simplement chainées
 * \author Julian Maurice
 * \date 24 Novembre 2009
 */

/*! 
 * \brief Noeud d'une liste simplement chainée
 */
struct l_list_node{
	generic_t d;			/*!< La donnée, de type générique */
	struct l_list_node *next;	/*!< Noeud suivant */
};

/*!
 * \brief Ajout d'un élément dans une liste
 * \param head Adresse du pointeur de tête d'une liste
 * \param pos Position où insérer le nouvel élément. \a pos peut prendre les
 * valeurs suivantes :
 * - -1: Ajout en fin de liste
 * -  0: Ajout en tête de liste
 * -  x: Ajout devant le x-ième élément ( x-1 -> new_elt -> x )
 * \param data Donnée générique à affecter au nouvel élément
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code llist_add(struct l_list_node **head, s16 pos, generic_t data);
/*!
 * \brief Suppression d'un élément d'une liste
 * \param head Adresse du pointeur de tête d'une liste
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
err_code llist_del(struct l_list_node **head, s16 pos);
/*!
 * \brief Récupération de la donnée d'un élément d'une liste
 * \param head Pointeur de tête d'une liste
 * \param pos Position de l'élément à récupérer
 * \return La donnée de l'élément choisi
 */
generic_t llist_get(struct l_list_node *head, s16 pos);
/*!
 * \brief Vérification de la présence d'un élément dans une liste
 *
 * Pour vérifier la présence d'un élément, llist_chk regarde le champ data, et
 * le compare à l'argument \a data.
 * \sa generic_cmp
 * \param head Pointeur de tête d'une liste
 * \param data Donnée comparative
 * \return La position du premier élément rencontré qui correspond à la
 * recherche effectuée
 * \return -1 si l'élement recherché n'est pas dans la liste
 */
s16 llist_chk(struct l_list_node *head, generic_t data);
/*!
 * \brief Affiche une liste
 *
 * Affiche la valeur des pointeurs dans l'ordre de la liste.
 * Fonction test.
 * \param head Pointeur de tête de la liste
 */
void llist_print(struct l_list_node *head);
/*!
 * \brief Libération de la mémoire
 * \param head Adresse du pointeur de tête d'une liste
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code llist_free(struct l_list_node **head);

#endif /* Not __l_list_h__ */
