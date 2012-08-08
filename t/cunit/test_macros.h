#ifndef test_macros_h_included
#define test_macros_h_included

#include <libexception/exception.h>
#include <CUnit/Basic.h>

#define GDS_ASSERT_THROW(exception, code) \
	do { \
		int exception_sent = 0; \
		try {\
			code; \
		} catch (exception) { \
			exception_sent = 1; \
			CU_PASS(#code " throwed " #exception); \
		} catch() { \
			exception_sent = 1; \
			CU_FAIL(#code " didn't throw " #exception); \
		} \
		if (exception_sent == 0) { \
			CU_FAIL(#code " didn't throw any exception"); \
		} \
	} while (0)

#endif /* Not test_macros_h_included */
