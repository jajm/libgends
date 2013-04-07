#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "test_macros.h"
#include "rbtree_keyin_node.h"
#include "test.h"

int init_suite(void)
{
	return 0;
}

int clean_suite(void)
{
	return 0;
}

void t_rbtree_keyin_node_new(void)
{
	test_t *t = test_new("key", 4);
	gds_rbtree_keyin_node_t *n;

	n = gds_rbtree_keyin_node_new(NULL);
	CU_ASSERT_PTR_NOT_NULL(n);
	CU_ASSERT_PTR_NULL(gds_rbtree_keyin_node_get_data(n));
	gds_rbtree_keyin_node_free(n, NULL);

	n = gds_rbtree_keyin_node_new(t);
	CU_ASSERT_PTR_NOT_NULL(n)
	CU_ASSERT_PTR_EQUAL(t, gds_rbtree_keyin_node_get_data(n));
	gds_rbtree_keyin_node_free(n, NULL);
	
	n = gds_rbtree_keyin_node_new(test_alloc(t));
	CU_ASSERT_PTR_NOT_NULL(n);
	CU_ASSERT_PTR_NOT_NULL(gds_rbtree_keyin_node_get_data(n));
	CU_ASSERT_PTR_NOT_EQUAL(t, gds_rbtree_keyin_node_get_data(n));
	gds_rbtree_keyin_node_free(n, (gds_free_cb)test_free);

	test_free(t);
}

void t_rbtree_keyin_node_get_data(void)
{
	test_t *t;
	test_t *data;
	gds_rbtree_keyin_node_t *n;

	t = test_new("key", 4);
	n = gds_rbtree_keyin_node_new(t);

	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_node_get_data(NULL));

	data = gds_rbtree_keyin_node_get_data(n);
	CU_ASSERT_PTR_NOT_NULL(data);
	CU_ASSERT_PTR_EQUAL(t, data);
	CU_ASSERT_EQUAL(test_getvalue(t), test_getvalue(data));
	CU_ASSERT_PTR_EQUAL(test_getkey(t), test_getkey(data));
	CU_ASSERT(0 == test_cmpkey(test_getkey(t), test_getkey(data)));

	gds_rbtree_keyin_node_free(n, (gds_free_cb)test_free);
}

void t_rbtree_keyin_node_set_data(void)
{
	test_t *t = test_new("key", 4);
	gds_rbtree_keyin_node_t *n;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	n = gds_rbtree_keyin_node_new(NULL);

	/* Returns always a negative value if first parameter is NULL */
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_node_set_data(NULL, NULL, NULL   ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_node_set_data(NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_node_set_data(NULL, t   , NULL   ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_node_set_data(NULL, t   , free_cb));

	/* Set NULL as data. */
	CU_ASSERT(0 == gds_rbtree_keyin_node_set_data(n, NULL, NULL   ));
	CU_ASSERT(0 == gds_rbtree_keyin_node_set_data(n, NULL, NULL   ));
	CU_ASSERT(0 == gds_rbtree_keyin_node_set_data(n, NULL, free_cb));
	CU_ASSERT(0 == gds_rbtree_keyin_node_set_data(n, NULL, free_cb));

	/* Set t as data. */
	CU_ASSERT(0 == gds_rbtree_keyin_node_set_data(n, test_alloc(t), NULL   ));
	CU_ASSERT(0 == gds_rbtree_keyin_node_set_data(n, t, free_cb));

	gds_rbtree_keyin_node_free(n, free_cb);
}

int main()
{
	CU_pSuite pSuite = NULL;
	int fails = 0;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Compact Red-Black tree node",
		init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if(
	   (NULL == CU_add_test(pSuite, "gds_rbtree_keyin_node_new()", t_rbtree_keyin_node_new))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_keyin_node_get_data()", t_rbtree_keyin_node_get_data))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_keyin_node_set_data()", t_rbtree_keyin_node_set_data))
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
