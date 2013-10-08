#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <libexception/exception.h>
#include "hash_map_fast.h"
#include "tap.h"
#include "test_macros.h"

int test_hash(const char *key)
{
	if (key) return key[0];
	return 0;
}

int test_cmpkey(const char *key1, const char *key2)
{
	return strcmp(key1, key2);
}

void t_hash_map_fast_new(void)
{
	gds_hash_map_fast_t *hash;

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, (gds_cmpkey_cb) test_cmpkey));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, (gds_hash_cb) test_hash, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, (gds_hash_cb) test_hash, (gds_cmpkey_cb) test_cmpkey));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, (gds_cmpkey_cb) test_cmpkey));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, (gds_hash_cb) test_hash, NULL));

	hash = gds_hash_map_fast_new(32, (gds_hash_cb) test_hash, (gds_cmpkey_cb) test_cmpkey);
	isntnull(hash, "hash creation succeeded");

	gds_hash_map_fast_free(hash, NULL, NULL);
}

void t_hash_map_fast_set(void)
{
	gds_hash_map_fast_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;
	int rc;

	hash = gds_hash_map_fast_new(32, (gds_hash_cb) test_hash, (gds_cmpkey_cb) test_cmpkey);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, NULL, data[0], NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, NULL, data[0], free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, keys[0], NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, keys[0], NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, keys[0], data[0], NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, keys[0], data[0], free));

	rc = gds_hash_map_fast_set(hash, keys[0], data[0], NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_set(hash, keys[0], data[0], free);
	is(rc, 0, NULL);
	rc = gds_hash_map_fast_set(hash, keys[0], data[0], free);
	is(rc, 0, NULL);

	s = gds_hash_map_fast_get(hash, keys[0]);
	isntnull(s, NULL);
	str_eq(s, data[0], NULL);

	gds_hash_map_fast_set(hash, keys[1], data[1], NULL);
	gds_hash_map_fast_set(hash, keys[2], data[2], NULL);

	s = gds_hash_map_fast_get(hash, keys[1]);
	isntnull(s, NULL);
	str_eq(s, data[1], NULL);

	s = gds_hash_map_fast_get(hash, keys[2]);
	isntnull(s, NULL);
	str_eq(s, data[2], NULL);

	gds_hash_map_fast_free(hash, NULL, NULL);
}

void t_hash_map_fast_get(void)
{
	gds_hash_map_fast_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;

	hash = gds_hash_map_fast_new(32, (gds_hash_cb) test_hash, (gds_cmpkey_cb) test_cmpkey);

	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_get(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_get(NULL, keys[0]));

	s = gds_hash_map_fast_get(hash, NULL);
	isnull(s, NULL);
	s = gds_hash_map_fast_get(hash, keys[0]);
	isnull(s, NULL);
	s = gds_hash_map_fast_get(hash, keys[1]);
	isnull(s, NULL);
	s = gds_hash_map_fast_get(hash, keys[2]);
	isnull(s, NULL);

	gds_hash_map_fast_set(hash, keys[0], data[0], NULL);
	s = gds_hash_map_fast_get(hash, keys[0]);
	isntnull(s, NULL);
	str_eq(s, data[0], NULL);
	s = gds_hash_map_fast_get(hash, keys[1]);
	isnull(s, NULL);
	s = gds_hash_map_fast_get(hash, keys[2]);
	isnull(s, NULL);

	gds_hash_map_fast_set(hash, keys[1], data[1], NULL);
	gds_hash_map_fast_set(hash, keys[2], data[2], NULL);
	s = gds_hash_map_fast_get(hash, keys[0]);
	isntnull(s, NULL);
	str_eq(s, data[0], NULL);
	s = gds_hash_map_fast_get(hash, keys[1]);
	isntnull(s, NULL);
	str_eq(s, data[1], NULL);
	s = gds_hash_map_fast_get(hash, keys[2]);
	isntnull(s, NULL);
	str_eq(s, data[2], NULL);

	gds_hash_map_fast_free(hash, NULL, NULL);
}

void t_hash_map_fast_unset(void)
{
	gds_hash_map_fast_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * data[] = {"one", "two", "three"};
	int rc;

	hash = gds_hash_map_fast_new(32, (gds_hash_cb) test_hash,
		(gds_cmpkey_cb) test_cmpkey);

	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, NULL, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, NULL, free, free));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, keys[0], NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, keys[0], NULL, free));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, keys[0], free, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, keys[0], free, free));

	rc = gds_hash_map_fast_unset(hash, NULL, NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[0], NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[1], NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[2], NULL, NULL);
	is(rc, 1, NULL);

	gds_hash_map_fast_set(hash, keys[0], data[0], NULL);
	gds_hash_map_fast_set(hash, keys[1], data[1], NULL);
	gds_hash_map_fast_set(hash, keys[2], data[2], NULL);

	rc = gds_hash_map_fast_unset(hash, NULL, NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[0], NULL, NULL);
	is(rc, 0, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[1], NULL, NULL);
	is(rc, 0, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[2], NULL, NULL);
	is(rc, 0, NULL);

	rc = gds_hash_map_fast_unset(hash, NULL, NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[0], NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[1], NULL, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[2], NULL, NULL);
	is(rc, 1, NULL);

	gds_hash_map_fast_free(hash, NULL, NULL);
}

void t_hash_map_fast_iterator(void)
{
	gds_hash_map_fast_t *hash;
	gds_iterator_t *it;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *key, *value;

	hash = gds_hash_map_fast_new(32, (gds_hash_cb) test_hash,
		(gds_cmpkey_cb) test_cmpkey);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_fast_set(hash, keys[i], values[i], NULL);
	}

	it = gds_hash_map_fast_iterator_new(hash);

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	str_eq(key, "1", NULL);
	str_eq(value, "one", NULL);

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	str_eq(key, "2", NULL);
	str_eq(value, "two", NULL);

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	str_eq(key, "3", NULL);
	str_eq(value, "three", NULL);

	ok(0 < gds_iterator_step(it), NULL);

	gds_iterator_free(it);
	gds_hash_map_fast_free(hash, NULL, NULL);
}

int main()
{
	plan(68);

	t_hash_map_fast_new();
	t_hash_map_fast_set();
	t_hash_map_fast_get();
	t_hash_map_fast_unset();
	t_hash_map_fast_iterator();

	return EXIT_SUCCESS;
}
