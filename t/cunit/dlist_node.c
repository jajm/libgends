#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "dlist_node.h"

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

void t_dlist_node_new(void)
{
	test_structure_t test;
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;
	gds_dlist_node_t *node;
	gds_free_cb free_cb = (gds_free_cb) test_free;

	CU_ASSERT(NULL == gds_dlist_node_new(NULL, NULL));
	CU_ASSERT(NULL == gds_dlist_node_new(NULL, alloc_cb));

	CU_ASSERT(NULL != (node = gds_dlist_node_new(&test, NULL)));
	CU_ASSERT(node->data == &test);
	CU_ASSERT(node->next == NULL);
	gds_dlist_node_free(node, NULL);

	CU_ASSERT(NULL != (node = gds_dlist_node_new(&test, alloc_cb)));
	CU_ASSERT(node->data != &test);
	CU_ASSERT(node->next == NULL);
	gds_dlist_node_free(node, free_cb);
}

void t_dlist_node_set_data(void)
{
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	test_structure_t t2 = {.i = 2, .c = 'b', .f = 2.2};
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;
	gds_dlist_node_t *node;
	gds_free_cb free_cb;

	node = gds_dlist_node_new(&t, NULL);
	assert(node != NULL);
	free_cb = (gds_free_cb) test_free;

	/* Fail because one of the two first parameters is NULL */
	CU_ASSERT(0 > gds_dlist_node_set_data(NULL, NULL, NULL   , NULL));
	CU_ASSERT(0 > gds_dlist_node_set_data(NULL, NULL, NULL   , alloc_cb));
	CU_ASSERT(0 > gds_dlist_node_set_data(NULL, NULL, free_cb, NULL));
	CU_ASSERT(0 > gds_dlist_node_set_data(NULL, NULL, free_cb, alloc_cb));
	CU_ASSERT(0 > gds_dlist_node_set_data(NULL, &t2 , NULL   , NULL));
	CU_ASSERT(0 > gds_dlist_node_set_data(NULL, &t2 , NULL   , alloc_cb));
	CU_ASSERT(0 > gds_dlist_node_set_data(NULL, &t2 , free_cb, NULL));
	CU_ASSERT(0 > gds_dlist_node_set_data(NULL, &t2 , free_cb, alloc_cb));
	CU_ASSERT(0 > gds_dlist_node_set_data(node, NULL, NULL   , NULL));
	CU_ASSERT(0 > gds_dlist_node_set_data(node, NULL, NULL   , alloc_cb));
	CU_ASSERT(0 > gds_dlist_node_set_data(node, NULL, free_cb, NULL));
	CU_ASSERT(0 > gds_dlist_node_set_data(node, NULL, free_cb, alloc_cb));

	CU_ASSERT(0 == gds_dlist_node_set_data(node, &t2, NULL, NULL));
	CU_ASSERT(node->data == &t2);

	CU_ASSERT(0 == gds_dlist_node_set_data(node, &t, NULL, alloc_cb));
	CU_ASSERT(node->data != &t);

	CU_ASSERT(0 == gds_dlist_node_set_data(node, &t2, free_cb, alloc_cb));
	CU_ASSERT(node->data != &t2);

	CU_ASSERT(0 == gds_dlist_node_set_data(node, &t, free_cb, NULL));
	CU_ASSERT(node->data == &t);

	gds_dlist_node_free(node, NULL);
}

void t_dlist_node_get_data(void)
{
	gds_dlist_node_t *node;
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	void *data;
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;

	node = gds_dlist_node_new(&t, NULL);
	assert(node != NULL);

	CU_ASSERT(NULL == gds_dlist_node_get_data(NULL, NULL));
	CU_ASSERT(NULL == gds_dlist_node_get_data(NULL, alloc_cb));

	CU_ASSERT(NULL != (data = gds_dlist_node_get_data(node, NULL)));
	CU_ASSERT(data == &t);

	CU_ASSERT(NULL != (data = gds_dlist_node_get_data(node, alloc_cb)));
	CU_ASSERT(data != &t);
	free(data);

	gds_dlist_node_free(node, NULL);
}

void t_dlist_node_set_next(void)
{
	gds_dlist_node_t *node, *next;
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	node = gds_dlist_node_new(&t, alloc_cb);
	assert(node != NULL);
	next = gds_dlist_node_new(&t, alloc_cb);
	assert(node != NULL);

	CU_ASSERT(0 > gds_dlist_node_set_next(NULL, NULL));
	CU_ASSERT(0 > gds_dlist_node_set_next(NULL, next));

	CU_ASSERT(0 == gds_dlist_node_set_next(node, next));
	CU_ASSERT(node->next == next);
	CU_ASSERT(0 == gds_dlist_node_set_next(node, NULL));
	CU_ASSERT(node->next == NULL);

	gds_dlist_node_free(node, free_cb);
	gds_dlist_node_free(next, free_cb);
}

void t_dlist_node_get_next(void)
{
	gds_dlist_node_t *node, *next;
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	node = gds_dlist_node_new(&t, alloc_cb);
	assert(node != NULL);
	next = gds_dlist_node_new(&t, alloc_cb);
	assert(node != NULL);

	CU_ASSERT(NULL == gds_dlist_node_get_next(NULL));
	CU_ASSERT(NULL == gds_dlist_node_get_next(node));
	gds_dlist_node_set_next(node, next);
	CU_ASSERT(next == gds_dlist_node_get_next(node));

	gds_dlist_node_free(node, free_cb);
	gds_dlist_node_free(next, free_cb);
}

void t_dlist_node_set_prev(void)
{
	gds_dlist_node_t *node, *prev;
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	node = gds_dlist_node_new(&t, alloc_cb);
	assert(node != NULL);
	prev = gds_dlist_node_new(&t, alloc_cb);
	assert(node != NULL);

	CU_ASSERT(0 > gds_dlist_node_set_prev(NULL, NULL));
	CU_ASSERT(0 > gds_dlist_node_set_prev(NULL, prev));

	CU_ASSERT(0 == gds_dlist_node_set_prev(node, prev));
	CU_ASSERT(node->prev == prev);
	CU_ASSERT(0 == gds_dlist_node_set_prev(node, NULL));
	CU_ASSERT(node->prev == NULL);

	gds_dlist_node_free(node, free_cb);
	gds_dlist_node_free(prev, free_cb);
}

void t_dlist_node_get_prev(void)
{
	gds_dlist_node_t *node, *prev;
	test_structure_t t = {.i = 1, .c = 'a', .f = 1.1};
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	node = gds_dlist_node_new(&t, alloc_cb);
	assert(node != NULL);
	prev = gds_dlist_node_new(&t, alloc_cb);
	assert(node != NULL);

	CU_ASSERT(NULL == gds_dlist_node_get_prev(NULL));
	CU_ASSERT(NULL == gds_dlist_node_get_prev(node));
	gds_dlist_node_set_prev(node, prev);
	CU_ASSERT(prev == gds_dlist_node_get_prev(node));

	gds_dlist_node_free(node, free_cb);
	gds_dlist_node_free(prev, free_cb);
}

int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Doubly linked list node data structure",
		init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if(
	   (NULL == CU_add_test(pSuite, "gds_dlist_node_new()", t_dlist_node_new))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_node_set_data()", t_dlist_node_set_data))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_node_get_data()", t_dlist_node_get_data))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_node_set_next()", t_dlist_node_set_next))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_node_get_next()", t_dlist_node_get_next))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_node_set_prev()", t_dlist_node_set_prev))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_node_get_prev()", t_dlist_node_get_prev))
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
