/*****************************************************************************
 * Fichier		: basic_types.h                                      *
 * Description Brève	: Liste de 'typedef' pour les types entiers          *
 * Auteur		: Julian Maurice                                     *
 * Créé le		: 01/03/2010                                         *
 *****************************************************************************
 * Les types définis ici utilisent les types de C99.                         *
 * Les types sont nommés aXX, où a est soit 'u', soit 's', selon qu'il soit  *
 * respectivement 'Unsigned', ou 'Signed'.                                   *
 * XX est la taille minimum du type, en bits. Par exemple, u32 est le type   *
 * d'un entier non signé qui occupe au moins 32 bits en mémoire.             *
 *****************************************************************************/

#ifndef basic_types_h_included
#define basic_types_h_included

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#endif /* Not basic_types_h_included */

