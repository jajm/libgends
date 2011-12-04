#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "types.h"
#include "hash_map.h"

typedef struct{
	u32 i;
	char *s;
} test_t;

test_t * test_new(u32 i, const char *s)
{
	test_t *t;
	size_t len;

	t = malloc(sizeof(test_t));
	assert(t != NULL);
	t->i = i;
	len = strlen(s);
	t->s = malloc(len+1);
	assert(t->s != NULL);
	strncpy(t->s, s, len);

	return t;
}

void test_free(test_t *t)
{
	if(t){
		free(t->s);
		free(t);
	}
}

void test_print(test_t *t, u32 indent)
{
	u32 i;

	if(t == NULL){
		printf("NULL");
	} else {
		printf("{\n");
		for(i=0; i<indent+4; i++) printf(" ");
		printf("i => %d\n", t->i);
		for(i=0; i<indent+4; i++) printf(" ");
		printf("s => %s\n", t->s);
		for(i=0; i<indent; i++) printf(" ");
		printf("}");
	}
}

int init(void)
{
	types_init(512);
	type_reg("test", sizeof(test_t));
	type_reg_func("test", "free", (func_ptr_t)&test_free);
	type_reg_func("test", "print", (func_ptr_t)&test_print);
	return 0;
}

int clean(void)
{
	types_free();
	return 0;
}

int main()
{
	hash_map_t *h;
	u32 i;

	init();

	h = hash_map_new("test", 512, NULL);
	for(i=0; i<10; i++) {
		char key[512];
		char s[512];
		sprintf(key, "test_%d", i);
		sprintf(s, "s_%d", i);
		hash_map_set(
			h,
			key,
			test_new(i, s)
		);
	}
	hash_map_print(h);

	clean();
	return 0;
}
