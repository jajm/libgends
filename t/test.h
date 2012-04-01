#ifndef test_h_included
#define test_h_included

#include <stdint.h>

typedef struct {
	char *key;
	int32_t value;
} test_t;

test_t *
test_new(
	const char *key,
	int32_t value
);

test_t *
test_alloc(
	test_t *t
);

char *
test_getkey(
	test_t *t
);

int8_t
test_setkey(
	test_t *t,
	const char *key
);

int32_t
test_cmpkey(
	const char *key1,
	const char *key2
);

int32_t
test_getvalue(
	test_t *t
);

int8_t
test_setvalue(
	test_t *t,
	int32_t value
);

void
test_free(
	test_t *t
);

#endif /* Not test_h_included */
