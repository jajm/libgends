#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "types.h"
#include "error.h"
#include "generic.h"

int main()
{
	error_ptr error = NULL;
	generic_ptr gen;
	s8 c = 'c';
	s8 d;

	type_init();
	error_init(&error);

	gen = generic("test", &c, &error);
	if(!gen){
		error_print(error);
		return error->errno;
	}
	d = *(s8*)(gen->data_ptr);
	if(d == 'c'){
		printf("Donnée récupérée avec succès!\n");
	}else{
		printf("Un problème est survenu...\n");
	}
	
	return 0;
}

