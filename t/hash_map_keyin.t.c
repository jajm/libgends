#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <libexception/exception.h>
#include "hash_map_keyin.h"
#include "slist.h"
#include "tap.h"
#include "test_macros.h"
#include "test.h"

int test_hash(const char *key)
{
	if (key) return key[0];
	return 0;
}

void t_hash_map_keyin_new(void)
{
	gds_hash_map_keyin_t *hash;
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb) test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb) test_getkey;
	gds_hash_cb hash_cb = (gds_hash_cb) test_hash;

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0 , NULL, NULL  , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0 , NULL, NULL  , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0 , NULL, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0 , NULL, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0 , hash_cb, NULL  , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0 , hash_cb, NULL  , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0 , hash_cb, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(0 , hash_cb, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, NULL  , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, NULL  , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, NULL, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, hash_cb, NULL  , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, hash_cb, NULL  , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_new(32, hash_cb, getkey_cb, NULL));

	hash = gds_hash_map_keyin_new(32, hash_cb, getkey_cb, cmpkey_cb);
	isntnull(hash, "hash creation succeeded");

	gds_hash_map_keyin_free(hash, NULL);
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

	hash = gds_hash_map_keyin_new(32, (gds_hash_cb) test_hash,
		(gds_getkey_cb) test_getkey, (gds_cmpkey_cb) test_cmpkey);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_set(NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_set(NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_set(NULL, &data[0], NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_keyin_set(NULL, &data[0], free));

	rc = gds_hash_map_keyin_set(hash, &data[0], NULL);
	is(rc, 0, NULL);
	rc = gds_hash_map_keyin_set(hash, &data[0], free);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_set(hash, &data[0], free);
	is(rc, 1, NULL);

	t = gds_hash_map_keyin_get(hash, data[0].key);
	isntnull(t, NULL);
	is(t, &data[0], NULL);

	gds_hash_map_keyin_set(hash, &data[1], NULL);
	gds_hash_map_keyin_set(hash, &data[2], NULL);

	t = gds_hash_map_keyin_get(hash, data[1].key);
	isntnull(t, NULL);
	is(t, &data[1], NULL);

	t = gds_hash_map_keyin_get(hash, data[2].key);
	isntnull(t, NULL);
	is(t, &data[2], NULL);

	gds_hash_map_keyin_free(hash, NULL);
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

	hash = gds_hash_map_keyin_new(32, (gds_hash_cb) test_hash,
		(gds_getkey_cb) test_getkey, (gds_cmpkey_cb) test_cmpkey);

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

	gds_hash_map_keyin_set(hash, &data[0], NULL);
	t = gds_hash_map_keyin_get(hash, data[0].key);
	isntnull(t, NULL);
	is(t, &data[0], NULL);
	t = gds_hash_map_keyin_get(hash, data[1].key);
	isnull(t, NULL);
	t = gds_hash_map_keyin_get(hash, data[2].key);
	isnull(t, NULL);

	gds_hash_map_keyin_set(hash, &data[1], NULL);
	gds_hash_map_keyin_set(hash, &data[2], NULL);
	t = gds_hash_map_keyin_get(hash, data[0].key);
	isntnull(t, NULL);
	is(t, &data[0], NULL);
	t = gds_hash_map_keyin_get(hash, data[1].key);
	isntnull(t, NULL);
	is(t, &data[1], NULL);
	t = gds_hash_map_keyin_get(hash, data[2].key);
	isntnull(t, NULL);
	is(t, &data[2], NULL);

	gds_hash_map_keyin_free(hash, NULL);
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

	hash = gds_hash_map_keyin_new(32, (gds_hash_cb) test_hash,
		(gds_getkey_cb) test_getkey, (gds_cmpkey_cb) test_cmpkey);

	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_keyin_unset(NULL, NULL       , NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_keyin_unset(NULL, NULL       , free));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_keyin_unset(NULL, data[0].key, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_keyin_unset(NULL, data[0].key, free));

	rc = gds_hash_map_keyin_unset(hash, NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[0].key, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[1].key, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[2].key, NULL);
	is(rc, 1, NULL);

	gds_hash_map_keyin_set(hash, &data[0], NULL);
	gds_hash_map_keyin_set(hash, &data[1], NULL);
	gds_hash_map_keyin_set(hash, &data[2], NULL);

	rc = gds_hash_map_keyin_unset(hash, NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[0].key, NULL);
	is(rc, 0, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[1].key, NULL);
	is(rc, 0, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[2].key, NULL);
	is(rc, 0, NULL);

	rc = gds_hash_map_keyin_unset(hash, NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[0].key, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[1].key, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_keyin_unset(hash, data[2].key, NULL);
	is(rc, 1, NULL);

	gds_hash_map_keyin_free(hash, NULL);
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

	hash = gds_hash_map_keyin_new(32, (gds_hash_cb) test_hash,
		(gds_getkey_cb) test_getkey, (gds_cmpkey_cb) test_cmpkey);
	for (int i = 0; i < 3; i++) {
		rc = gds_hash_map_keyin_set(hash, &data[i], NULL);
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
	gds_hash_map_keyin_free(hash, NULL);
}

void t_hash_map_keyin_values(void)
{
	gds_hash_map_keyin_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};

	hash = gds_hash_map_keyin_new(32, (gds_hash_cb) test_hash,
		(gds_getkey_cb) test_getkey, (gds_cmpkey_cb) test_cmpkey);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_keyin_set(hash, &data[i], NULL);
	}

	gds_slist_t *v = gds_hash_map_keyin_values(hash);
	is(gds_slist_size(v), 3, NULL);
	is(gds_slist_get(v, 0), &data[0], NULL);
	is(gds_slist_get(v, 1), &data[2], NULL);
	is(gds_slist_get(v, 2), &data[1], NULL);
	gds_slist_free(v, NULL, NULL);

	gds_hash_map_keyin_free(hash, NULL);
}

int main()
{
	plan(79);

	t_hash_map_keyin_new();
	t_hash_map_keyin_set();
	t_hash_map_keyin_get();
	t_hash_map_keyin_unset();
	t_hash_map_keyin_iterator();
	t_hash_map_keyin_values();

	return EXIT_SUCCESS;
}
