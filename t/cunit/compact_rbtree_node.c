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

	n = gds_compact_rbtree_node_new(NULL, false, NULL);
	CU_ASSERT(NULL != n);
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(NULL == gds_compact_rbtree_node_get_data(n, false, NULL));
	gds_compact_rbtree_node_free(n, false, NULL);

	n = gds_compact_rbtree_node_new(NULL, false, (gds_func_ptr_t)test_alloc);
	CU_ASSERT(NULL != n);
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(NULL == gds_compact_rbtree_node_get_data(n, false, NULL));
	gds_compact_rbtree_node_free(n, false, NULL);

	n = gds_compact_rbtree_node_new(NULL, true, NULL);
	CU_ASSERT(NULL == n);
	CU_ASSERT(false == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(NULL == gds_compact_rbtree_node_get_data(n, false, NULL));
	
	n = gds_compact_rbtree_node_new(NULL, true, (gds_func_ptr_t)test_alloc);
	CU_ASSERT(NULL != n);
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(NULL == gds_compact_rbtree_node_get_data(n, false, NULL));
	gds_compact_rbtree_node_free(n, false, NULL);
	
	n = gds_compact_rbtree_node_new(t, false, NULL);
	CU_ASSERT(NULL != n)
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(t == gds_compact_rbtree_node_get_data(n, false, NULL));
	gds_compact_rbtree_node_free(n, false, NULL);
	
	n = gds_compact_rbtree_node_new(t, false, (gds_func_ptr_t)test_alloc);
	CU_ASSERT(NULL != n);
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(t == gds_compact_rbtree_node_get_data(n, false, NULL));
	gds_compact_rbtree_node_free(n, false, NULL);
	
	n = gds_compact_rbtree_node_new(t, true, NULL);
	CU_ASSERT(NULL == n);
	CU_ASSERT(false == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(NULL == gds_compact_rbtree_node_get_data(n, false, NULL));
	
	n = gds_compact_rbtree_node_new(t, true, (gds_func_ptr_t)test_alloc);
	CU_ASSERT(NULL != n);
	CU_ASSERT(true == gds_compact_rbtree_node_is_red(n));
	CU_ASSERT(NULL != gds_compact_rbtree_node_get_data(n, false, NULL));
	CU_ASSERT(t != gds_compact_rbtree_node_get_data(n, false, NULL));
	gds_compact_rbtree_node_free(n, true, (gds_func_ptr_t)test_free);

	test_free(t);
}

void t_compact_rbtree_node_get_data(void)
{
	test_t *t;
	test_t *data;
	gds_compact_rbtree_node_t *n;

	t = test_new("key", 4);
	n = gds_compact_rbtree_node_new(t, false, NULL);

	data = gds_compact_rbtree_node_get_data(NULL, false, NULL);
	CU_ASSERT(NULL == data);

	data = gds_compact_rbtree_node_get_data(NULL, false, (gds_func_ptr_t)test_alloc);
	CU_ASSERT(NULL == data);

	data = gds_compact_rbtree_node_get_data(NULL, true, NULL);
	CU_ASSERT(NULL == data);

	data = gds_compact_rbtree_node_get_data(NULL, true, (gds_func_ptr_t)test_alloc);
	CU_ASSERT(NULL == data);

	data = gds_compact_rbtree_node_get_data(n, false, NULL);
	CU_ASSERT(NULL != data);
	CU_ASSERT(t == data);
	CU_ASSERT(test_getvalue(t) == test_getvalue(data));
	CU_ASSERT(test_getkey(t) == test_getkey(data));
	CU_ASSERT(0 == test_cmpkey(test_getkey(t), test_getkey(data)));

	data = gds_compact_rbtree_node_get_data(n, false, (gds_func_ptr_t)test_alloc);
	CU_ASSERT(NULL != data);
	CU_ASSERT(t == data);
	CU_ASSERT(test_getvalue(t) == test_getvalue(data));
	CU_ASSERT(test_getkey(t) == test_getkey(data));
	CU_ASSERT(0 == test_cmpkey(test_getkey(t), test_getkey(data)));

	data = gds_compact_rbtree_node_get_data(n, true, NULL);
	CU_ASSERT(NULL == data);

	data = gds_compact_rbtree_node_get_data(n, true, (gds_func_ptr_t)test_alloc);
	CU_ASSERT(NULL != data);
	CU_ASSERT(t != data);
	CU_ASSERT(test_getvalue(t) == test_getvalue(data));
	CU_ASSERT(test_getkey(t) != test_getkey(data));
	CU_ASSERT(0 == test_cmpkey(test_getkey(t), test_getkey(data)));

	test_free(data);
	gds_compact_rbtree_node_free(n, true, (gds_func_ptr_t)test_free);
}

void t_compact_rbtree_node_set_data(void)
{
	test_t *t = test_new("key", 4);
	//test_t *data;
	gds_compact_rbtree_node_t *n;

	n = gds_compact_rbtree_node_new(NULL, false, NULL);

	/* Returns always a negative value if first parameter is NULL */
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, false, NULL                      , false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, false, NULL                      , false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, false, NULL                      , true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, false, NULL                      , true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, false, (gds_func_ptr_t)test_alloc, false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, false, (gds_func_ptr_t)test_alloc, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, false, (gds_func_ptr_t)test_alloc, true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, false, (gds_func_ptr_t)test_alloc, true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, true , NULL                      , false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, true , NULL                      , false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, true , NULL                      , true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, true , NULL                      , true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, true , (gds_func_ptr_t)test_alloc, false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, true , (gds_func_ptr_t)test_alloc, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, true , (gds_func_ptr_t)test_alloc, true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, NULL, true , (gds_func_ptr_t)test_alloc, true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , false, NULL                      , false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , false, NULL                      , false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , false, NULL                      , true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , false, NULL                      , true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , false, (gds_func_ptr_t)test_alloc, false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , false, (gds_func_ptr_t)test_alloc, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , false, (gds_func_ptr_t)test_alloc, true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , false, (gds_func_ptr_t)test_alloc, true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , true , NULL                      , false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , true , NULL                      , false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , true , NULL                      , true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , true , NULL                      , true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , true , (gds_func_ptr_t)test_alloc, false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , true , (gds_func_ptr_t)test_alloc, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , true , (gds_func_ptr_t)test_alloc, true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(NULL, t   , true , (gds_func_ptr_t)test_alloc, true , (gds_func_ptr_t)test_free));

	/* Set NULL as data. */
	/* Fail because 3rd parameter is true and 4th is NULL (or 5th is true and 6th is NULL) */
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , NULL, false, NULL                      , true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , NULL, false, (gds_func_ptr_t)test_alloc, true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , NULL, true , NULL                      , false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , NULL, true , NULL                      , false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , NULL, true , NULL                      , true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , NULL, true , NULL                      , true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , NULL, true , (gds_func_ptr_t)test_alloc, true , NULL                     ));
	/* No alloc, no free */
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, false, NULL                      , false, NULL                     ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, false, NULL                      , false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, false, (gds_func_ptr_t)test_alloc, false, NULL                     ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, false, (gds_func_ptr_t)test_alloc, false, (gds_func_ptr_t)test_free));
	/* Alloc and free, alternatively */
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, true , (gds_func_ptr_t)test_alloc, false, NULL                     ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, true , (gds_func_ptr_t)test_alloc, true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, false, NULL                      , true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, true , (gds_func_ptr_t)test_alloc, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , NULL, false, (gds_func_ptr_t)test_alloc, true , (gds_func_ptr_t)test_free));

	/* Set t as data. The order of tests is modified to fit the alloc/free process in order to avoid segmentation faults. */
	/* Fails */
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , t   , false, NULL                      , true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , t   , false, (gds_func_ptr_t)test_alloc, true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , t   , true , NULL                      , false, NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , t   , true , NULL                      , false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , t   , true , NULL                      , true , NULL                     ));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , t   , true , NULL                      , true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 >  gds_compact_rbtree_node_set_data(n   , t   , true , (gds_func_ptr_t)test_alloc, true , NULL                     ));
	/* No alloc no free */
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , false, NULL                      , false, NULL                     ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , false, NULL                      , false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , false, (gds_func_ptr_t)test_alloc, false, NULL                     ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , false, (gds_func_ptr_t)test_alloc, false, (gds_func_ptr_t)test_free));
	/* Alloc and free, alternatively */
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , true , (gds_func_ptr_t)test_alloc, false, NULL                     ));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , true , (gds_func_ptr_t)test_alloc, true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , false, NULL                      , true , (gds_func_ptr_t)test_free));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , true , (gds_func_ptr_t)test_alloc, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 == gds_compact_rbtree_node_set_data(n   , t   , false, (gds_func_ptr_t)test_alloc, true , (gds_func_ptr_t)test_free));

	gds_compact_rbtree_node_free(n, true, (gds_func_ptr_t)test_free);
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
