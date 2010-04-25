#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "error.h"
#include "types.h"
#include "generic.h"
#include "vw_slist.h"

typedef struct{
	double d;
	int i;
	char *s;
} test_t;

void print_test(test_t *t)
{
	if(t) printf("%s:\t%d\t%lf\n", t->s, t->i, t->d);
}
void print_test2(test_t *t)
{
	if(t) printf("%lf\t%d\t%s\n", t->d, t->i, t->s);
}

typedef union{
	char *c;
	double *d;
} cd;

int main()
{
	test_t t1 = {1.00, 1, "Un"}, t2 = {2.00, 2, "Deux"};
	test_t t3 = {3.00, 3, "Trois"}, t4 = {4.00, 4, "Quatre"};
	vw_slist_t *list;
	generic_t *g;
	test_t *t;

	Error_init();
	if((types_init(0)) < 0) pError();
	
	if(type_reg("test_t", sizeof(test_t)) < 0) pError();
	if(type_reg("test_t2", sizeof(test_t)) < 0) pError();
	if((list = vw_slist_new()) == NULL) pError();
	vw_slist_add_last(list, generic("test_t", &t1));
	vw_slist_add_last(list, generic("test_t2", &t2));
	vw_slist_add_last(list, generic("test_t", &t3));
	vw_slist_add_last(list, generic("test_t2", &t4));
	
	vw_slist_begin(list);
	while(g = vw_slist_pop_last(list)){
		if(strcmp(g->type_name, "test_t") == 0)
			print_test(g->data_ptr);
		else if(strcmp(g->type_name, "test_t2") == 0)
			print_test2(g->data_ptr);
		else
			printf("Heu...\n");
	}
	
	vw_slist_free_list(list);
	types_free();

	return 0;
}

