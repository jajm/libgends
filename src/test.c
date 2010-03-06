#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "types.h"

int main()
{
	type_init();

	type_reg("GROSSE STRUCTURE", 455);
	type_reg(NULL, 4);
	type_reg("ptite struct", 0);
	
	type_print();
	printf("\n");
	
	type_unreg("s16");
	type_unreg("u16");
	type_unreg("n'existe pas");
	type_unreg("string");

	type_print();

	return 0;
}

