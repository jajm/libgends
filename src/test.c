#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "generic.h"
#include "SLList.h"

int main()
{
	generic_t truc, plop;
	void *machin;
	char *hoho = "abcdef";

	truc = generic(20, "ahahahahahahahahahahahahha");
	machin = malloc(8);
	memcpy(machin, hoho, 8);
	plop = machin;
	printf("sizeof(struct _generic_t) = %d\n", sizeof(struct
	_generic_t));

	printf("sizeof(truc) = %d\n", sizeof(truc));
	printf("sizeof(*truc) = %d\n", sizeof(double));
	printf("sizeof(machin) = %d\n", sizeof(machin));
	printf("sizeof(*machin) = %d\n", sizeof(*machin));

	return 0;
}

