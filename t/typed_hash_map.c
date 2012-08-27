#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "typed_hash_map.h"

typedef struct{
	uint32_t i;
	char *s;
} test_t;

test_t * test_new(uint32_t i, const char *s)
{
	test_t *t;
	size_t len;

	t = malloc(sizeof(test_t));
	t->i = i;
	len = strlen(s);
	t->s = malloc(len+1);
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

void test_print(test_t *t, uint32_t indent)
{
	uint32_t i;

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
	gds_types_init(512);
	gds_type_register("test");
	gds_type_register_func("test", "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func("test", "print", (gds_func_ptr_t)&test_print);
	return 0;
}

int clean(void)
{
	gds_types_free();
	return 0;
}

int main()
{
	gds_typed_hash_map_t *h;
	uint32_t i;

	init();

	h = gds_typed_hash_map_new("test", 512, NULL);
	for(i=0; i<10; i++) {
		char key[512];
		char s[512];
		sprintf(key, "test_%d", i);
		sprintf(s, "s_%d", i);
		gds_typed_hash_map_set(
			h,
			key,
			true,
			test_new(i, s),
			false
		);
	}

	clean();
	return 0;
}

