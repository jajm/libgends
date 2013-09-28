#ifndef tap_h_included
#define tap_h_included

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int current = 0;

#define plan(n) \
	printf("1..%d\n", n)

#define ok(test, msg, ...) \
	do { \
		const char *_msg = msg; \
		current++; \
		if (test) printf("ok %d ", current); \
		else printf("not ok %d ", current); \
		if (_msg != NULL) printf(_msg, ##__VA_ARGS__); \
		printf("\n"); \
	} while(0)

#define is(got, expected, msg, ...) \
	ok(got == expected, msg, ##__VA_ARGS__)

#define isnt(got, expected, msg, ...) \
	ok(got != expected, msg, ##__VA_ARGS__)

#define isnull(got, msg, ...) \
	is(got, NULL, msg, ##__VA_ARGS__)

#define isntnull(got, msg, ...) \
	isnt(got, NULL, msg, ##__VA_ARGS__)

#define str_eq(got, expected, msg, ...) \
	do { \
		const char *_got = got, *_expected = expected, *_msg = msg; \
		if (_msg != NULL) { \
			ok(strcmp(_got, _expected) == 0, msg, ##__VA_ARGS__); \
		} else { \
			ok(strcmp(_got, _expected) == 0, \
				"string is \"%s\" (got: \"%s\")", \
				_expected, _got); \
		} \
	} while(0)

#define pass(msg, ...) \
	ok(1, msg, ##__VA_ARGS__)

#define fail(msg, ...) \
	ok(0, msg, ##__VA_ARGS__)

#define diag(msg, ...) \
	do { \
		printf("# "); \
		printf(msg, ##__VA_ARGS__); \
		printf("\n"); \
	} while(0)

#endif /* ! tap_h_included */
