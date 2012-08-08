#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "exception.h"
#include "test_macros.h"
#include "types.h"

int init_suite(void){
	return gds_types_init(512);
}

int clean_suite(void){
	gds_types_free();
	return 0;
}

void t_gds_type_register(void){
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register(NULL));
	
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
	gds_func_ptr_t func_ptr = (gds_func_ptr_t)&test_func;
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func(NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func(NULL, NULL, func_ptr));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func(NULL, "", NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func(NULL, "", func_ptr));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func(NULL, "func1", NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func(NULL, "func1", func_ptr));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func("", NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func("", NULL, func_ptr));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func("", "", NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func("", "func1", NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func("newtype", NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func("newtype", NULL, func_ptr));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func("newtype", "", NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_register_func("newtype", "func1", NULL));

	CU_ASSERT(0 > gds_type_register_func("newtype", "", func_ptr));
	CU_ASSERT(0 > gds_type_register_func("newtype", "func1", func_ptr));

	CU_ASSERT(0 == gds_type_register_func("", "", func_ptr));
	CU_ASSERT(0 == gds_type_register_func("", "func1", func_ptr));
}

void t_type_get_func(void)
{
	int a = 2, b = 3;
	gds_func_ptr_t fp;

	GDS_ASSERT_THROW(BadArgumentException, gds_type_get_func(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_get_func(NULL, ""));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_get_func("", NULL));

	CU_ASSERT(NULL == gds_type_get_func("", "newfunc"));
	CU_ASSERT(NULL == gds_type_get_func("newtype", ""));
	
	fp = gds_type_get_func("","");
	CU_ASSERT_PTR_NOT_NULL(fp);
	CU_ASSERT_EQUAL(test_func(&a,&b), fp(&a,&b));
}

void t_type_unregister_func(void)
{
	GDS_ASSERT_THROW(BadArgumentException, gds_type_unregister_func(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_unregister_func(NULL, ""));
	GDS_ASSERT_THROW(BadArgumentException, gds_type_unregister_func("newtype", NULL));

	CU_ASSERT(0 > gds_type_unregister_func("newtype", ""));
	CU_ASSERT(0 > gds_type_unregister_func("", "newfunc"));

	CU_ASSERT(0 == gds_type_unregister_func("", ""));
	CU_ASSERT(0 == gds_type_unregister_func("", "func1"));

	CU_ASSERT(0 > gds_type_unregister_func("", ""));
}

void t_type_unregister(void)
{
	GDS_ASSERT_THROW(BadArgumentException, gds_type_unregister(NULL));

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
