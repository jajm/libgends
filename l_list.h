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
	generic_t data;			/*!< La donnée, de type générique */
	struct l_list_node *next;	/*!< Noeud suivant */
};

err_code llist_add(struct l_list_node **head, s16 pos, generic_t *data);

#endif /* Not __l_list_h__ */
