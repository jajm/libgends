#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <libexception/exception.h>
#include "hash_map_fast.h"
#include "hash_functions.h"
#include "tap.h"
#include "test_macros.h"

int test_cmpkey(const char *key1, const char *key2)
{
	return strcmp(key1, key2);
}

void t_hash_map_fast_new(void)
{
	gds_hash_map_fast_t *hash;

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, NULL, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, NULL, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, test_cmpkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, test_cmpkey, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, test_cmpkey, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, NULL, test_cmpkey, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, NULL, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, NULL, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, test_cmpkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, test_cmpkey, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, test_cmpkey, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, NULL, test_cmpkey, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, free, free));

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, NULL);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, free);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, free, NULL);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, free, free);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_set(void)
{
	gds_hash_map_fast_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;
	int rc;

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, NULL);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, NULL, data[0]));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, keys[0], NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_fast_set(NULL, keys[0], data[0]));

	rc = gds_hash_map_fast_set(hash, keys[0], data[0]);
	is(rc, 0, NULL);
	rc = gds_hash_map_fast_set(hash, keys[0], data[0]);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_set(hash, keys[0], data[0]);
	is(rc, 1, NULL);

	s = gds_hash_map_fast_get(hash, keys[0]);
	isntnull(s, NULL);
	str_eq(s, data[0], NULL);

	gds_hash_map_fast_set(hash, keys[1], data[1]);
	gds_hash_map_fast_set(hash, keys[2], data[2]);

	s = gds_hash_map_fast_get(hash, keys[1]);
	isntnull(s, NULL);
	str_eq(s, data[1], NULL);

	s = gds_hash_map_fast_get(hash, keys[2]);
	isntnull(s, NULL);
	str_eq(s, data[2], NULL);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_get(void)
{
	gds_hash_map_fast_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, NULL);

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

	gds_hash_map_fast_set(hash, keys[0], data[0]);
	s = gds_hash_map_fast_get(hash, keys[0]);
	isntnull(s, NULL);
	str_eq(s, data[0], NULL);
	s = gds_hash_map_fast_get(hash, keys[1]);
	isnull(s, NULL);
	s = gds_hash_map_fast_get(hash, keys[2]);
	isnull(s, NULL);

	gds_hash_map_fast_set(hash, keys[1], data[1]);
	gds_hash_map_fast_set(hash, keys[2], data[2]);
	s = gds_hash_map_fast_get(hash, keys[0]);
	isntnull(s, NULL);
	str_eq(s, data[0], NULL);
	s = gds_hash_map_fast_get(hash, keys[1]);
	isntnull(s, NULL);
	str_eq(s, data[1], NULL);
	s = gds_hash_map_fast_get(hash, keys[2]);
	isntnull(s, NULL);
	str_eq(s, data[2], NULL);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_unset(void)
{
	gds_hash_map_fast_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * data[] = {"one", "two", "three"};
	int rc;

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, keys[0]));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, keys[0]));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, keys[0]));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_fast_unset(NULL, keys[0]));

	rc = gds_hash_map_fast_unset(hash, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[0]);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[1]);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[2]);
	is(rc, 1, NULL);

	gds_hash_map_fast_set(hash, keys[0], data[0]);
	gds_hash_map_fast_set(hash, keys[1], data[1]);
	gds_hash_map_fast_set(hash, keys[2], data[2]);

	rc = gds_hash_map_fast_unset(hash, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[0]);
	is(rc, 0, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[1]);
	is(rc, 0, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[2]);
	is(rc, 0, NULL);

	rc = gds_hash_map_fast_unset(hash, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[0]);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[1]);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_unset(hash, keys[2]);
	is(rc, 1, NULL);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_iterator(void)
{
	gds_hash_map_fast_t *hash;
	gds_iterator_t *it;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *key, *value;

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_fast_set(hash, keys[i], values[i]);
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
	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_keys(void)
{
	gds_hash_map_fast_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *s;
	int check[] = {0, 0, 0};

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_fast_set(hash, keys[i], values[i]);
	}

	gds_slist_t *k = gds_hash_map_fast_keys(hash);
	is(gds_slist_size(k), 3, NULL);
	gds_slist_foreach (s, k) {
		if (s == keys[0]) check[0] = 1;
		else if (s == keys[1]) check[1] = 1;
		else if (s == keys[2]) check[2] = 1;
	}
	is(check[0], 1, NULL);
	is(check[1], 1, NULL);
	is(check[2], 1, NULL);

	gds_slist_free(k, NULL, NULL);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_values(void)
{
	gds_hash_map_fast_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *s;
	int check[] = {0, 0, 0};

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_fast_set(hash, keys[i], values[i]);
	}

	gds_slist_t *v = gds_hash_map_fast_values(hash);
	is(gds_slist_size(v), 3, NULL);
	gds_slist_foreach (s, v) {
		if (s == values[0]) check[0] = 1;
		else if (s == values[1]) check[1] = 1;
		else if (s == values[2]) check[2] = 1;
	}
	is(check[0], 1, NULL);
	is(check[1], 1, NULL);
	is(check[2], 1, NULL);

	gds_slist_free(v, NULL, NULL);

	gds_hash_map_fast_free(hash);
}

int main()
{
	plan(96);

	t_hash_map_fast_new();
	t_hash_map_fast_set();
	t_hash_map_fast_get();
	t_hash_map_fast_unset();
	t_hash_map_fast_iterator();
	t_hash_map_fast_keys();
	t_hash_map_fast_values();

	return EXIT_SUCCESS;
}
