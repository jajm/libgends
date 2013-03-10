#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "test_macros.h"
#include "slist_node.h"
#include "callbacks.h"

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

test_structure_t * test_new(int i, char c, double f)
{
	test_structure_t *t = malloc(sizeof(test_structure_t));
	if (t) {
		t->i = i;
		t->c = c;
		t->f = f;
		return t;
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

	GDS_ASSERT_THROW(BadArgumentException, gds_slist_node_new(NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_slist_node_new(NULL));

	CU_ASSERT(NULL != (node = gds_slist_node_new(&test)));
	CU_ASSERT(node->data == &test);
	gds_slist_node_free(node, NULL, NULL);
}

void t_gds_slist_node_set_data(void)
{
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	test_structure_t t2 = {.i = 2, .c = 'b', .f = 2.2};
	gds_slist_node_t *node;
	gds_free_cb free_cb = (gds_free_cb) test_free;

	node = gds_slist_node_new(&t);
	CU_ASSERT_FATAL(node != NULL);

	/* Fail because the first parameter is NULL */
	GDS_ASSERT_THROW(BadArgumentException, gds_slist_node_set_data(NULL, NULL, NULL   , NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_slist_node_set_data(NULL, NULL, free_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_slist_node_set_data(NULL, &t2 , NULL   , NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_slist_node_set_data(NULL, &t2 , free_cb, NULL));

	CU_ASSERT(0 == gds_slist_node_set_data(node, NULL, NULL   , NULL));
	CU_ASSERT_PTR_NULL(node->data);
	CU_ASSERT(0 == gds_slist_node_set_data(node, NULL, free_cb, NULL));
	CU_ASSERT_PTR_NULL(node->data);

	CU_ASSERT(0 == gds_slist_node_set_data(node, &t2, NULL, NULL));
	CU_ASSERT(node->data == &t2);

	CU_ASSERT(0 == gds_slist_node_set_data(node, test_alloc(&t), NULL, NULL));
	CU_ASSERT(node->data != &t);

	CU_ASSERT(0 == gds_slist_node_set_data(node, test_alloc(&t2), free_cb, NULL));
	CU_ASSERT(node->data != &t2);

	CU_ASSERT(0 == gds_slist_node_set_data(node, &t, free_cb, NULL));
	CU_ASSERT(node->data == &t);

	gds_slist_node_free(node, NULL, NULL);
}

void t_gds_slist_node_get_data(void)
{
	gds_slist_node_t *node;
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	void *data;

	node = gds_slist_node_new(&t);
	CU_ASSERT_FATAL(node != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_slist_node_get_data(NULL));

	CU_ASSERT(NULL != (data = gds_slist_node_get_data(node)));
	CU_ASSERT(data == &t);

	gds_slist_node_free(node, NULL, NULL);
}

int main()
{
	CU_pSuite pSuite = NULL;
	int fails = 0;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Singly linked list node data structure",
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

	fails = CU_get_number_of_failures();
	CU_cleanup_registry();

	return fails ? EXIT_FAILURE : EXIT_SUCCESS;
}
