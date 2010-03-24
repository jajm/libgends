#ifndef generic_h_included
#define generic_h_included

#include "types.h"
#include "error.h"

typedef struct generic_t{
	char *type_name;	/* Type de la donnée */
	void *data_ptr;		/* Pointeur vers la donnée */
} *generic_ptr;

/* Crée une donnée générique à partir des arguments */
generic_ptr generic(const char *type_name, const void *data_ptr, error_ptr *err);

/* Affecte une valeur à une donnée générique */
s8 generic_affect(generic_ptr *g, const char *type_name, const void *data_ptr,
	error_ptr *err);

/* Copie de donnée générique */
s8 generic_copy(generic_ptr *to, const generic_ptr from, error_ptr *err);

/* Comparaison de deux données génériques */
s32 generic_cmp(const generic_ptr g1, const generic_ptr g2);

/* Donne la taille d'une donnée générique */
u32 generic_size(const generic_ptr g);

/* Libère la mémoire */
void generic_free(generic_ptr *g);

#endif /* Not generic_h_included */

