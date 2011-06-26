#include <stdlib.h>
#include <stdio.h>
#include <CUnit/Basic.h>
#include "error.h"

int init_suite(void){
	return 0;
}

int clean_suite(void){
	return 0;
}

void t_Error_at(void)
{
	char *msg = "Sample error message";
	char *file = "file_where_error_occured";
	char *function = "function_where_error_occured";
	u32 line = 42;

	Error_at(NULL, -1, NULL, NULL);
	CU_ASSERT(NULL == Error_msg());
	CU_ASSERT(NULL == Error_file());
	CU_ASSERT(NULL == Error_function());
	CU_ASSERT((u32)-1 == Error_line());

	Error_at(NULL, 0, NULL, msg);
	CU_ASSERT(0 == strcmp(msg, Error_msg()));
	CU_ASSERT(NULL == Error_file());
	CU_ASSERT(NULL == Error_function());
	CU_ASSERT(0 == Error_line());

	Error_at(NULL, line, function, msg);
	CU_ASSERT(0 == strcmp(msg, Error_msg()));
	CU_ASSERT(NULL == Error_file());
	CU_ASSERT(0 == strcmp(function, Error_function()));
	CU_ASSERT(line == Error_line());

	Error_at(NULL, line, function, NULL);
	CU_ASSERT(NULL ==  Error_msg());
	CU_ASSERT(NULL == Error_file());
	CU_ASSERT(0 == strcmp(function, Error_function()));
	CU_ASSERT(line == Error_line());

	Error_at(file, line, function, NULL);
	CU_ASSERT(NULL ==  Error_msg());
	CU_ASSERT(0 == strcmp(file, Error_file()));
	CU_ASSERT(0 == strcmp(function, Error_function()));
	CU_ASSERT(line == Error_line());

	Error_at(file, line, function, msg);
	CU_ASSERT(0 == strcmp(msg, Error_msg()));
	CU_ASSERT(0 == strcmp(file, Error_file()));
	CU_ASSERT(0 == strcmp(function, Error_function()));
	CU_ASSERT(line == Error_line());

	Error_at(file, line, NULL, msg);
	CU_ASSERT(0 == strcmp(msg, Error_msg()));
	CU_ASSERT(0 == strcmp(file, Error_file()));
	CU_ASSERT(NULL == Error_function());
	CU_ASSERT(line == Error_line());
}

void t_Error_at_with_args(void)
{
	char *msg = "Some vars : %d, %f, %s\n";
	int d = 42;
	float f = 4.2;
	char *s = "42";
	char buffer[512];

	Error_at(NULL, 0, NULL, msg, d, f, s);
	sprintf(buffer, msg, d, f, s);
	CU_ASSERT(0 == strcmp(buffer, Error_msg()) );
}

int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Errors", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if( (NULL == CU_add_test(pSuite, "Error_at()", t_Error_at))
	 || (NULL == CU_add_test(pSuite, "Error_at() with args", t_Error_at_with_args)) )
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
