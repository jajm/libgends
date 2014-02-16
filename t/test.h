#ifndef test_h_included
#define test_h_included

typedef struct {
	char *key;
	int value;
} test_t;

test_t *
test_new(
	const char *key,
	int value
);

test_t *
test_alloc(
	test_t *t
);

char *
test_getkey(
	test_t *t
);

int
test_setkey(
	test_t *t,
	const char *key
);

int
test_cmpkey(
	const char *key1,
	const char *key2
);

int
test_getvalue(
	test_t *t
);

int
test_setvalue(
	test_t *t,
	int value
);

void
test_free(
	test_t *t
);

#endif /* Not test_h_included */
