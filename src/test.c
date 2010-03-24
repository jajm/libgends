#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "types.h"
#include "error.h"
#include "generic.h"
#include "vw_slist.h"

struct test_t{
	u32 i;
	double f;
	char *s;
};

int main()
{
	error_ptr error = NULL;
	vw_slist_ptr list;
	struct test_t t1 = {1, 0.1, "un"}, t2 = {2, 0.2, "deux"};

	printf("a\n");

	error_init(&error);
	types_init(10, &error);
	if(type_reg("test_t", sizeof(struct test_t), &error) < 0)
		error_print(error);

	list = vw_slist_new(&error);
	if(list == NULL) error_print(error);
	vw_slist_add_last(list, generic("test_t", &t1, NULL), &error);
	error_print(error);
	vw_slist_add_last(list, generic("test_t", &t2, NULL), &error);
	error_print(error);
	vw_slist_print(list);

	types_free();
	error_free(&error);
	
	return 0;
}

