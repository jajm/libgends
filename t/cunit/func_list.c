#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "test_macros.h"
#include "func_list.h"

gds_func_list_node_t *funclist;

int init_suite(void){
	funclist = NULL;
	return 0;
}

int clean_suite(void){
	gds_func_list_free(funclist);
	return 0;
}

int test_func(int* a, int* b)
{
	return *a+*b;
}

void t_gds_func_list_add(void)
{
	gds_func_ptr_t func_ptr = (gds_func_ptr_t)&test_func;

	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_add(NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_add(NULL, NULL, func_ptr));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_add(NULL, "f1", NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_add(NULL, "f1", func_ptr));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_add(&funclist, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_add(&funclist, NULL, func_ptr));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_add(&funclist, "f1", NULL));

	CU_ASSERT_PTR_NOT_NULL(gds_func_list_add(&funclist, "f1", func_ptr));
	CU_ASSERT_PTR_NOT_NULL(gds_func_list_add(&funclist, "f2", func_ptr));
}

void t_gds_func_list_get_ptr(void)
{
	gds_func_ptr_t f1_ptr, f2_ptr;
	int a = 2, b = 3;

	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_get_ptr(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_get_ptr(funclist, NULL));

	CU_ASSERT_PTR_NULL(gds_func_list_get_ptr(NULL, ""));
	CU_ASSERT_PTR_NULL(gds_func_list_get_ptr(NULL, "f1"));
	CU_ASSERT_PTR_NULL(gds_func_list_get_ptr(funclist, ""));

	f1_ptr = gds_func_list_get_ptr(funclist, "f1");
	f2_ptr = gds_func_list_get_ptr(funclist, "f2");
	CU_ASSERT_PTR_NOT_NULL(f1_ptr);
	CU_ASSERT_PTR_NOT_NULL(f2_ptr);
	CU_ASSERT_EQUAL(test_func(&a,&b), f1_ptr(&a, &b));
	CU_ASSERT_EQUAL(test_func(&a,&b), f2_ptr(&a, &b));
}

void t_gds_func_list_del(void)
{
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_del(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_del(NULL, ""));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_del(NULL, "f1"));
	GDS_ASSERT_THROW(BadArgumentException, gds_func_list_del(&funclist, NULL));

	CU_ASSERT(0 > gds_func_list_del(&funclist, ""));
	CU_ASSERT(0 == gds_func_list_del(&funclist, "f1"));
	CU_ASSERT(0 > gds_func_list_del(&funclist, "f1"));
	CU_ASSERT(0 == gds_func_list_del(&funclist, "f2"));
	CU_ASSERT(0 > gds_func_list_del(&funclist, "f2"));
}

int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Custom types", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if(
	    (NULL == CU_add_test(pSuite, "gds_func_list_add()", t_gds_func_list_add))
	 || (NULL == CU_add_test(pSuite, "gds_func_list_get_ptr()", t_gds_func_list_get_ptr))
	 || (NULL == CU_add_test(pSuite, "gds_func_list_del()", t_gds_func_list_del))
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
