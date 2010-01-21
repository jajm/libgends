#ifndef __generic_h__
#define __generic_h__

#include "types.h"
#include "err_code.h"

/*!
 * \file generic.h
 * \brief Gestion d'un type de données générique
 * \author Julian Maurice
 * \date 24 Novembre 2009
 */

/*!
 * \brief Type générique
 *
 * Type de données générique, ie de taille variable
 */
typedef struct _generic_t{
	u8 sz;		/*!< Taille de la donnée, en octets */
	void *d;	/*!< Pointeur vers la donnée */
} *generic_t;

/*!
 * \brief Crée une donnée générique à partir de ses arguments
 * \param size Taille de la donnée
 * \param data La donnée
 * \return Un pointeur vers la structure nouvellement créée
 */
generic_t generic(u8 size, void *data);
/*!
 * \brief Affecte une valeur à une donnée générique
 * \param g Donnée générique à modifier
 * \param size Taille de la valeur à affecter
 * \param data Valeur à affecter
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code generic_affect(generic_t *g, u8 size, void *data);
/*!
 * \brief Copie de donnée générique
 *
 * La fonction crée une copie parfaite de \a from dans \a to
 * \param to Destination de la copie
 * \param from Donnée générique à copie
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code generic_copy(generic_t *to, generic_t from);
/*!
 * \brief Comparaison de deux données génériques
 *
 * Une donnée générique g1 est strictement plus grande qu'une donnée générique
 * g2 si :
 * <ul>
 * <li>g1->sz = g2->sz ET *(g1->d) = *(g2->d)</li>
 * <li>g1->sz \f$ \neq \f$ g2->sz ET g1->d[0..m] > g2->d[0..m], m =
 * min{g1->sz,g2->sz}</li>
 * </ul>
 * \param g1 Première donnée générique
 * \param g2 Deuxième donnée générique
 * \retval 1 \a g1 = \a g2
 * \retval 0 \a g1 \f$ \neq \f$ \a g2
 */
u8 generic_cmp(generic_t g1, generic_t g2);
/*!
 * \brief Donne la taille d'une donnée générique
 * \param g Donnée générique dont la taille est voulue
 * \return La taille de la donnée générique, en octets
 */
u8 generic_size(const generic_t g);
/*!
 * \brief Renvoie un pointeur vers la valeur de la donnée générique
 * \param g Donnée générique dont la valeur est voulue
 * \return Un pointeur vers la valeur de la donnée générique
 */
void *generic_data(const generic_t g);
/*!
 * \brief Libère la mémoire
 *
 * Libère la mémoire
 * \param g Donnée générique à libérer
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code generic_free(generic_t *g);

#endif /* Not __generic_h__ */

