#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "types.h"
#include "stack.h"

typedef struct {
	int32_t i;
	char c;
	double f;
} test_structure_t;

test_structure_t * test_alloc(test_structure_t *t)
{
	test_structure_t *copy = malloc(sizeof(test_structure_t));
	if(copy && t) {
		copy->i = t->i;
		copy->c = t->c;
		copy->f = t->f;
		return copy;
	}
	return NULL;
}

void test_free(test_structure_t *t)
{
	free(t);
}

int init_suite(void){
	gds_types_init(0);
	return 0;
}

int clean_suite(void){
	gds_types_free();
	return 0;
}

void t_gds_stack_new(void)
{
	gds_stack_t *s;

	CU_ASSERT(NULL == gds_stack_new(NULL));
	CU_ASSERT(NULL != (s = gds_stack_new("")));
	gds_stack_free(s, false);
	CU_ASSERT(NULL != (s = gds_stack_new("basic_type_name")));
	gds_stack_free(s, false);
	CU_ASSERT(NULL != (s = gds_stack_new("name with spaces")));
	gds_stack_free(s, false);
	CU_ASSERT(NULL != (s = gds_stack_new("numb3rs & ()ther 5tuff... 'éèëêç'")));
	gds_stack_free(s, false);
}

void t_gds_stack_push_and_pop(void)
{
	gds_stack_t *s;
	test_structure_t *test;
	char *type_name = "type_name";
	int8_t i;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	test = calloc(1, sizeof(test_structure_t));

	s = gds_stack_new(type_name);

	for(i=0; i<=9; i++) {
		test->i = i;
		CU_ASSERT(0 == gds_stack_push(s, test, true));
	}
	test_free(test);

	for(i=9; i>=0; i--) {
		test = gds_stack_pop(s);
		CU_ASSERT(test != NULL);
		CU_ASSERT(test->i == i);
		test_free(test);
	}

	gds_stack_free(s, true);
	gds_type_unregister(type_name);
}


int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Container data structure",
		init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if(
	   (NULL == CU_add_test(pSuite, "gds_stack_new()", t_gds_stack_new))
	|| (NULL == CU_add_test(pSuite, "gds_stack push and pop", t_gds_stack_push_and_pop))
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}