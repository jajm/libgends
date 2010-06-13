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
 * Fichier		: funcs.h                                            *
 * Description Brève	: Liste de fonctions génériques                      *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 13/03/2010					     *
 *****************************************************************************
 * Liste de fonctions génériques (en réalité de pointeurs de fonctions de    *
 * type intptr_t (*ptr)(void *, ...) ).                                      *
 *****************************************************************************/

#ifndef funcs_h_included
#define funcs_h_included

#include <stdint.h>
#include "basic_types.h"

typedef intptr_t (*func_ptr_t)(void *, ...);

typedef struct {
	char *name;	/* Nom de la fonction */
	func_ptr_t ptr;	/* Pointeur de fonction */
} func_t;

typedef struct func_list_node_s {
	func_t *func;
	struct func_list_node_s *next;
} func_list_node_t, *funcs_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Crée une nouvelle fonction et retourne son adresse */
func_t *func_new(const char *func_name, func_ptr_t func_ptr);
/* Retourne le nom de la fonction */
char *func_get_name(func_t *func);
/* Retourne le pointeur de la fonction */
func_ptr_t func_get_ptr(func_t *func);
/* Libère la mémoire */
void func_free(func_t *func);

/* Ajoute une fonction à la liste et retourne l'adresse du nœud créé */
func_list_node_t *funcs_add(funcs_t *funcs, func_t *func);
/* Supprime une fonction dans la liste */
void funcs_del(funcs_t *head, const char *name);
/* Libère la mémoire */
void funcs_free(funcs_t head);
/* Retourne l'adresse de la fonction (structure) */
func_t *funcs_get(funcs_t head, const char *name);
/* Retourne le pointeur de la fonction */
func_ptr_t funcs_get_ptr(funcs_t funcs, const char *name);

/* Appelle la fonction nommée 'func_name', si elle est présente dans la liste
 * 'funcs', avec les arguments passés en paramètres.
 * Retourne la valeur de retour de la fonction appelée */
#define funcs_call(funcs, name, arg1, ...) 	\
		(funcs_get_ptr(funcs, name))(arg1, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* Not funcs_h_included */
