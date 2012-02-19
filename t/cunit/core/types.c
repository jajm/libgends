#include <stdlib.h>
#include <CUnit/Basic.h>
#include "types.h"

int init_suite(void){
	return gds_types_init(512);
}

int clean_suite(void){
	gds_types_free();
	return 0;
}

void t_gds_type_register(void){
	CU_ASSERT(0 > gds_type_register(NULL));
	
	CU_ASSERT(0 == gds_type_register(""));
	CU_ASSERT(0 == gds_type_register("type8"));
	CU_ASSERT(0 == gds_type_register("type42"));
	
	CU_ASSERT(0 < gds_type_register("type8"));
	CU_ASSERT(0 < gds_type_register("type42"));
}

int test_func(int* a, int* b)
{
	return *a+*b;
}

void t_gds_type_register_func(void)
{
	CU_ASSERT(0 > gds_type_register_func(NULL, NULL, (gds_func_ptr_t)NULL));
	CU_ASSERT(0 > gds_type_register_func(NULL, NULL, (gds_func_ptr_t)&test_func));
	CU_ASSERT(0 > gds_type_register_func(NULL, "", (gds_func_ptr_t)NULL));
	CU_ASSERT(0 > gds_type_register_func(NULL, "", (gds_func_ptr_t)&test_func));
	CU_ASSERT(0 > gds_type_register_func(NULL, "func1", (gds_func_ptr_t)NULL));
	CU_ASSERT(0 > gds_type_register_func(NULL, "func1", (gds_func_ptr_t)&test_func));
	CU_ASSERT(0 > gds_type_register_func("", NULL, (gds_func_ptr_t)NULL));
	CU_ASSERT(0 > gds_type_register_func("", NULL, (gds_func_ptr_t)&test_func));
	CU_ASSERT(0 > gds_type_register_func("", "", (gds_func_ptr_t)NULL));
	CU_ASSERT(0 > gds_type_register_func("", "func1", (gds_func_ptr_t)NULL));
	CU_ASSERT(0 > gds_type_register_func("newtype", NULL, (gds_func_ptr_t)NULL));
	CU_ASSERT(0 > gds_type_register_func("newtype", NULL, (gds_func_ptr_t)&test_func));
	CU_ASSERT(0 > gds_type_register_func("newtype", "", (gds_func_ptr_t)NULL));
	CU_ASSERT(0 > gds_type_register_func("newtype", "", (gds_func_ptr_t)&test_func));
	CU_ASSERT(0 > gds_type_register_func("newtype", "func1", (gds_func_ptr_t)NULL));
	CU_ASSERT(0 > gds_type_register_func("newtype", "func1", (gds_func_ptr_t)&test_func));

	CU_ASSERT(0 == gds_type_register_func("", "", (gds_func_ptr_t)&test_func));
	CU_ASSERT(0 == gds_type_register_func("", "func1", (gds_func_ptr_t)&test_func));
}

void t_type_get_func(void)
{
	CU_ASSERT(NULL == gds_type_get_func(NULL, NULL));
	CU_ASSERT(NULL == gds_type_get_func(NULL, ""));
	CU_ASSERT(NULL == gds_type_get_func("", NULL));
	CU_ASSERT(NULL == gds_type_get_func("", "newfunc"));
	CU_ASSERT(NULL == gds_type_get_func("newtype", ""));
	
	gds_func_ptr_t fp = gds_type_get_func("","");
	CU_ASSERT(NULL != fp);
	int a = 2, b = 3;
	CU_ASSERT(test_func(&a,&b) == fp(&a,&b));
}

void t_type_unregister_func(void)
{
	CU_ASSERT(0 > gds_type_unregister_func(NULL, NULL));
	CU_ASSERT(0 > gds_type_unregister_func(NULL, ""));
	CU_ASSERT(0 > gds_type_unregister_func("newtype", NULL));
	CU_ASSERT(0 > gds_type_unregister_func("newtype", ""));
	CU_ASSERT(0 > gds_type_unregister_func("", "newfunc"));

	CU_ASSERT(0 == gds_type_unregister_func("", ""));
	CU_ASSERT(0 == gds_type_unregister_func("", "func1"));

	CU_ASSERT(0 > gds_type_unregister_func("", ""));
}

void t_type_unregister(void)
{
	CU_ASSERT(0 > gds_type_unregister(NULL));
	CU_ASSERT(0 > gds_type_unregister("newtype"));

	CU_ASSERT(0 == gds_type_unregister(""));

	CU_ASSERT(0 > gds_type_unregister(""));
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
	    (NULL == CU_add_test(pSuite, "gds_type_register()", t_gds_type_register))
	 || (NULL == CU_add_test(pSuite, "gds_type_register_func()", t_gds_type_register_func))
	 || (NULL == CU_add_test(pSuite, "gds_type_get_func()", t_type_get_func))
	 || (NULL == CU_add_test(pSuite, "gds_type_unregister_func()", t_type_unregister_func))
	 || (NULL == CU_add_test(pSuite, "gds_type_unregister()", t_type_unregister))
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
