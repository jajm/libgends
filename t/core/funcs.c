#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "funcs.h"

funcs_t funclist;

int init_suite(void){
	funclist = NULL;
	return 0;
}

int clean_suite(void){
	funcs_free(funclist);
	return 0;
}

int test_func(int* a, int* b)
{
	return *a+*b;
}

void t_func(void)
{
	func_t *f1, *f2, *f3, *f4, *f5, *f6;
	f1 = func_new(NULL, NULL);
	f2 = func_new(NULL, (func_ptr_t)&test_func);
	f3 = func_new("", NULL);
	f4 = func_new("", (func_ptr_t)&test_func);
	f5 = func_new("Test", NULL);
	f6 = func_new("Test", (func_ptr_t)&test_func);

	CU_ASSERT(NULL == f1);
	CU_ASSERT(NULL == f2);
	CU_ASSERT(NULL == f3);
	CU_ASSERT(NULL != f4);
	CU_ASSERT(NULL == f5);
	CU_ASSERT(NULL != f6);

	CU_ASSERT(NULL == func_get_name(NULL));
	CU_ASSERT(0 == strcmp("Test", func_get_name(f6)));

	CU_ASSERT(NULL == func_get_ptr(NULL));
	CU_ASSERT((func_ptr_t)&test_func == (func_ptr_t)func_get_ptr(f6));

	func_free(f1);
	func_free(f2);
	func_free(f3);
	func_free(f4);
	func_free(f5);
	func_free(f6);
}

void t_funcs_add(void)
{
	func_t *f1 = func_new("f1", (func_ptr_t)&test_func);
	func_t *f2 = func_new("f2", (func_ptr_t)&test_func);

	CU_ASSERT(NULL == funcs_add(NULL, NULL));
	CU_ASSERT(NULL == funcs_add(NULL, f1));
	CU_ASSERT(NULL == funcs_add(&funclist, NULL));
	CU_ASSERT(NULL != funcs_add(&funclist, f1));
	CU_ASSERT(NULL != funcs_add(&funclist, f2));
}

void t_funcs_get(void)
{
	CU_ASSERT(NULL == funcs_get(NULL, NULL));
	CU_ASSERT(NULL == funcs_get(NULL, ""));
	CU_ASSERT(NULL == funcs_get(NULL, "f1"));
	CU_ASSERT(NULL == funcs_get(funclist, NULL));
	CU_ASSERT(NULL == funcs_get(funclist, ""));
	func_t *f1 = funcs_get(funclist, "f1");
	func_t *f2 = funcs_get(funclist, "f2");
	CU_ASSERT(NULL != f1);
	CU_ASSERT(NULL != f2);
	CU_ASSERT(0 == strcmp("f1", f1->name));
	CU_ASSERT(0 == strcmp("f2", f2->name));
}

void t_funcs_get_ptr(void)
{
	CU_ASSERT(NULL == funcs_get_ptr(NULL, NULL));
	CU_ASSERT(NULL == funcs_get_ptr(NULL, ""));
	CU_ASSERT(NULL == funcs_get_ptr(NULL, "f1"));
	CU_ASSERT(NULL == funcs_get_ptr(funclist, NULL));
	CU_ASSERT(NULL == funcs_get_ptr(funclist, ""));
	func_ptr_t f1_ptr = funcs_get_ptr(funclist, "f1");
	func_ptr_t f2_ptr = funcs_get_ptr(funclist, "f2");
	CU_ASSERT(NULL != f1_ptr);
	CU_ASSERT(NULL != f2_ptr);
	int a=2, b=3;
	CU_ASSERT(test_func(&a,&b) == f1_ptr(&a, &b));
	CU_ASSERT(test_func(&a,&b) == f2_ptr(&a, &b));
}

void t_funcs_del(void)
{
	CU_ASSERT(0 > funcs_del(NULL, NULL));
	CU_ASSERT(0 > funcs_del(NULL, ""));
	CU_ASSERT(0 > funcs_del(NULL, "f1"));
	CU_ASSERT(0 > funcs_del(&funclist, NULL));
	CU_ASSERT(0 > funcs_del(&funclist, ""));
	CU_ASSERT(0 == funcs_del(&funclist, "f1"));
	CU_ASSERT(0 > funcs_del(&funclist, "f1"));
	CU_ASSERT(0 == funcs_del(&funclist, "f2"));
	CU_ASSERT(0 > funcs_del(&funclist, "f2"));
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
	if( (NULL == CU_add_test(pSuite, "func_*()", t_func))
	 || (NULL == CU_add_test(pSuite, "funcs_add()", t_funcs_add))
	 || (NULL == CU_add_test(pSuite, "funcs_get()", t_funcs_get))
	 || (NULL == CU_add_test(pSuite, "funcs_get_ptr()", t_funcs_get_ptr))
	 || (NULL == CU_add_test(pSuite, "funcs_del()", t_funcs_del)) )
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
