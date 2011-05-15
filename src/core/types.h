/*****************************************************************************
 * Copyright (C) 2010 Julian Maurice                                         *
 *                                                                           *
 * This file is part of libgends.                                            *
 *                                                                           *
 * libgends is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * libgends is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.         *
 *****************************************************************************/

/*****************************************************************************
 * Fichier		: types.h                                            *
 * Description Brève	: Gestion de types 'utilisateur'                     *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Les types 'utilisateur' sont des nouveaux types définis par l'utilisateur.*
 * Ils sont décrits par un nom unique, une taille en octets et possèdent une *
 * liste de fonctions identifiées par leur nom.                              *
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
/* Retourne 0, ou une valeur négative s'il y a eu une erreur */
s8 type_reg_func(const char *name, const char *func_name, func_ptr_t func_ptr);
/* Retourne un pointeur vers la fonction correspondante, ou NULL si la fonction n'existe pas */
func_ptr_t type_get_func(const char *name, const char *func_name);
/* Supprimer un type existant */
/* Retourne 0, ou une valeur négative s'il y a eu une erreur */
s8 type_unreg(const char *name);
/* Supprimer une fonction associée à un type */
/* Retourne 0, ou une valeur négative s'il y a eu une erreur */
s8 type_unreg_func(const char *name, const char *func_name);
/* Retourne la taille d'un type existant, ou 0 si inexistant */
u32 type_sizeof(const char *name);

/* Détruire tous les types */
void types_free(void);

/* For debugging purposes */
void types_print(void);

#ifdef __cplusplus
}
#endif

#endif /* Not types_h_included */

