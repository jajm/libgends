#ifndef __err_code_h__
#define __err_code_h__

/*!
 * \file err_code.h
 * \brief Définition des valeurs de retours des fonctions
 * \author Julian Maurice
 * \date 24 Novembre 2009
 */

/*!
 * \brief Code d'erreur
 *
 * Permet d'identifier facilement une erreur en sortie de fonction
 */
typedef enum{
	OK = 0,			/*!< Pas d'erreurs, tout va bien */
	MEM_ALLOC_ERROR = -1,	/*!< Erreur d'allocation mémoire */
	PARAM_VALUE_ERROR = -2,	/*!< Mauvaise valeur pour un paramètre */
	/* ... */
	ANY_ERROR = -128	/*!< N'importe quelle autre erreur */
} err_code;

#endif /* Not __err_code_h__ */

