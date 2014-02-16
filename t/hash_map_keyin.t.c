#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <libexception/exception.h>
#include "hash_map_keyin.h"
#include "hash_functions.h"
#include "slist.h"
#include "tap.h"
#include "test_macros.h"
#include "test.h"

void t_hash_map_keyin_new(void)
{
	gds_hash_map_keyin_t *hash;

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, NULL, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, NULL, NULL, test_cmpkey, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, NULL, NULL, test_cmpkey, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, NULL, test_getkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, NULL, test_getkey, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, NULL, test_getkey, test_cmpkey, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, NULL, test_getkey, test_cmpkey, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, gds_hash_djb2, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, gds_hash_djb2, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, gds_hash_djb2, NULL, test_cmpkey, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, gds_hash_djb2, NULL, test_cmpkey, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, gds_hash_djb2, test_getkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, gds_hash_djb2, test_getkey, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, gds_hash_djb2, test_getkey, test_cmpkey, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0, gds_hash_djb2, test_getkey, test_cmpkey, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, NULL, test_cmpkey, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, NULL, test_cmpkey, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, test_getkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, test_getkey, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, test_getkey, test_cmpkey, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, test_getkey, test_cmpkey, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, gds_hash_djb2, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, gds_hash_djb2, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, gds_hash_djb2, NULL, test_cmpkey, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, gds_hash_djb2, NULL, test_cmpkey, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, gds_hash_djb2, test_getkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, gds_hash_djb2, test_getkey, NULL, free));

	hash = gds_hash_map_keyin_new(32, gds_hash_djb2, test_getkey, test_cmpkey, NULL);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_keyin_free(hash);

	hash = gds_hash_map_keyin_new(32, gds_hash_djb2, test_getkey, test_cmpkey, free);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_keyin_free(hash);
}

void t_hash_map_keyin_set(void)
{
	gds_hash_map_keyin_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	test_t *t;
	int rc;

	hash = gds_hash_map_keyin_new(32, gds_hash_djb2, test_getkey, test_cmpkey, NULL);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_set(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_set(NULL, &data[0]));

	rc = gds_hash_map_keyin_set(hash, &data[0]);
	is(rc, 0, NULL);
	rc = gds_hash_map_keyin_set(hash, &data[0]);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_set(hash, &data[0]);
	is(rc, 1, NULL);

	t = gds_hash_map_keyin_get(hash, data[0].key);
	isntnull(t, NULL);
	is(t, &data[0], NULL);

	gds_hash_map_keyin_set(hash, &data[1]);
	gds_hash_map_keyin_set(hash, &data[2]);

	t = gds_hash_map_keyin_get(hash, data[1].key);
	isntnull(t, NULL);
	is(t, &data[1], NULL);

	t = gds_hash_map_keyin_get(hash, data[2].key);
	isntnull(t, NULL);
	is(t, &data[2], NULL);

	gds_hash_map_keyin_free(hash);
}

void t_hash_map_keyin_get(void)
{
	gds_hash_map_keyin_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	test_t *t;

	hash = gds_hash_map_keyin_new(32, gds_hash_djb2, test_getkey, test_cmpkey, NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_keyin_get(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_keyin_get(NULL, data[0].key));

	t = gds_hash_map_keyin_get(hash, NULL);
	isnull(t, NULL);
	t = gds_hash_map_keyin_get(hash, data[0].key);
	isnull(t, NULL);
	t = gds_hash_map_keyin_get(hash, data[1].key);
	isnull(t, NULL);
	t = gds_hash_map_keyin_get(hash, data[2].key);
	isnull(t, NULL);

	gds_hash_map_keyin_set(hash, &data[0]);
	t = gds_hash_map_keyin_get(hash, data[0].key);
	isntnull(t, NULL);
	is(t, &data[0], NULL);
	t = gds_hash_map_keyin_get(hash, data[1].key);
	isnull(t, NULL);
	t = gds_hash_map_keyin_get(hash, data[2].key);
	isnull(t, NULL);

	gds_hash_map_keyin_set(hash, &data[1]);
	gds_hash_map_keyin_set(hash, &data[2]);
	t = gds_hash_map_keyin_get(hash, data[0].key);
	isntnull(t, NULL);
	is(t, &data[0], NULL);
	t = gds_hash_map_keyin_get(hash, data[1].key);
	isntnull(t, NULL);
	is(t, &data[1], NULL);
	t = gds_hash_map_keyin_get(hash, data[2].key);
	isntnull(t, NULL);
	is(t, &data[2], NULL);

	gds_hash_map_keyin_free(hash);
}

void t_hash_map_keyin_unset(void)
{
	gds_hash_map_keyin_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	int rc;

	hash = gds_hash_map_keyin_new(32, gds_hash_djb2, test_getkey, test_cmpkey, NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_keyin_unset(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_keyin_unset(NULL, data[0].key));

	rc = gds_hash_map_keyin_unset(hash, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[0].key);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[1].key);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[2].key);
	is(rc, 1, NULL);

	gds_hash_map_keyin_set(hash, &data[0]);
	gds_hash_map_keyin_set(hash, &data[1]);
	gds_hash_map_keyin_set(hash, &data[2]);

	rc = gds_hash_map_keyin_unset(hash, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[0].key);
	is(rc, 0, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[1].key);
	is(rc, 0, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[2].key);
	is(rc, 0, NULL);

	rc = gds_hash_map_keyin_unset(hash, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[0].key);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[1].key);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[2].key);
	is(rc, 1, NULL);

	gds_hash_map_keyin_free(hash);
}

void t_hash_map_keyin_iterator(void)
{
	gds_hash_map_keyin_t *hash;
	gds_iterator_t *it;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	char *key;
	test_t *value;
	int rc;

	hash = gds_hash_map_keyin_new(32, gds_hash_djb2, test_getkey, test_cmpkey, NULL);
	for (int i = 0; i < 3; i++) {
		rc = gds_hash_map_keyin_set(hash, &data[i]);
		is(rc, 0, NULL);
	}

	it = gds_hash_map_keyin_iterator_new(hash);
	isntnull(it, NULL);

	while (!gds_iterator_step(it)) {
		key = gds_iterator_getkey(it);
		value = gds_iterator_get(it);
		isntnull(key, NULL);
		isntnull(value, NULL);

		if (value == &data[0]) {
			str_eq(key, data[0].key, NULL);
		} else if (value == &data[1]) {
			str_eq(key, data[1].key, NULL);
		} else if (value == &data[2]) {
			str_eq(key, data[2].key, NULL);
		} else {
			fail("Unknown data in hash");
		}
	}

	ok(0 < gds_iterator_step(it), NULL);

	gds_iterator_free(it);
	gds_hash_map_keyin_free(hash);
}

void t_hash_map_keyin_values(void)
{
	gds_hash_map_keyin_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	test_t *t;
	int check[] = {0, 0, 0};

	hash = gds_hash_map_keyin_new(32, gds_hash_djb2, test_getkey, test_cmpkey, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_keyin_set(hash, &data[i]);
	}

	gds_slist_t *v = gds_hash_map_keyin_values(hash);
	is(gds_slist_size(v), 3, NULL);
	gds_slist_foreach (t, v) {
		if (t == &data[0]) check[0] = 1;
		else if (t == &data[1]) check[1] = 1;
		else if (t == &data[2]) check[2] = 1;
	}
	is(check[0], 1, NULL);
	is(check[1], 1, NULL);
	is(check[2], 1, NULL);

	gds_slist_free(v, NULL, NULL);

	gds_hash_map_keyin_free(hash);
}

int main()
{
	plan(91);

	t_hash_map_keyin_new();
	t_hash_map_keyin_set();
	t_hash_map_keyin_get();
	t_hash_map_keyin_unset();
	t_hash_map_keyin_iterator();
	t_hash_map_keyin_values();

	return EXIT_SUCCESS;
}
