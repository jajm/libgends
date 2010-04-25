/*****************************************************************************
 * Fichier		: error.h                                            *
 * Description Brève	: Gestion d'erreurs                                  *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Une 'erreur' est ici représentée par un message, et la                    *
 * localisation de l'erreur (fichier, ligne, fonction).                      *
 * Une fois une erreur enregistrée (par l'appel de Error_at, Error ou ErrorP)*
 * elle peut être affichée par l'appel de pError.                            *
 * ErrorP concatène la dernière erreur avec la courante, en faisant suivre   *
 * celle-ci de la chaine suivante : "\n -> "                                 *
 *****************************************************************************
 * Utilisation:                                                              *
 * 	Error_init();	// Init avant la première utilisation                *
 * 	Error("malloc failed");                                              *
 *	ErrorP("creation failed");                                           *
 *	pError();                                                            *
 *	// Affiche, à supposer qu'on soit dans la fonction main, dans main.c *
 *	// "Error in main at main.c:17: creation failed"                     *
 *	// " -> Error in main at main.c:16: malloc failed"                   *
 *	Error_at("doc.xml", NULL, 42, "Invalid syntax");                     *
 * 	pError();                                                            *
 *	// "Error at doc.xml:42: Invalid syntax"                             *
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

