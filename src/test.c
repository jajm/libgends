#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "error.h"

int main()
{
	struct error_t *error;
	
	error = malloc(sizeof(struct error_t));
	error_init(error);

	error_set(error, -4, "Une erreur pas banale");

	error_print(error);
	error_free(error);

	return 0;
}

