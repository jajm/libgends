#include <stdlib.h>
#include <CUnit/Basic.h>
#include "types.h"

int init_suite(void){
	return types_init(512);
}

int clean_suite(void){
	types_free();
	return 0;
}

void t_type_reg(void){
	CU_ASSERT(0 > type_reg(NULL, 0));
	CU_ASSERT(0 > type_reg(NULL, 1));
	CU_ASSERT(0 > type_reg("", 0));
	CU_ASSERT(0 > type_reg("type0", 0));
	
	CU_ASSERT(0 == type_reg("", 1));
	CU_ASSERT(0 == type_reg("type8", 8));
	CU_ASSERT(0 == type_reg("type42", 42));
	
	CU_ASSERT(0 < type_reg("type8", 8));
	CU_ASSERT(0 < type_reg("type42", 42));
	CU_ASSERT(0 < type_reg("type42", 43));
}

int test_func(int* a, int* b)
{
	return *a+*b;
}

void t_type_reg_func(void)
{
	CU_ASSERT(0 > type_reg_func(NULL, NULL, (func_ptr_t)NULL));
	CU_ASSERT(0 > type_reg_func(NULL, NULL, (func_ptr_t)&test_func));
	CU_ASSERT(0 > type_reg_func(NULL, "", (func_ptr_t)NULL));
	CU_ASSERT(0 > type_reg_func(NULL, "", (func_ptr_t)&test_func));
	CU_ASSERT(0 > type_reg_func(NULL, "func1", (func_ptr_t)NULL));
	CU_ASSERT(0 > type_reg_func(NULL, "func1", (func_ptr_t)&test_func));
	CU_ASSERT(0 > type_reg_func("", NULL, (func_ptr_t)NULL));
	CU_ASSERT(0 > type_reg_func("", NULL, (func_ptr_t)&test_func));
	CU_ASSERT(0 > type_reg_func("", "", (func_ptr_t)NULL));
	CU_ASSERT(0 > type_reg_func("", "func1", (func_ptr_t)NULL));
	CU_ASSERT(0 > type_reg_func("newtype", NULL, (func_ptr_t)NULL));
	CU_ASSERT(0 > type_reg_func("newtype", NULL, (func_ptr_t)&test_func));
	CU_ASSERT(0 > type_reg_func("newtype", "", (func_ptr_t)NULL));
	CU_ASSERT(0 > type_reg_func("newtype", "", (func_ptr_t)&test_func));
	CU_ASSERT(0 > type_reg_func("newtype", "func1", (func_ptr_t)NULL));
	CU_ASSERT(0 > type_reg_func("newtype", "func1", (func_ptr_t)&test_func));

	CU_ASSERT(0 == type_reg_func("", "", (func_ptr_t)&test_func));
	CU_ASSERT(0 == type_reg_func("", "func1", (func_ptr_t)&test_func));
}

void t_type_get_func(void)
{
	CU_ASSERT(NULL == type_get_func(NULL, NULL));
	CU_ASSERT(NULL == type_get_func(NULL, ""));
	CU_ASSERT(NULL == type_get_func("", NULL));
	CU_ASSERT(NULL == type_get_func("", "newfunc"));
	CU_ASSERT(NULL == type_get_func("newtype", ""));
	
	func_ptr_t fp = type_get_func("","");
	CU_ASSERT(NULL != fp);
	int a = 2, b = 3;
	CU_ASSERT(test_func(&a,&b) == fp(&a,&b));
}

void t_type_unreg_func(void)
{
	CU_ASSERT(0 > type_unreg_func(NULL, NULL));
	CU_ASSERT(0 > type_unreg_func(NULL, ""));
	CU_ASSERT(0 > type_unreg_func("newtype", NULL));
	CU_ASSERT(0 > type_unreg_func("newtype", ""));
	CU_ASSERT(0 > type_unreg_func("", "newfunc"));

	CU_ASSERT(0 == type_unreg_func("", ""));
	CU_ASSERT(0 == type_unreg_func("", "func1"));

	CU_ASSERT(0 > type_unreg_func("", ""));
}

void t_type_unreg(void)
{
	CU_ASSERT(0 > type_unreg(NULL));
	CU_ASSERT(0 > type_unreg("newtype"));

	CU_ASSERT(0 == type_unreg(""));

	CU_ASSERT(0 > type_unreg(""));
}

void t_type_sizeof(void)
{
	CU_ASSERT(0 == type_sizeof(NULL));
	CU_ASSERT(0 == type_sizeof(""));
	
	CU_ASSERT(8 == type_sizeof("type8"));
	CU_ASSERT(42 == type_sizeof("type42"));
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
	if( (NULL == CU_add_test(pSuite, "type_reg()", t_type_reg))
	 || (NULL == CU_add_test(pSuite, "type_reg_func()", t_type_reg_func))
	 || (NULL == CU_add_test(pSuite, "type_get_func()", t_type_get_func))
	 || (NULL == CU_add_test(pSuite, "type_unreg_func()", t_type_unreg_func))
	 || (NULL == CU_add_test(pSuite, "type_unreg()", t_type_unreg))
	 || (NULL == CU_add_test(pSuite, "type_sizeof()", t_type_sizeof)))
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
