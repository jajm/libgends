#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "compact_rbtree_node.h"
#include "test.h"

int init_suite(void)
{
	return 0;
}

int clean_suite(void)
{
	return 0;
}

void t_compact_rbtree_node_new(void)
{
	test_t *t = test_new("key", 4);
	gds_compact_rbtree_node_t *n;

	n = gds_compact_rbtree_node_new(NULL, NULL);
	CU_ASSERT(NULL != n);
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(NULL == gds_compact_rbtree_node_get_data(n, NULL));
	gds_compact_rbtree_node_free(n, NULL);

	n = gds_compact_rbtree_node_new(NULL, (gds_alloc_cb)test_alloc);
	CU_ASSERT(NULL != n);
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(NULL == gds_compact_rbtree_node_get_data(n, NULL));
	gds_compact_rbtree_node_free(n, NULL);
	
	n = gds_compact_rbtree_node_new(t, NULL);
	CU_ASSERT(NULL != n)
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(t == gds_compact_rbtree_node_get_data(n, NULL));
	gds_compact_rbtree_node_free(n, NULL);
	
	n = gds_compact_rbtree_node_new(t, (gds_alloc_cb)test_alloc);
	CU_ASSERT(NULL != n);
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(NULL != gds_compact_rbtree_node_get_data(n, NULL));
	CU_ASSERT(t != gds_compact_rbtree_node_get_data(n, NULL));
	gds_compact_rbtree_node_free(n, (gds_free_cb)test_free);

	test_free(t);
}

void t_compact_rbtree_node_get_data(void)
{
	test_t *t;
	test_t *data;
	gds_compact_rbtree_node_t *n;

	t = test_new("key", 4);
	n = gds_compact_rbtree_node_new(t, NULL);

	data = gds_compact_rbtree_node_get_data(NULL, NULL);
	CU_ASSERT(NULL == data);

	data = gds_compact_rbtree_node_get_data(NULL, (gds_alloc_cb)test_alloc);
	CU_ASSERT(NULL == data);

	data = gds_compact_rbtree_node_get_data(n, NULL);
	CU_ASSERT(NULL != data);
	CU_ASSERT(t == data);
	CU_ASSERT(test_getvalue(t) == test_getvalue(data));
	CU_ASSERT(test_getkey(t) == test_getkey(data));
	CU_ASSERT(0 == test_cmpkey(test_getkey(t), test_getkey(data)));

	data = gds_compact_rbtree_node_get_data(n, (gds_alloc_cb)test_alloc);
	CU_ASSERT(NULL != data);
	CU_ASSERT(t != data);
	CU_ASSERT(test_getvalue(t) == test_getvalue(data));
	CU_ASSERT(test_getkey(t) != test_getkey(data));
	CU_ASSERT(0 == test_cmpkey(test_getkey(t), test_getkey(data)));

	test_free(data);
	gds_compact_rbtree_node_free(n, (gds_free_cb)test_free);
}

void t_compact_rbtree_node_set_data(void)
{
	test_t *t = test_new("key", 4);
	gds_compact_rbtree_node_t *n;
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	n = gds_compact_rbtree_node_new(NULL, NULL);

	/* Returns always a negative value if first parameter is NULL */
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, NULL    , NULL   ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, NULL    , free_cb));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, alloc_cb, NULL   ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, alloc_cb, free_cb));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , NULL    , NULL   ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , NULL    , free_cb));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , alloc_cb, NULL   ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , alloc_cb, free_cb));

	/* Set NULL as data. */
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, NULL    , NULL   ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, alloc_cb, NULL   ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, alloc_cb, free_cb));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, NULL    , free_cb));

	/* Set t as data. */
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , NULL    , NULL   ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , alloc_cb, NULL   ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , alloc_cb, free_cb));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , NULL    , free_cb));

	gds_compact_rbtree_node_free(n, free_cb);
}

int main()
{
	CU_pSuite pSuite = NULL;

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
	   (NULL == CU_add_test(pSuite, "gds_compact_rbtree_node_new()", t_compact_rbtree_node_new))
	|| (NULL == CU_add_test(pSuite, "gds_compact_rbtree_node_get_data()", t_compact_rbtree_node_get_data))
	|| (NULL == CU_add_test(pSuite, "gds_compact_rbtree_node_set_data()", t_compact_rbtree_node_set_data))
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
