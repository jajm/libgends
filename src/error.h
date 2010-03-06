#ifndef __error_h__
#define __error_h__

#include "types.h"

#define error_set(err, errno, errmsg) \
	_error_set(err, errno, errmsg, __FILE__, __func__, __LINE__)

typedef struct error_t{
	s32 errno;
	char *errmsg;
	char *file;
	char *function;
	u32 line;
} *error_ptr;

void error_init(error_ptr *err);

/* Affecte err avec les valeurs des paramètres suivants */
void _error_set(error_ptr *err, s32 errno, const char *errmsg,
	const char *file, const char *function, u32 line);

/* Affiche l'erreur enregistrée dans err avec le format suivant:
 * Error (errno) in function at file:line: errmsg */
void error_print(const error_ptr err);

void error_free(error_ptr *err);


#endif /* Not __error_h__ */

