#include <stdio.h>
#include <stdlib.h>
#include <libexception/exception.h>
#include <libtap13/tap.h>
#include "hash_map_fast.h"
#include "hash_functions.h"

int test_cmpkey(const char *key1, const char *key2)
{
	return strcmp(key1, key2);
}

char * test_key_alloc(const char *key)
{
	char *k = malloc(strlen(key) + 1);
	return strcpy(k, key);
}

void t_hash_map_fast_new(void)
{
	gds_hash_map_fast_t *hash;

	ok(NULL == gds_hash_map_fast_new(0, NULL, NULL, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(0, NULL, NULL, NULL, NULL, free));
	ok(NULL == gds_hash_map_fast_new(0, NULL, NULL, NULL, free, NULL));
	ok(NULL == gds_hash_map_fast_new(0, NULL, NULL, NULL, free, free));
	ok(NULL == gds_hash_map_fast_new(0, NULL, NULL, test_key_alloc, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(0, NULL, NULL, test_key_alloc, NULL, free));
	ok(NULL == gds_hash_map_fast_new(0, NULL, NULL, test_key_alloc, free, NULL));
	ok(NULL == gds_hash_map_fast_new(0, NULL, NULL, test_key_alloc, free, free));
	ok(NULL == gds_hash_map_fast_new(0, NULL, test_cmpkey, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(0, NULL, test_cmpkey, NULL, NULL, free));
	ok(NULL == gds_hash_map_fast_new(0, NULL, test_cmpkey, NULL, free, NULL));
	ok(NULL == gds_hash_map_fast_new(0, NULL, test_cmpkey, NULL, free, free));
	ok(NULL == gds_hash_map_fast_new(0, NULL, test_cmpkey, test_key_alloc, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(0, NULL, test_cmpkey, test_key_alloc, NULL, free));
	ok(NULL == gds_hash_map_fast_new(0, NULL, test_cmpkey, test_key_alloc, free, NULL));
	ok(NULL == gds_hash_map_fast_new(0, NULL, test_cmpkey, test_key_alloc, free, free));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, NULL, NULL, free));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, NULL, free, NULL));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, NULL, free, free));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, test_key_alloc, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, test_key_alloc, NULL, free));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, test_key_alloc, free, NULL));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, NULL, test_key_alloc, free, free));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, NULL, NULL, free));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, NULL, free, NULL));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, NULL, free, free));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, test_key_alloc, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, test_key_alloc, NULL, free));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, NULL));
	ok(NULL == gds_hash_map_fast_new(0, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, free));
	ok(NULL == gds_hash_map_fast_new(32, NULL, NULL, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(32, NULL, NULL, NULL, NULL, free));
	ok(NULL == gds_hash_map_fast_new(32, NULL, NULL, NULL, free, NULL));
	ok(NULL == gds_hash_map_fast_new(32, NULL, NULL, NULL, free, free));
	ok(NULL == gds_hash_map_fast_new(32, NULL, NULL, test_key_alloc, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(32, NULL, NULL, test_key_alloc, NULL, free));
	ok(NULL == gds_hash_map_fast_new(32, NULL, NULL, test_key_alloc, free, NULL));
	ok(NULL == gds_hash_map_fast_new(32, NULL, NULL, test_key_alloc, free, free));
	ok(NULL == gds_hash_map_fast_new(32, NULL, test_cmpkey, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(32, NULL, test_cmpkey, NULL, NULL, free));
	ok(NULL == gds_hash_map_fast_new(32, NULL, test_cmpkey, NULL, free, NULL));
	ok(NULL == gds_hash_map_fast_new(32, NULL, test_cmpkey, NULL, free, free));
	ok(NULL == gds_hash_map_fast_new(32, NULL, test_cmpkey, test_key_alloc, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(32, NULL, test_cmpkey, test_key_alloc, NULL, free));
	ok(NULL == gds_hash_map_fast_new(32, NULL, test_cmpkey, test_key_alloc, free, NULL));
	ok(NULL == gds_hash_map_fast_new(32, NULL, test_cmpkey, test_key_alloc, free, free));
	ok(NULL == gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, NULL, NULL, free));
	ok(NULL == gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, NULL, free, NULL));
	ok(NULL == gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, NULL, free, free));
	ok(NULL == gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, test_key_alloc, NULL, NULL));
	ok(NULL == gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, test_key_alloc, NULL, free));
	ok(NULL == gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, test_key_alloc, free, NULL));
	ok(NULL == gds_hash_map_fast_new(32, gds_hash_sdbm, NULL, test_key_alloc, free, free));

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, NULL, NULL);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, NULL, free);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, free, NULL);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, NULL, free, free);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, NULL, NULL);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, NULL, free);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, NULL);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_fast_free(hash);

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, free);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_set(void)
{
	gds_hash_map_fast_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;
	int rc;

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, NULL);

	ok(0 > gds_hash_map_fast_set(NULL, NULL, NULL));
	ok(0 > gds_hash_map_fast_set(NULL, NULL, data[0]));
	ok(0 > gds_hash_map_fast_set(NULL, keys[0], NULL));
	ok(0 > gds_hash_map_fast_set(NULL, keys[0], data[0]));

	rc = gds_hash_map_fast_set(hash, keys[0], data[0]);
	is(rc, 0, NULL);
	rc = gds_hash_map_fast_set(hash, keys[0], data[0]);
	is(rc, 1, NULL);
	rc = gds_hash_map_fast_set(hash, keys[0], data[0]);
	is(rc, 1, NULL);

	s = gds_hash_map_fast_get(hash, keys[0]);
	isnt(s, NULL);
	str_eq(s, data[0]);

	gds_hash_map_fast_set(hash, keys[1], data[1]);
	gds_hash_map_fast_set(hash, keys[2], data[2]);

	s = gds_hash_map_fast_get(hash, keys[1]);
	isnt(s, NULL);
	str_eq(s, data[1]);

	s = gds_hash_map_fast_get(hash, keys[2]);
	isnt(s, NULL);
	str_eq(s, data[2]);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_get(void)
{
	gds_hash_map_fast_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, NULL);

	ok(NULL == gds_hash_map_fast_get(NULL, NULL));
	ok(NULL == gds_hash_map_fast_get(NULL, keys[0]));

	s = gds_hash_map_fast_get(hash, NULL);
	is(s, NULL);
	s = gds_hash_map_fast_get(hash, keys[0]);
	is(s, NULL);
	s = gds_hash_map_fast_get(hash, keys[1]);
	is(s, NULL);
	s = gds_hash_map_fast_get(hash, keys[2]);
	is(s, NULL);

	gds_hash_map_fast_set(hash, keys[0], data[0]);
	s = gds_hash_map_fast_get(hash, keys[0]);
	isnt(s, NULL);
	str_eq(s, data[0]);
	s = gds_hash_map_fast_get(hash, keys[1]);
	is(s, NULL);
	s = gds_hash_map_fast_get(hash, keys[2]);
	is(s, NULL);

	gds_hash_map_fast_set(hash, keys[1], data[1]);
	gds_hash_map_fast_set(hash, keys[2], data[2]);
	s = gds_hash_map_fast_get(hash, keys[0]);
	isnt(s, NULL);
	str_eq(s, data[0]);
	s = gds_hash_map_fast_get(hash, keys[1]);
	isnt(s, NULL);
	str_eq(s, data[1]);
	s = gds_hash_map_fast_get(hash, keys[2]);
	isnt(s, NULL);
	str_eq(s, data[2]);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_unset(void)
{
	gds_hash_map_fast_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * data[] = {"one", "two", "three"};
	int rc;

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, NULL);

	ok(0 > gds_hash_map_fast_unset(NULL, NULL));
	ok(0 > gds_hash_map_fast_unset(NULL, keys[0]));

	rc = gds_hash_map_fast_unset(hash, NULL);
	is(rc, 1);
	rc = gds_hash_map_fast_unset(hash, keys[0]);
	is(rc, 1);
	rc = gds_hash_map_fast_unset(hash, keys[1]);
	is(rc, 1);
	rc = gds_hash_map_fast_unset(hash, keys[2]);
	is(rc, 1);

	gds_hash_map_fast_set(hash, keys[0], data[0]);
	gds_hash_map_fast_set(hash, keys[1], data[1]);
	gds_hash_map_fast_set(hash, keys[2], data[2]);

	rc = gds_hash_map_fast_unset(hash, NULL);
	is(rc, 1);
	rc = gds_hash_map_fast_unset(hash, keys[0]);
	is(rc, 0);
	rc = gds_hash_map_fast_unset(hash, keys[1]);
	is(rc, 0);
	rc = gds_hash_map_fast_unset(hash, keys[2]);
	is(rc, 0);

	rc = gds_hash_map_fast_unset(hash, NULL);
	is(rc, 1);
	rc = gds_hash_map_fast_unset(hash, keys[0]);
	is(rc, 1);
	rc = gds_hash_map_fast_unset(hash, keys[1]);
	is(rc, 1);
	rc = gds_hash_map_fast_unset(hash, keys[2]);
	is(rc, 1);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_pop(void)
{
	gds_hash_map_fast_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * data[] = {"one", "two", "three"};
	void *d;

	hash = gds_hash_map_fast_new(32, gds_hash_djb2, test_cmpkey, test_key_alloc, free, NULL);

	ok(NULL == gds_hash_map_fast_pop(NULL, NULL));
	ok(NULL == gds_hash_map_fast_pop(NULL, keys[0]));

	d = gds_hash_map_fast_pop(hash, NULL);
	is(d, NULL);
	d = gds_hash_map_fast_pop(hash, keys[0]);
	is(d, NULL);
	d = gds_hash_map_fast_pop(hash, keys[1]);
	is(d, NULL);
	d = gds_hash_map_fast_pop(hash, keys[2]);
	is(d, NULL);

	gds_hash_map_fast_set(hash, keys[0], data[0]);
	gds_hash_map_fast_set(hash, keys[1], data[1]);
	gds_hash_map_fast_set(hash, keys[2], data[2]);

	d = gds_hash_map_fast_pop(hash, NULL);
	is(d, NULL);
	d = gds_hash_map_fast_pop(hash, keys[0]);
	is(d, data[0]);
	d = gds_hash_map_fast_pop(hash, keys[1]);
	is(d, data[1]);
	d = gds_hash_map_fast_pop(hash, keys[2]);
	is(d, data[2]);

	d = gds_hash_map_fast_pop(hash, NULL);
	is(d, NULL);
	d = gds_hash_map_fast_pop(hash, keys[0]);
	is(d, NULL);
	d = gds_hash_map_fast_pop(hash, keys[1]);
	is(d, NULL);
	d = gds_hash_map_fast_pop(hash, keys[2]);
	is(d, NULL);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_iterator(void)
{
	gds_hash_map_fast_t *hash;
	gds_iterator_t *it;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *key, *value;

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_fast_set(hash, keys[i], values[i]);
	}

	it = gds_hash_map_fast_iterator_new(hash);

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	str_eq(key, "1");
	str_eq(value, "one");

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	str_eq(key, "2");
	str_eq(value, "two");

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	str_eq(key, "3");
	str_eq(value, "three");

	ok(0 < gds_iterator_step(it));

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

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_fast_set(hash, keys[i], values[i]);
	}

	gds_slist_t *k = gds_hash_map_fast_keys(hash);
	is(gds_slist_size(k), 3);
	gds_slist_foreach (s, k) {
		if (!strcmp(s, keys[0])) check[0] = 1;
		else if (!strcmp(s, keys[1])) check[1] = 1;
		else if (!strcmp(s, keys[2])) check[2] = 1;
	}
	is(check[0], 1);
	is(check[1], 1);
	is(check[2], 1);

	gds_slist_free(k);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_values(void)
{
	gds_hash_map_fast_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *s;
	int check[] = {0, 0, 0};

	hash = gds_hash_map_fast_new(32, gds_hash_sdbm, test_cmpkey, test_key_alloc, free, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_fast_set(hash, keys[i], values[i]);
	}

	gds_slist_t *v = gds_hash_map_fast_values(hash);
	is(gds_slist_size(v), 3);
	gds_slist_foreach (s, v) {
		if (s == values[0]) check[0] = 1;
		else if (s == values[1]) check[1] = 1;
		else if (s == values[2]) check[2] = 1;
	}
	is(check[0], 1);
	is(check[1], 1);
	is(check[2], 1);

	gds_slist_free(v);

	gds_hash_map_fast_free(hash);
}

void t_hash_map_fast_change_size(void)
{
	gds_hash_map_fast_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};

	hash = gds_hash_map_fast_new(32, gds_hash_djb2, test_cmpkey, test_key_alloc, free, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_fast_set(hash, keys[i], values[i]);
	}

	gds_hash_map_fast_change_size(hash, 2);
	is(gds_hash_map_fast_get(hash, keys[0]), values[0]);
	is(gds_hash_map_fast_get(hash, keys[1]), values[1]);
	is(gds_hash_map_fast_get(hash, keys[2]), values[2]);

	gds_hash_map_fast_change_size(hash, 64);
	is(gds_hash_map_fast_get(hash, keys[0]), values[0]);
	is(gds_hash_map_fast_get(hash, keys[1]), values[1]);
	is(gds_hash_map_fast_get(hash, keys[2]), values[2]);

	gds_hash_map_fast_free(hash);
}

int main()
{
	plan(142);

	t_hash_map_fast_new();
	t_hash_map_fast_set();
	t_hash_map_fast_get();
	t_hash_map_fast_unset();
	t_hash_map_fast_pop();
	t_hash_map_fast_iterator();
	t_hash_map_fast_keys();
	t_hash_map_fast_values();
	t_hash_map_fast_change_size();

	return EXIT_SUCCESS;
}
