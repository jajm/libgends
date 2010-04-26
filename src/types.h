/*****************************************************************************
 * Fichier		: types.h                                            *
 * Description Brève	: Gestion de types 'utilisateur'                     *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Les types 'utilisateur' sont des nouveaux types définis par l'utilisateur.*
 * Ils sont décrits par un nom unique, et une taille en octets.              *
 * Ces types sont utilisés dans generic.h, pour faire référence au type d'une*
 * donnée générique.                                                         *
 *****************************************************************************/

#ifndef types_h_included
#define types_h_included

#include "basic_types.h"

typedef void (*free_func_t)(void *);
typedef s32 (*cmp_func_t)(void *, void *);

#ifdef __cplusplus
extern "C" {
#endif

/* À appeler avant d'utiliser les autres fonctions */
s8 types_init(u32 size);
/* Enregistrer un nouveau type */
s8 type_reg(const char *name, u32 size, free_func_t free, cmp_func_t cmp);
/* Supprimer un type existant */
s8 type_unreg(const char *name);
/* Obtenir la taille d'un type existant */
u32 type_sizeof(const char *name);
/* Récupère un pointeur vers la fonction de libération */
/* ou NULL si aucune fonction n'est enregistrée */
free_func_t type_free_func(const char *name);
/* Récupère un pointeur vers la fonction de comparaison */
/* ou NULL si aucune fonction n'est enregistrée */
cmp_func_t type_cmp_func(const char *name);
/* Détruire tous les types */
void types_free(void);

/* For debugging purposes */
void types_print(void);

#ifdef __cplusplus
}
#endif

#endif /* Not types_h_included */

