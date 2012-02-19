#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "slist_node.h"

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
	return 0;
}

int clean_suite(void){
	return 0;
}

void t_gds_slist_node_new(void)
{
	test_structure_t test;
	gds_slist_node_t *node;
	gds_func_ptr_t alloc_f = (gds_func_ptr_t) &test_alloc;
	gds_func_ptr_t free_f = (gds_func_ptr_t) &test_free;

	CU_ASSERT(NULL == gds_slist_node_new(NULL, false, 0));
	CU_ASSERT(NULL == gds_slist_node_new(NULL, false, alloc_f));
	CU_ASSERT(NULL == gds_slist_node_new(NULL, true, 0));
	CU_ASSERT(NULL == gds_slist_node_new(NULL, true, alloc_f));

	CU_ASSERT(NULL != (node = gds_slist_node_new(&test, false, 0)));
	CU_ASSERT(node->data == &test);
	CU_ASSERT(node->next == NULL);
	gds_slist_node_free(node, false, NULL);

	CU_ASSERT(NULL != (node = gds_slist_node_new(&test, false, alloc_f)));
	CU_ASSERT(node->data == &test);
	CU_ASSERT(node->next == NULL);
	gds_slist_node_free(node, false, NULL);

	CU_ASSERT(NULL == gds_slist_node_new(&test, true, 0));

	CU_ASSERT(NULL != (node = gds_slist_node_new(&test, true, alloc_f)));
	CU_ASSERT(node->data != &test);
	CU_ASSERT(node->next == NULL);
	gds_slist_node_free(node, true, free_f);
}

void t_gds_slist_node_set_data(void)
{
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	test_structure_t t2 = {.i = 2, .c = 'b', .f = 2.2};
	gds_func_ptr_t alloc_f = (gds_func_ptr_t)&test_alloc;
	gds_slist_node_t *node;
	gds_func_ptr_t free_f;

	node = gds_slist_node_new(&t, false, 0);
	assert(node != NULL);
	free_f = (gds_func_ptr_t) &test_free;

	/* Fail because one of the two first parameters is NULL */
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, false, NULL, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, false, NULL, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, false, NULL, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, false, NULL, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, false, free_f, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, false, free_f, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, false, free_f, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, false, free_f, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, true, NULL, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, true, NULL, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, true, NULL, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, true, NULL, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, true, free_f, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, true, free_f, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, true, free_f, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, NULL, true, free_f, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, false, NULL, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, false, NULL, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, false, NULL, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, false, NULL, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, false, free_f, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, false, free_f, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, false, free_f, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, false, free_f, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, true, NULL, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, true, NULL, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, true, NULL, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, true, NULL, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, true, free_f, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, true, free_f, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, true, free_f, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(NULL, &t2, true, free_f, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, false, NULL, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, false, NULL, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, false, NULL, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, false, NULL, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, false, free_f, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, false, free_f, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, false, free_f, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, false, free_f, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, true, NULL, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, true, NULL, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, true, NULL, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, true, NULL, true, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, true, free_f, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, true, free_f, false, alloc_f));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, true, free_f, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, NULL, true, free_f, true, alloc_f));

	/* Fail because copy_data is true but alloc_f is 0 */
	CU_ASSERT(0 > gds_slist_node_set_data(node, &t2, false, NULL, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, &t2, false, free_f, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, &t2, true, NULL, true, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, &t2, true, free_f, true, 0));

	/* Don't free memory, dont copy data */
	CU_ASSERT(0 == gds_slist_node_set_data(node, &t2, false, NULL, false, 0));
	CU_ASSERT(node->data == &t2);

	CU_ASSERT(0 == gds_slist_node_set_data(node, &t, false, NULL, false, alloc_f));
	CU_ASSERT(node->data == &t);

	CU_ASSERT(0 == gds_slist_node_set_data(node, &t2, false, free_f, false, 0));
	CU_ASSERT(node->data == &t2);

	CU_ASSERT(0 == gds_slist_node_set_data(node, &t, false, free_f, false, alloc_f));
	CU_ASSERT(node->data == &t);


	/* Don't free memory, copy data */
	CU_ASSERT(0 == gds_slist_node_set_data(node, &t2, false, NULL, true, alloc_f));
	CU_ASSERT(node->data != &t2);

	free(node->data); /* Just to avoid memory loss */
	CU_ASSERT(0 == gds_slist_node_set_data(node, &t, false, free_f, true, alloc_f));
	CU_ASSERT(node->data != &t);


	/* Free memory, copy data */
	/* Fails because no function is given for free */
	CU_ASSERT(0 > gds_slist_node_set_data(node, &t2, true, NULL, true, alloc_f));
	CU_ASSERT(0 == gds_slist_node_set_data(node, &t, true, free_f, true, alloc_f));
	CU_ASSERT(node->data != &t);


	/* Free memory, don't copy data */
	CU_ASSERT(0 > gds_slist_node_set_data(node, &t2, true, NULL, false, 0));
	CU_ASSERT(0 > gds_slist_node_set_data(node, &t2, true, NULL, false, alloc_f));


	gds_slist_node_set_data(node, &t, false, NULL, true, alloc_f);
	CU_ASSERT(0 == gds_slist_node_set_data(node, &t2, true, free_f, false, 0));
	CU_ASSERT(node->data == &t2);

	gds_slist_node_set_data(node, &t, false, NULL, true, alloc_f);
	CU_ASSERT(0 == gds_slist_node_set_data(node, &t2, true, free_f, false, alloc_f));
	CU_ASSERT(node->data == &t2);

	gds_slist_node_free(node, false, NULL);
}

void t_gds_slist_node_get_data(void)
{
	gds_slist_node_t *node;
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	void *data;
	gds_func_ptr_t alloc_f = (gds_func_ptr_t)&test_alloc;

	node = gds_slist_node_new(&t, false, 0);
	assert(node != NULL);

	CU_ASSERT(NULL == gds_slist_node_get_data(NULL, false, 0));
	CU_ASSERT(NULL == gds_slist_node_get_data(NULL, false, alloc_f));
	CU_ASSERT(NULL == gds_slist_node_get_data(NULL, true, 0));
	CU_ASSERT(NULL == gds_slist_node_get_data(NULL, true, alloc_f));
	CU_ASSERT(NULL == gds_slist_node_get_data(node, true, 0));

	CU_ASSERT(NULL != (data = gds_slist_node_get_data(node, false, 0)));
	CU_ASSERT(data == &t);

	CU_ASSERT(NULL != (data = gds_slist_node_get_data(node, false, alloc_f)));
	CU_ASSERT(data == &t);

	CU_ASSERT(NULL != (data = gds_slist_node_get_data(node, true, alloc_f)));
	CU_ASSERT(data != &t);
	free(data);

	gds_slist_node_free(node, false, NULL);
}

void t_gds_slist_node_set_next(void)
{
	gds_slist_node_t *node, *next;
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	gds_func_ptr_t alloc_f = (gds_func_ptr_t)&test_alloc;
	gds_func_ptr_t free_f = (gds_func_ptr_t)&test_free;

	node = gds_slist_node_new(&t, true, alloc_f);
	assert(node != NULL);
	next = gds_slist_node_new(&t, true, alloc_f);
	assert(node != NULL);

	CU_ASSERT(0 > gds_slist_node_set_next(NULL, NULL));
	CU_ASSERT(0 > gds_slist_node_set_next(NULL, next));

	CU_ASSERT(0 == gds_slist_node_set_next(node, next));
	CU_ASSERT(node->next == next);
	CU_ASSERT(0 == gds_slist_node_set_next(node, NULL));
	CU_ASSERT(node->next == NULL);

	gds_slist_node_free(node, true, free_f);
	gds_slist_node_free(next, true, free_f);
}

void t_gds_slist_node_get_next(void)
{
	gds_slist_node_t *node, *next;
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	gds_func_ptr_t alloc_f = (gds_func_ptr_t)&test_alloc;
	gds_func_ptr_t free_f = (gds_func_ptr_t)&test_free;

	node = gds_slist_node_new(&t, true, alloc_f);
	assert(node != NULL);
	next = gds_slist_node_new(&t, true, alloc_f);
	assert(node != NULL);

	CU_ASSERT(NULL == gds_slist_node_get_next(NULL));
	CU_ASSERT(NULL == gds_slist_node_get_next(node));
	gds_slist_node_set_next(node, next);
	CU_ASSERT(next == gds_slist_node_get_next(node));

	gds_slist_node_free(node, true, free_f);
	gds_slist_node_free(next, true, free_f);
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
	   (NULL == CU_add_test(pSuite, "gds_slist_node_new()", t_gds_slist_node_new))
	|| (NULL == CU_add_test(pSuite, "gds_slist_node_set_data()", t_gds_slist_node_set_data))
	|| (NULL == CU_add_test(pSuite, "gds_slist_node_get_data()", t_gds_slist_node_get_data))
	|| (NULL == CU_add_test(pSuite, "gds_slist_node_set_next()", t_gds_slist_node_set_next))
	|| (NULL == CU_add_test(pSuite, "gds_slist_node_get_next()", t_gds_slist_node_get_next))
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
