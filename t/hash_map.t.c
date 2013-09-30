#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <libexception/exception.h>
#include "hash_map.h"
#include "tap.h"
#include "test_macros.h"

int init_suite(void)
{
	return 0;
}

int clean_suite(void)
{
	return 0;
}

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
		gds_hash_map_new(0, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, NULL, (gds_cmpkey_cb) test_cmpkey));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, (gds_hash_cb) test_hash, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(0, (gds_hash_cb) test_hash, (gds_cmpkey_cb) test_cmpkey));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, NULL, (gds_cmpkey_cb) test_cmpkey));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_hash_map_new(32, (gds_hash_cb) test_hash, NULL));

	hash = gds_hash_map_new(32, (gds_hash_cb) test_hash, (gds_cmpkey_cb) test_cmpkey);
	isntnull(hash, "hash creation succeeded");

	gds_hash_map_free(hash, NULL, NULL);
}

void t_hash_map_iterator(void)
{
	gds_hash_map_t *hash;
	gds_iterator_t *it;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *key, *value;
	int rc;

	hash = gds_hash_map_new(32, (gds_hash_cb) test_hash,
		(gds_cmpkey_cb) test_cmpkey);
	for (int i = 0; i < 3; i++) {
		rc = gds_hash_map_set(hash, keys[i], values[i], NULL, NULL);
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
	gds_hash_map_free(hash, NULL, NULL);
}

int main()
{
	plan(21);

	t_hash_map_new();
	t_hash_map_iterator();

	return EXIT_SUCCESS;
}
