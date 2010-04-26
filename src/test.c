#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "error.h"
#include "types.h"
#include "generic.h"
#include "slist.h"

typedef struct{
	int a;
	char *s;
} test_t;

test_t *test_new(int a)
{
	test_t *t = malloc(sizeof(test_t));
	t->a = a;
	t->s = malloc(512*sizeof(char));
	sprintf(t->s, "a = %d", t->a);
	return t;
}

void test_free(test_t *t)
{
	if(t){
		free(t->s);
		free(t);
	}
	printf("Correctly freed\n");
}

s32 test_cmp(test_t *t1, test_t *t2)
{
	if(t1 && t2){
		return t1->a - t2->a;
	}
	return 0;
}

int main()
{
	generic_t *g[10];
	generic_t *h;
	u32 entier[4] = {10, 20, 30, 40};
	double flottant[4] = {0.11, 0.22, 0.33, 0.44};
	test_t *t = test_new(20);
	test_t *u = test_new(23);
	test_t *v;
	slist_t *list;
	int i;

	Error_init();
	
	if((types_init(0)) < 0)
		pError();
	if((type_reg("test_t", sizeof(test_t),
	  (free_func_t)&test_free, (cmp_func_t)&test_cmp)) < 0)
		pError();
	if((type_reg("generic_t", sizeof(generic_t),
	  (free_func_t)&generic_free, (cmp_func_t)&generic_cmp)) < 0)
		pError();
	if((type_reg("double", sizeof(double), NULL, NULL)) < 0)
		pError();
	if((type_reg("u32", sizeof(u32), NULL, NULL)) < 0)
		pError();
	g[0] = generic("test_t", t);
	g[1] = generic("test_t", u);
	for(i=0; i<4; i++) g[i+2] = generic("u32", &entier[i]);
	for(i=0; i<4; i++) g[i+6] = generic("double", &flottant[i]);
	
	list = slist_new("generic_t");
	for(i=0; i<10; i++) slist_add_last(list, g[i]);

	slist_begin(list);
	while(!slist_end(list)){
		h = slist_get(list);
		if(strcmp(h->type_name, "test_t") == 0){
			v = h->data_ptr;
			printf("test_t\t%s\n", v->s);
		}else if(strcmp(h->type_name, "u32") == 0){
			printf("u32\t%d\n", *((u32*)h->data_ptr));
		}else if(strcmp(h->type_name, "double") == 0){
			printf("double\t%lf\n", *((double*)h->data_ptr));
		}else{
			printf("Heuuuu\n");
		}
		slist_next(list);
	}
	
	//for(i=0; i<10; i++) generic_free(g[i]);
	slist_free_list(list);

	types_free();

	return 0;
}

