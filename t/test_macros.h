#ifndef test_macros_h_included
#define test_macros_h_included

#include <libexception/exception.h>

#define GDS_ASSERT_THROW(exception, code) \
	do { \
		int exception_sent = 0; \
		try {\
			code; \
		} catch (exception) { \
			exception_sent = 1; \
			pass(#code " throwed " #exception); \
		} catch() { \
			exception_sent = 1; \
			fail(#code " didn't throw " #exception); \
		} \
		if (exception_sent == 0) { \
			fail(#code " didn't throw any exception"); \
		} \
	} while (0)

#endif /* ! test_macros_h_included */
