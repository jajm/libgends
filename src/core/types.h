/*****************************************************************************
 * Fichier		: types.h                                            *
 * Description Brève	: Gestion de types 'utilisateur'                     *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Les types 'utilisateur' sont des nouveaux types définis par l'utilisateur.*
 * Ils sont décrits par un nom unique, et une taille en octets.              *
 *****************************************************************************/

#ifndef types_h_included
#define types_h_included

#include "basic_types.h"
#include "funcs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* À appeler avant d'utiliser les autres fonctions */
/* Retourne 0, ou une valeur négative s'il y a eu une erreur */
s8 types_init(u32 size);
/* Enregistrer un nouveau type */
/* Retourne une valeur négative s'il y a eu une erreur, */
/* 	    une valeur positive si le type existe déjà, */
/*	    0 sinon */
s8 type_reg(const char *name, u32 size);
/* Associer une fonction à un type */
s8 type_reg_func(const char *name, const char *func_name, func_ptr_t func_ptr);
/* Retourne un pointeur vers la fonction correspondante */
func_ptr_t type_get_func(const char *name, const char *func_name);
/* Supprimer un type existant */
s8 type_unreg(const char *name);
/* Supprimer une fonction associée à un type */
s8 type_unreg_func(const char *name, const char *func_name);
/* Obtenir la taille d'un type existant */
u32 type_sizeof(const char *name);

/* Détruire tous les types */
void types_free(void);

/* For debugging purposes */
void types_print(void);

#ifdef __cplusplus
}
#endif

#endif /* Not types_h_included */

