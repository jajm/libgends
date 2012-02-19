#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
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
	CU_ASSERT(NULL == gds_func_list_add(NULL, NULL, (gds_func_ptr_t)&test_func));
	CU_ASSERT(NULL == gds_func_list_add(NULL, NULL, NULL));
	CU_ASSERT(NULL == gds_func_list_add(NULL, "f1", (gds_func_ptr_t)&test_func));
	CU_ASSERT(NULL == gds_func_list_add(NULL, "f1", NULL));
	CU_ASSERT(NULL == gds_func_list_add(&funclist, NULL, (gds_func_ptr_t)&test_func));
	CU_ASSERT(NULL == gds_func_list_add(&funclist, NULL, NULL));
	CU_ASSERT(NULL != gds_func_list_add(&funclist, "f1", (gds_func_ptr_t)&test_func));
	CU_ASSERT(NULL == gds_func_list_add(&funclist, "f1", NULL));

	CU_ASSERT(NULL != gds_func_list_add(&funclist, "f2", (gds_func_ptr_t)&test_func));
}

void t_gds_func_list_get_ptr(void)
{
	CU_ASSERT(NULL == gds_func_list_get_ptr(NULL, NULL));
	CU_ASSERT(NULL == gds_func_list_get_ptr(NULL, ""));
	CU_ASSERT(NULL == gds_func_list_get_ptr(NULL, "f1"));
	CU_ASSERT(NULL == gds_func_list_get_ptr(funclist, NULL));
	CU_ASSERT(NULL == gds_func_list_get_ptr(funclist, ""));
	gds_func_ptr_t f1_ptr = gds_func_list_get_ptr(funclist, "f1");
	gds_func_ptr_t f2_ptr = gds_func_list_get_ptr(funclist, "f2");
	CU_ASSERT(NULL != f1_ptr);
	CU_ASSERT(NULL != f2_ptr);
	int a=2, b=3;
	CU_ASSERT(test_func(&a,&b) == f1_ptr(&a, &b));
	CU_ASSERT(test_func(&a,&b) == f2_ptr(&a, &b));
}

void t_gds_func_list_del(void)
{
	CU_ASSERT(0 > gds_func_list_del(NULL, NULL));
	CU_ASSERT(0 > gds_func_list_del(NULL, ""));
	CU_ASSERT(0 > gds_func_list_del(NULL, "f1"));
	CU_ASSERT(0 > gds_func_list_del(&funclist, NULL));
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
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
