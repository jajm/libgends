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
 * Fichier		: error.h                                            *
 * Description Brève	: Gestion d'erreurs                                  *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Une 'erreur' est ici représentée par un message, et la                    *
 * localisation de l'erreur (fichier, ligne, fonction).                      *
 * Une fois une erreur enregistrée (par l'appel de Error_at ou Error,        *
 * elle peut être affichée par l'appel de pError.                            *
 *****************************************************************************
 * Utilisation:                                                              *
 * 	Error_init();	// Init avant la première utilisation                *
 * 	Error("malloc failed");                                              *
 *	Error_at("doc.xml", 42, NULL, "Invalid syntax");                     *
 * 	pError();                                                            *
 *****************************************************************************/

#ifndef error_h_included
#define error_h_included

#include "basic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* À appeler avant tout appel à Error_at où pError */
void Error_init(void);

/* Enregistre un message d'erreur avec sa localisation */
void Error_at(const char *file, u32 line, const char *function,
	const char *msg, ...);
#define Error(errmsg, ...) \
	Error_at(__FILE__, __LINE__, __func__, errmsg, ##__VA_ARGS__)
#define ErrorP(errmsg, ...) \
	Error(errmsg "\n -> %s", ##__VA_ARGS__, Error_string())

/* Affiche la dernière erreur enregistrée avec le format suivant:
 * Error in function at file:line: errmsg */
void pError(void);

/* Fonctions de récupération des infos concernant une erreur */
char * Error_msg(void);
char * Error_file(void);
char * Error_function(void);
u32 Error_line(void);
/* Renvoie la chaine entière telle qu'affichée par pError() */
char * Error_string(void);

#ifdef __cplusplus
}
#endif

#endif /* Not error_h_included */

