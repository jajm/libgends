#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "error.h"

int main()
{
	error_ptr error = NULL;
	
	error_init(&error);

	error_set(&error, -4, "Une erreur pas banale");
	error_set(&error, 2, "Une autre erreur");

	error_print(error);
	error_free(&error);

	return 0;
}

