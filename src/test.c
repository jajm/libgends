#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "error.h"
#include "types.h"
#include "generic.h"
#include "slist.h"
#include "dlist.h"
#include "funcs.h"

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

u32 tesf(void *v, int b, int c)
{
	int *a = v;
	printf("%d %d\n", c, INT32_MAX);
	return *a+b+c;
}

#define MAX 199
int main()
{
	dlist_t *list;
	u32 *tab[MAX];
	u32 i;
	u32 *i_ptr;

	Error_init();
	
	if((types_init(0)) < 0)
		pError();
	
	type_reg("test", sizeof(test_t));
	

	list = dlist_new("u32");
	for(i=0; i<MAX; i++){
		tab[i] = malloc(sizeof(u32));
		*tab[i] = i;
		dlist_add_last(list, tab[i]);
	}
	
	dlist_begin(list);
	while(!dlist_end(list)){
		i_ptr = (u32 *)dlist_get(list);
		if(i_ptr == NULL) pError();
		printf("%i ", *i_ptr);
		dlist_next(list);
	}
	printf("\n");
	
	dlist_free(list);
	types_free();

	return 0;
}

