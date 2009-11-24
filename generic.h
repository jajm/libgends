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
typedef struct{
	u8 sz;		/*!< Taille de la donnée, en octets */
	void *d;	/*!< Pointeur vers la donnée */
} generic_t;

/*!
 * \brief Initialise une donnée générique
 * \warning \a g doit avoir une zone mémoire allouée, sous peine de crash
 * immédiat ou, au mieux, de méchants bugs
 * \warning Toute donnée générique doit être initialisée avant d'être utilisée
 * \param g Donnée générique à initialiser
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code generic_init(generic_t *g);
/*!
 * \brief Affecte une valeur à une donnée générique
 *
 * La mémoire est allouée (ou désallouée) au besoin, mais l'appel préalable à
 * generic_init() est obligatoire
 * \warning \a g doit avoir une zone mémoire allouée, sous peine de crash
 * immédiat ou, au mieux, de méchants bugs
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
 * \warning \a to doit avoir une zone mémoire allouée, sous peine de crash
 * immédiat ou, au mieux, de méchants bugs
 * \param to Destination de la copie
 * \param from Donnée générique à copie
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code generic_copy(generic_t *to, const generic_t *from);
/*!
 * \brief Donne la taille d'une donnée générique
 * \param g Donnée générique dont la taille est voulue
 * \return La taille de la donnée générique, en octets
 */
u8 generic_size(const generic_t *g);
/*!
 * \brief Renvoie un pointeur vers la valeur de la donnée générique
 * \param g Donnée générique dont la valeur est voulue
 * \return Un pointeur vers la valeur de la donnée générique
 */
void *generic_data(const generic_t *g);
/*!
 * \brief Réinitialise la donnée générique à zéro
 *
 * Libère la mémoire et réinitialise la taille à zéro
 * \warning \a g doit avoir une zone mémoire allouée, sous peine de crash
 * immédiat ou, au mieux, de méchants bugs
 * \param g Donnée générique à réinitialiser
 * \return Un code d'erreur comme défini dans err_code.h
 */
err_code generic_delete(generic_t *g);

#endif /* Not __generic_h__ */

