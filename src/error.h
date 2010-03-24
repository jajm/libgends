#ifndef error_h_included
#define error_h_included

#include "basic_types.h"

#define error_set(err, errno, errmsg, ...) \
	_error_set(err, errno, __FILE__, __func__, __LINE__, \
		errmsg, ##__VA_ARGS__)

typedef struct error_t{
	s32 errno;
	char *errmsg;
	char *file;
	char *function;
	u32 line;
} *error_ptr;

#ifdef __cplusplus
extern "C" {
#endif

void error_init(error_ptr *err);

/* Affecte err avec les valeurs des paramètres suivants */
void _error_set(error_ptr *err, s32 errno, const char *file,
	const char *function, u32 line, const char *errmsg, ...);

/* Affiche l'erreur enregistrée dans err avec le format suivant:
 * Error (errno) in function at file:line: errmsg */
void error_print(const error_ptr err);

void error_free(error_ptr *err);

#ifdef __cplusplus
}
#endif

#endif /* Not error_h_included */

