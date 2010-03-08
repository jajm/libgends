#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "types.h"
#include "error.h"
#include "generic.h"

struct test_t{
	u32 i;
	double f;
	char *s;
};

int main()
{
	error_ptr error = NULL;
	generic_ptr gen;
	s8 c = 'c';
	s8 d;
	struct test_t t;
	struct test_t *t_ptr = NULL;

	type_init();
	error_init(&error);

	type_reg("test_t", sizeof(struct test_t));
	
	t.i = 23;
	t.f = 9.876;
	t.s = malloc(10);
	t.s[0] = 'a'; t.s[1] = 'b'; t.s[9] = '\0';
	gen = generic("test_t", &t, &error);
	if(!gen){
		error_print(error);
		return error->errno;
	}
	t_ptr = (struct test_t*)(gen->data_ptr);
	
	printf("\ni = %d\nf = %f\ns = %s\n", t_ptr->i, t_ptr->f, t_ptr->s);
	
	return 0;
}

