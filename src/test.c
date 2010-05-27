#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "error.h"
#include "stack.h"

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
	stack_t *S;
	test_t *t1, *t2, *T;

	Error_init();
	if((types_init(0)) < 0)
		pError();
	
	type_reg("test", sizeof(test_t));
	type_reg_func("test", "free", (func_ptr_t)&test_free);
	type_reg_func("test", "cmp", (func_ptr_t)&test_cmp);

	S = stack_new("test");
	t1 = test_new(2);
	t2 = test_new(4);
	stack_push(S, t1);
	stack_push(S, t2);
	T = stack_pop(S);
	printf("%s\n", T->s);
	/*T = stack_pop(S);
	printf("%s\n", T->s);
	*/stack_free(S);

	types_free();

	return 0;
}

