#ifndef __types_h__
#define __types_h__

#include <stdint.h>

typedef uint_least8_t u8;
typedef uint_least16_t u16;
typedef uint_least32_t u32;
typedef uint_least64_t u64;

typedef int_least8_t s8;
typedef int_least16_t s16;
typedef int_least32_t s32;
typedef int_least64_t s64;

/*
Les fonctions suivantes permettent de créer de nouveaux types
identifiés par un nom unique.
*/
/* À appeler avant d'utiliser les autres fonctions */
void type_init(void);
/* Enregistrer un nouveau type */
s8 type_reg(const char *type_name, u32 type_size);
/* Supprimer un type existant */
s8 type_unreg(const char *type_name);
/* Obtenir la taille d'un type existant */
u32 type_sizeof(const char *type_name);


#endif /* Not __types_h__ */

