#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "types.h"
#include "container.h"
#include "error.h"

typedef struct {
	int32_t i;
	char c;
	double f;
} test_structure_t;

int init_suite(void){
	types_init(0);
	return 0;
}

int clean_suite(void){
	types_free();
	return 0;
}

void t_container_new(void)
{
	test_structure_t test;
	container_t *c;
	char name[] = "my-very#long/type name";

	/* container_new must return NULL if no type_name is given */
	CU_ASSERT(NULL == container_new(NULL, NULL, false));
	CU_ASSERT(NULL == container_new(NULL, NULL, true));
	CU_ASSERT(NULL == container_new(NULL, &test, false));
	CU_ASSERT(NULL == container_new(NULL, &test, true));

	/* the empty string is a valid type_name, but no data is given */
	CU_ASSERT(NULL == container_new("", NULL, false));
	CU_ASSERT(NULL == container_new("", NULL, true));

	/* Must succeed and return a valid pointer */
	CU_ASSERT(NULL != container_new("", &test, false));
	/* Will fail because "" type is not registered (needed to copy data) */
	CU_ASSERT(NULL == container_new("", &test, true));

	/* Same as above, with a real type_name, all characters are allowed */
	CU_ASSERT(NULL == container_new(name, NULL, false));
	CU_ASSERT(NULL == container_new(name, NULL, true));
	CU_ASSERT(NULL != container_new(name, &test, false));
	CU_ASSERT(NULL == container_new(name, &test, true));

	/* == Now register the two used types and re-test == */
	type_reg("", sizeof(test_structure_t));
	type_reg(name, sizeof(test_structure_t));

	/* Same as above because no data is given */
	CU_ASSERT(NULL == container_new("", NULL, false));
	CU_ASSERT(NULL == container_new("", NULL, true));

	/* Same as above */
	CU_ASSERT(NULL != container_new("", &test, false));
	/* Now that the type is registered, it must succeed */
	CU_ASSERT(NULL != container_new("", &test, true));

	/* Same as above : no data given */
	CU_ASSERT(NULL == container_new(name, NULL, false));
	CU_ASSERT(NULL == container_new(name, NULL, true));
	/* Same as above */
	CU_ASSERT(NULL != container_new(name, &test, false));
	/* Now that the type is registered, it must succeed */
	CU_ASSERT(NULL != container_new(name, &test, true));

	c = container_new(name, &test, false);
	CU_ASSERT(c != NULL);
	CU_ASSERT(c->type_name != name);
	CU_ASSERT(0 == strcmp(c->type_name, name));
	CU_ASSERT(c->data_ptr == &test);
	
	c = container_new(name, &test, true);
	CU_ASSERT(c != NULL);
	CU_ASSERT(c->type_name != name);
	CU_ASSERT(0 == strcmp(c->type_name, name));
	CU_ASSERT(c->data_ptr != &test);
	CU_ASSERT(0 == memcmp(c->data_ptr, &test, sizeof(test_structure_t)));

	container_free(c, true);
	type_unreg(name);
	type_unreg("");
}

void t_container_new_clone(void)
{
	test_structure_t test = {.i = 2, .c = 'b', .f = 2.0};
	container_t *c, *clone;

	type_reg("test_structure_t", sizeof(test_structure_t));
	c = container_new("test_structure_t", &test, true);

	CU_ASSERT(NULL != c);

	CU_ASSERT(NULL == container_new_clone(NULL));
	clone = container_new_clone(c);
	CU_ASSERT(NULL != clone)
	CU_ASSERT(0 == strcmp(clone->type_name, c->type_name));
	CU_ASSERT(clone->data_ptr != c->data_ptr);
	CU_ASSERT(0 == memcmp(clone->data_ptr, c->data_ptr, sizeof(test_structure_t)));

	container_free(c, true);
	container_free(clone, true);
	type_unreg("test_structure_t");
}

void t_container_set(void)
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
	container_t *c;
	char name[] = "test_structure_t";
	char name2[] = "test_structure";

	type_reg(name, sizeof(test_structure_t));
	c = container_new(name, &t, true);
	assert(c != NULL);

	/* Always fail if one of the first three parameters is NULL */
	CU_ASSERT(0 > container_set(NULL, NULL, NULL, false, false));
	CU_ASSERT(0 > container_set(NULL, NULL, NULL, false, true));
	CU_ASSERT(0 > container_set(NULL, NULL, NULL, true, false));
	CU_ASSERT(0 > container_set(NULL, NULL, NULL, true, true));
	CU_ASSERT(0 > container_set(NULL, NULL, &t2, false, false));
	CU_ASSERT(0 > container_set(NULL, NULL, &t2, false, true));
	CU_ASSERT(0 > container_set(NULL, NULL, &t2, true, false));
	CU_ASSERT(0 > container_set(NULL, NULL, &t2, true, true));
	CU_ASSERT(0 > container_set(NULL, name2, NULL, false, false));
	CU_ASSERT(0 > container_set(NULL, name2, NULL, false, true));
	CU_ASSERT(0 > container_set(NULL, name2, NULL, true, false));
	CU_ASSERT(0 > container_set(NULL, name2, NULL, true, true));
	CU_ASSERT(0 > container_set(NULL, name2, &t2, false, false));
	CU_ASSERT(0 > container_set(NULL, name2, &t2, false, true));
	CU_ASSERT(0 > container_set(NULL, name2, &t2, true, false));
	CU_ASSERT(0 > container_set(NULL, name2, &t2, true, true));
	CU_ASSERT(0 > container_set(c, NULL, NULL, false, false));
	CU_ASSERT(0 > container_set(c, NULL, NULL, false, true));
	CU_ASSERT(0 > container_set(c, NULL, NULL, true, false));
	CU_ASSERT(0 > container_set(c, NULL, NULL, true, true));
	CU_ASSERT(0 > container_set(c, NULL, &t2, false, false));
	CU_ASSERT(0 > container_set(c, NULL, &t2, false, true));
	CU_ASSERT(0 > container_set(c, NULL, &t2, true, false));
	CU_ASSERT(0 > container_set(c, NULL, &t2, true, true));
	CU_ASSERT(0 > container_set(c, name2, NULL, false, false));
	CU_ASSERT(0 > container_set(c, name2, NULL, false, true));
	CU_ASSERT(0 > container_set(c, name2, NULL, true, false));
	CU_ASSERT(0 > container_set(c, name2, NULL, true, true));

	/* Will fail because name2 is not a registered type_name */
	CU_ASSERT(0 > container_set(c, name2, &t2, true, true));
	CU_ASSERT(0 > container_set(c, name2, &t2, false, true));

	/* Clone freeing data and without copying data */
	CU_ASSERT(0 == container_set(c, name2, &t2, true, false));
	CU_ASSERT(0 == strcmp(c->type_name, name2));
	CU_ASSERT(c->data_ptr == &t2);

	/* Clone without freeing old data and without copying new data */
	CU_ASSERT(0 == container_set(c, name, &t, false, false));
	CU_ASSERT(0 == strcmp(c->type_name, name));
	CU_ASSERT(c->data_ptr == &t);

	type_reg(name2, sizeof(test_structure_t));

	/* Clone without freeing old data but copying new data */
	CU_ASSERT(0 == container_set(c, name, &t, false, true));
	CU_ASSERT(0 == strcmp(c->type_name, name));
	CU_ASSERT(c->data_ptr != &t);
	CU_ASSERT(0 == memcmp(c->data_ptr, &t, sizeof(test_structure_t)));

	/* Clone freeing old data and copying new data */
	CU_ASSERT(0 == container_set(c, name2, &t2, true, true));
	CU_ASSERT(0 == strcmp(c->type_name, name2));
	CU_ASSERT(c->data_ptr != &t2);
	CU_ASSERT(0 == memcmp(c->data_ptr, &t2, sizeof(test_structure_t)));

	container_free(c, true);
	type_unreg(name);
	type_unreg(name2);
}

void t_container_set_clone(void)
{
	container_t *c, *c2;
	char name[] = "test_structure_t";
	char name2[] = "test_structure_t2";
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	test_structure_t t2 = {.i = 2, .c = 'b', .f = 2.2};

	type_reg(name, sizeof(test_structure_t));
	c = container_new(name, &t, false);
	assert(c != NULL);

	type_reg(name2, sizeof(test_structure_t));
	c2 = container_new(name2, &t2, true);
	assert(c2 != NULL);

	CU_ASSERT(0 > container_set_clone(NULL, NULL, false));
	CU_ASSERT(0 > container_set_clone(NULL, NULL, true));
	CU_ASSERT(0 > container_set_clone(NULL, c2, false));
	CU_ASSERT(0 > container_set_clone(NULL, c2, true));
	CU_ASSERT(0 > container_set_clone(c, NULL, false));
	CU_ASSERT(0 > container_set_clone(c, NULL, true));

	CU_ASSERT(0 == container_set_clone(c, c2, false));
	CU_ASSERT(0 == strcmp(c->type_name, c2->type_name));
	CU_ASSERT(c->data_ptr != c2->data_ptr);
	CU_ASSERT(0 == memcmp(c->data_ptr, c2->data_ptr, sizeof(test_structure_t)));

	container_set(c, name, &t, true, true);
	CU_ASSERT(0 == container_set_clone(c, c2, true));
	CU_ASSERT(0 == strcmp(c->type_name, c2->type_name));
	CU_ASSERT(0 == strcmp(c->type_name, c2->type_name));
	CU_ASSERT(c->data_ptr != c2->data_ptr);
	CU_ASSERT(0 == memcmp(c->data_ptr, c2->data_ptr, sizeof(test_structure_t)));
	CU_ASSERT(c->data_ptr != c2->data_ptr);
	CU_ASSERT(0 == memcmp(c->data_ptr, c2->data_ptr, sizeof(test_structure_t)));

	container_free(c, true);
	container_free(c2, true);
	type_unreg(name);
	type_unreg(name2);
}

void t_container_get(void)
{
	container_t *c;
	char name[] = "test_structure_t";
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	void *data = NULL;

	type_reg(name, sizeof(test_structure_t));
	c = container_new(name, &t, false);
	assert(c != NULL);

	CU_ASSERT(NULL == container_get(NULL, false));
	CU_ASSERT(NULL == container_get(NULL, true));

	CU_ASSERT(NULL != (data = container_get(c, false)));
	CU_ASSERT(data == &t);
	CU_ASSERT(NULL != (data = container_get(c, true)));
	CU_ASSERT(data != &t);
	CU_ASSERT(0 == memcmp(data, &t, sizeof(test_structure_t)));

	container_free(c, false);
	free(data);
	type_unreg(name);
}

void t_container_data_size(void)
{
	container_t *c;
	char name[] = "test_structure_t";
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};

	c = container_new(name, &t, false);
	assert(c != NULL);

	CU_ASSERT(0 == container_data_size(NULL));
	CU_ASSERT(0 == container_data_size(c));
	type_reg(name, sizeof(test_structure_t));
	CU_ASSERT(sizeof(test_structure_t) == container_data_size(c));

	container_free(c, false);
	type_unreg(name);
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
	   (NULL == CU_add_test(pSuite, "container_new()", t_container_new))
	|| (NULL == CU_add_test(pSuite, "container_new_clone()", t_container_new_clone))
	|| (NULL == CU_add_test(pSuite, "container_set()", t_container_set))
	|| (NULL == CU_add_test(pSuite, "container_set_clone()", t_container_set_clone))
	|| (NULL == CU_add_test(pSuite, "container_get()", t_container_get))
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
