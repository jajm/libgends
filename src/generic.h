/*****************************************************************************
 * Fichier		: generic.h                                          *
 * Brève Description	: Donnée générique                                   *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Une donnée générique associe un type (représenté par un nom unique, voir  *
 * types.h) et un pointeur générique (void *) vers une donnée de ce type.    *
 *****************************************************************************/

#ifndef generic_h_included
#define generic_h_included

typedef struct{
	char *type_name;	/* Type de la donnée */
	void *data_ptr;		/* Pointeur vers la donnée */
} generic_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Crée une donnée générique à partir des arguments */
generic_t * generic(const char *type_name, const void *data_ptr);

/* Affecte une valeur à une donnée générique */
s8 generic_affect(generic_t **g, const char *type_name, const void *data_ptr);

/* Copie de donnée générique */
s8 generic_copy(generic_t **to, const generic_t *from);

/* Comparaison de deux données génériques */
s32 generic_cmp(const generic_t *g1, const generic_t *g2);

/* Donne la taille d'une donnée générique */
u32 generic_size(const generic_t *g);

/* Libère la mémoire */
void generic_free(generic_t *g);

#ifdef __cplusplus
}
#endif

#endif /* Not generic_h_included */

