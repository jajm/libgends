#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "exception.h"
#include "types.h"
#include "typed_container.h"
#include "log.h"
#include "test_macros.h"

typedef struct {
	int32_t i;
	char c;
	double f;
} test_structure_t;

test_structure_t * test_alloc(test_structure_t *t)
{
	test_structure_t *copy;
	if(t == NULL) {
		return NULL;
	}
	copy = malloc(sizeof(test_structure_t));
	if(copy) {
		copy->i = t->i;
		copy->c = t->c;
		copy->f = t->f;
	}
	return copy;
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

void t_gds_typed_container_new(void)
{
	test_structure_t test;
	gds_typed_container_t *c;
	char name[] = "my-very#long/type name";

	/* gds_typed_container_new must throw BadArgumentException if no type_name is given */
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new(NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new(NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new(NULL, &test, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new(NULL, &test, true));

	/* the empty string is a valid type_name, but no data is given */
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new("", NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new("", NULL, true));

	/* Must succeed and return a valid pointer */
	CU_ASSERT_PTR_NOT_NULL(gds_typed_container_new("", &test, false));
	/* Will fail because "" type is not registered (needed to copy data) */
	GDS_ASSERT_THROW(FunctionNotFoundException, gds_typed_container_new("", &test, true));

	/* Same as above, with a real type_name, all characters are allowed */
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new(name, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new(name, NULL, true));
	CU_ASSERT_PTR_NOT_NULL(gds_typed_container_new(name, &test, false));
	GDS_ASSERT_THROW(FunctionNotFoundException, gds_typed_container_new(name, &test, true));


	/* == Now register the two used types and re-test == */
	gds_type_register("");
	gds_type_register_func("", "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func("", "free", (gds_func_ptr_t)&test_free);
	gds_type_register(name);
	gds_type_register_func(name, "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func(name, "free", (gds_func_ptr_t)&test_free);

	/* Same as above because no data is given */
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new("", NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new("", NULL, true));

	/* Same as above */
	CU_ASSERT_PTR_NOT_NULL(gds_typed_container_new("", &test, false));
	/* Now that the type is registered, it must succeed */
	CU_ASSERT_PTR_NOT_NULL(gds_typed_container_new("", &test, true));

	/* Same as above : no data given */
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new(name, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new(name, NULL, true));
	/* Same as above */
	CU_ASSERT_PTR_NOT_NULL(gds_typed_container_new(name, &test, false));
	/* Now that the type is registered, it must succeed */
	CU_ASSERT_PTR_NOT_NULL(gds_typed_container_new(name, &test, true));

	c = gds_typed_container_new(name, &test, false);
	CU_ASSERT_PTR_NOT_NULL_FATAL(c);
	CU_ASSERT_PTR_NOT_EQUAL(c->type_name, name);
	CU_ASSERT_STRING_EQUAL(c->type_name, name);
	CU_ASSERT_PTR_EQUAL(c->data_ptr, &test);
	gds_typed_container_free(c, false);
	
	c = gds_typed_container_new(name, &test, true);
	CU_ASSERT_PTR_NOT_NULL_FATAL(c);
	CU_ASSERT_PTR_NOT_EQUAL(c->type_name, name);
	CU_ASSERT_STRING_EQUAL(c->type_name, name);
	CU_ASSERT_PTR_NOT_EQUAL(c->data_ptr, &test);
	gds_typed_container_free(c, true);

	gds_type_unregister(name);
	gds_type_unregister("");
}

void t_gds_typed_container_new_clone(void)
{
	test_structure_t test = {.i = 2, .c = 'b', .f = 2.0};
	gds_typed_container_t *c, *clone;

	gds_type_register("test_structure_t");
	gds_type_register_func("test_structure_t", "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func("test_structure_t", "free", (gds_func_ptr_t)&test_free);
	c = gds_typed_container_new("test_structure_t", &test, true);

	CU_ASSERT_PTR_NOT_NULL(c);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_new_clone(NULL));
	clone = gds_typed_container_new_clone(c);
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone)
	CU_ASSERT_STRING_EQUAL(clone->type_name, c->type_name);
	CU_ASSERT_PTR_NOT_EQUAL(clone->data_ptr, c->data_ptr);
	gds_typed_container_free(clone, true);

	gds_typed_container_free(c, true);
	gds_type_unregister("test_structure_t");
}

void t_gds_typed_container_set(void)
{
	test_structure_t t = {
		.i = 1,
		.c = 'a',
		.f = 1.1
	};
	test_structure_t t2 = {
		.i = 2,
		.c = 'b',
		.f = 2.2
	};
	gds_typed_container_t *c;
	char name[] = "test_structure_t";
	char name2[] = "test_structure";

	gds_type_register(name);
	gds_type_register_func(name, "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func(name, "free", (gds_func_ptr_t)&test_free);
	c = gds_typed_container_new(name, &t, true);
	CU_ASSERT_PTR_NOT_NULL_FATAL(c);

	/* Always fail if one of the first three parameters is NULL */
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, NULL, NULL, false, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, NULL, NULL, false, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, NULL, NULL, true, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, NULL, NULL, true, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, NULL, &t2, false, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, NULL, &t2, false, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, NULL, &t2, true, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, NULL, &t2, true, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, name2, NULL, false, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, name2, NULL, false, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, name2, NULL, true, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, name2, NULL, true, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, name2, &t2, false, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, name2, &t2, false, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, name2, &t2, true, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(NULL, name2, &t2, true, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, NULL, NULL, false, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, NULL, NULL, false, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, NULL, NULL, true, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, NULL, NULL, true, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, NULL, &t2, false, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, NULL, &t2, false, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, NULL, &t2, true, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, NULL, &t2, true, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, name2, NULL, false, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, name2, NULL, false, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, name2, NULL, true, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set(c, name2, NULL, true, true));

	/* Will fail because name2 is not a registered type_name */
	GDS_ASSERT_THROW(FunctionNotFoundException, gds_typed_container_set(c, name2, &t2, true, true));
	GDS_ASSERT_THROW(FunctionNotFoundException, gds_typed_container_set(c, name2, &t2, false, true));

	/* Clone freeing data and without copying data */
	gds_typed_container_set(c, name2, &t2, true, false);
	CU_ASSERT_STRING_EQUAL(c->type_name, name2);
	CU_ASSERT_PTR_EQUAL(c->data_ptr, &t2);

	/* Clone without freeing old data and without copying new data */
	gds_typed_container_set(c, name, &t, false, false);
	CU_ASSERT_STRING_EQUAL(c->type_name, name);
	CU_ASSERT_PTR_EQUAL(c->data_ptr, &t);

	gds_type_register(name2);
	gds_type_register_func(name2, "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func(name2, "free", (gds_func_ptr_t)&test_free);

	/* Clone without freeing old data but copying new data */
	gds_typed_container_set(c, name, &t, false, true);
	CU_ASSERT_STRING_EQUAL(c->type_name, name);
	CU_ASSERT_PTR_NOT_EQUAL(c->data_ptr, &t);

	/* Clone freeing old data and copying new data */
	gds_typed_container_set(c, name2, &t2, true, true);
	CU_ASSERT_STRING_EQUAL(c->type_name, name2);
	CU_ASSERT_PTR_NOT_EQUAL(c->data_ptr, &t2);

	gds_typed_container_free(c, true);
	gds_type_unregister(name);
	gds_type_unregister(name2);
}

void t_gds_typed_container_set_clone(void)
{
	gds_typed_container_t *c, *c2;
	char name[] = "test_structure_t";
	char name2[] = "test_structure_t2";
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	test_structure_t t2 = {.i = 2, .c = 'b', .f = 2.2};

	gds_type_register(name);
	gds_type_register_func(name, "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func(name, "free", (gds_func_ptr_t)&test_alloc);
	c = gds_typed_container_new(name, &t, false);
	CU_ASSERT_PTR_NOT_NULL_FATAL(c);

	gds_type_register(name2);
	gds_type_register_func(name2, "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func(name2, "free", (gds_func_ptr_t)&test_free);
	c2 = gds_typed_container_new(name2, &t2, true);
	CU_ASSERT_PTR_NOT_NULL_FATAL(c2);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set_clone(NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set_clone(NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set_clone(NULL, c2, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set_clone(NULL, c2, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set_clone(c, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_set_clone(c, NULL, true));

	gds_typed_container_set_clone(c, c2, false);
	CU_ASSERT_STRING_EQUAL(c->type_name, c2->type_name);
	CU_ASSERT_PTR_NOT_EQUAL(c->data_ptr, c2->data_ptr);

	gds_typed_container_set(c, name, &t, true, true);
	gds_typed_container_set_clone(c, c2, true);
	CU_ASSERT_STRING_EQUAL(c->type_name, c2->type_name);
	CU_ASSERT_PTR_NOT_EQUAL(c->data_ptr, c2->data_ptr);

	gds_typed_container_free(c, true);
	gds_typed_container_free(c2, true);
	gds_type_unregister(name);
	gds_type_unregister(name2);
}

void t_gds_typed_container_get(void)
{
	gds_typed_container_t *c;
	char name[] = "test_structure_t";
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	void *data = NULL;

	gds_type_register(name);
	gds_type_register_func(name, "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func(name, "free", (gds_func_ptr_t)&test_free);
	c = gds_typed_container_new(name, &t, false);
	CU_ASSERT_PTR_NOT_NULL_FATAL(c);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_get(NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_container_get(NULL, true));

	data = gds_typed_container_get(c, false);
	CU_ASSERT_PTR_NOT_NULL(data);
	CU_ASSERT_PTR_EQUAL(data, &t);

	data = gds_typed_container_get(c, true);
	CU_ASSERT_PTR_NOT_NULL(data);
	CU_ASSERT_PTR_NOT_EQUAL(data, &t);

	free(data);

	gds_typed_container_free(c, false);
	gds_type_unregister(name);
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
	   (NULL == CU_add_test(pSuite, "gds_typed_container_new()", t_gds_typed_container_new))
	|| (NULL == CU_add_test(pSuite, "gds_typed_container_new_clone()", t_gds_typed_container_new_clone))
	|| (NULL == CU_add_test(pSuite, "gds_typed_container_set()", t_gds_typed_container_set))
	|| (NULL == CU_add_test(pSuite, "gds_typed_container_set_clone()", t_gds_typed_container_set_clone))
	|| (NULL == CU_add_test(pSuite, "gds_typed_container_get()", t_gds_typed_container_get))
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	try {
		CU_basic_run_tests();
	} catch() as (e) {
		fprintf(stderr, "\nTests returned an unexpected exception\n");
		fprintf(stderr, "\tType: %s\n", e->type());
		fprintf(stderr, "\tMessage: %s\n", e->message());
		fprintf(stderr, "\tFile: %s\n", e->filename());
		fprintf(stderr, "\tFunction: %s\n", e->function());
		fprintf(stderr, "\tLine: %d\n", e->line());
		return EXIT_FAILURE;
	}
	CU_cleanup_registry();
	return CU_get_error();
}
