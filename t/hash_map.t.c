#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <libexception/exception.h>
#include "hash_map.h"
#include "slist.h"
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

void t_hash_map_new(void)
{
	gds_hash_map_t *hash;

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, NULL, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, NULL, NULL, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, NULL, NULL, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, NULL, test_cmpkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, NULL, test_cmpkey, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, NULL, test_cmpkey, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, NULL, test_cmpkey, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, test_hash, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, test_hash, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, test_hash, NULL, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, test_hash, NULL, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, test_hash, test_cmpkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, test_hash, test_cmpkey, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, test_hash, test_cmpkey, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, test_hash, test_cmpkey, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, NULL, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, NULL, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, test_cmpkey, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, test_cmpkey, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, test_cmpkey, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, test_cmpkey, free, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, test_hash, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, test_hash, NULL, NULL, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, test_hash, NULL, free, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, test_hash, NULL, free, free));

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, NULL, NULL);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_free(hash);

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, NULL, free);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_free(hash);

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, free, NULL);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_free(hash);

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, free, free);
	isntnull(hash, "hash creation succeeded");
	gds_hash_map_free(hash);
}

void t_hash_map_set(void)
{
	gds_hash_map_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;
	int rc;

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, NULL, NULL);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_set(NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_set(NULL, NULL, data[0]));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_set(NULL, keys[0], NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_set(NULL, keys[0], data[0]));

	rc = gds_hash_map_set(hash, keys[0], data[0]);
	is(rc, 0, NULL);
	rc = gds_hash_map_set(hash, keys[0], data[0]);
	is(rc, 1, NULL);
	rc = gds_hash_map_set(hash, keys[0], data[0]);
	is(rc, 1, NULL);

	s = gds_hash_map_get(hash, keys[0]);
	isntnull(s, NULL);
	str_eq(s, data[0], NULL);

	gds_hash_map_set(hash, keys[1], data[1]);
	gds_hash_map_set(hash, keys[2], data[2]);

	s = gds_hash_map_get(hash, keys[1]);
	isntnull(s, NULL);
	str_eq(s, data[1], NULL);

	s = gds_hash_map_get(hash, keys[2]);
	isntnull(s, NULL);
	str_eq(s, data[2], NULL);

	gds_hash_map_free(hash);
}

void t_hash_map_get(void)
{
	gds_hash_map_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, NULL, NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_get(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_get(NULL, keys[0]));

	s = gds_hash_map_get(hash, NULL);
	isnull(s, NULL);
	s = gds_hash_map_get(hash, keys[0]);
	isnull(s, NULL);
	s = gds_hash_map_get(hash, keys[1]);
	isnull(s, NULL);
	s = gds_hash_map_get(hash, keys[2]);
	isnull(s, NULL);

	gds_hash_map_set(hash, keys[0], data[0]);
	s = gds_hash_map_get(hash, keys[0]);
	isntnull(s, NULL);
	str_eq(s, data[0], NULL);
	s = gds_hash_map_get(hash, keys[1]);
	isnull(s, NULL);
	s = gds_hash_map_get(hash, keys[2]);
	isnull(s, NULL);

	gds_hash_map_set(hash, keys[1], data[1]);
	gds_hash_map_set(hash, keys[2], data[2]);
	s = gds_hash_map_get(hash, keys[0]);
	isntnull(s, NULL);
	str_eq(s, data[0], NULL);
	s = gds_hash_map_get(hash, keys[1]);
	isntnull(s, NULL);
	str_eq(s, data[1], NULL);
	s = gds_hash_map_get(hash, keys[2]);
	isntnull(s, NULL);
	str_eq(s, data[2], NULL);

	gds_hash_map_free(hash);
}

void t_hash_map_unset(void)
{
	gds_hash_map_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * data[] = {"one", "two", "three"};
	int rc;

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, NULL, NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_unset(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_hash_map_unset(NULL, keys[0]));

	rc = gds_hash_map_unset(hash, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_unset(hash, keys[0]);
	is(rc, 1, NULL);
	rc = gds_hash_map_unset(hash, keys[1]);
	is(rc, 1, NULL);
	rc = gds_hash_map_unset(hash, keys[2]);
	is(rc, 1, NULL);

	gds_hash_map_set(hash, keys[0], data[0]);
	gds_hash_map_set(hash, keys[1], data[1]);
	gds_hash_map_set(hash, keys[2], data[2]);

	rc = gds_hash_map_unset(hash, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_unset(hash, keys[0]);
	is(rc, 0, NULL);
	rc = gds_hash_map_unset(hash, keys[1]);
	is(rc, 0, NULL);
	rc = gds_hash_map_unset(hash, keys[2]);
	is(rc, 0, NULL);

	rc = gds_hash_map_unset(hash, NULL);
	is(rc, 1, NULL);
	rc = gds_hash_map_unset(hash, keys[0]);
	is(rc, 1, NULL);
	rc = gds_hash_map_unset(hash, keys[1]);
	is(rc, 1, NULL);
	rc = gds_hash_map_unset(hash, keys[2]);
	is(rc, 1, NULL);

	gds_hash_map_free(hash);
}

void t_hash_map_iterator(void)
{
	gds_hash_map_t *hash;
	gds_iterator_t *it;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *key, *value;
	int rc;

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		rc = gds_hash_map_set(hash, keys[i], values[i]);
		is(rc, 0, NULL);
	}

	it = gds_hash_map_iterator_new(hash);
	isntnull(it, NULL);

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	isntnull(key, NULL);
	isntnull(value, NULL);
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
	gds_hash_map_free(hash);
}

void t_hash_map_keys(void)
{
	gds_hash_map_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_set(hash, keys[i], values[i]);
	}

	gds_slist_t *k = gds_hash_map_keys(hash);
	is(gds_slist_size(k), 3, NULL);
	is(gds_slist_get(k, 0), keys[0], NULL);
	is(gds_slist_get(k, 1), keys[1], NULL);
	is(gds_slist_get(k, 2), keys[2], NULL);
	gds_slist_free(k, NULL, NULL);

	gds_hash_map_free(hash);
}

void t_hash_map_values(void)
{
	gds_hash_map_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};

	hash = gds_hash_map_new(32, test_hash, test_cmpkey, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_set(hash, keys[i], values[i]);
	}

	gds_slist_t *v = gds_hash_map_values(hash);
	is(gds_slist_size(v), 3, NULL);
	is(gds_slist_get(v, 0), values[0], NULL);
	is(gds_slist_get(v, 1), values[1], NULL);
	is(gds_slist_get(v, 2), values[2], NULL);
	gds_slist_free(v, NULL, NULL);

	gds_hash_map_free(hash);
}

int main()
{
	plan(96);

	t_hash_map_new();
	t_hash_map_set();
	t_hash_map_get();
	t_hash_map_unset();
	t_hash_map_iterator();
	t_hash_map_keys();
	t_hash_map_values();

	return EXIT_SUCCESS;
}
