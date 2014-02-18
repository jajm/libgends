#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libexception/exception.h>
#include <libtap13/tap.h>
#include "hash_map.h"
#include "hash_functions.h"
#include "slist.h"

int test_cmpkey(const char *key1, const char *key2)
{
	return strcmp(key1, key2);
}

void t_hash_map_new(void)
{
	gds_hash_map_t *hash;

	ok(NULL == gds_hash_map_new(0, NULL, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_new(0, NULL, NULL, NULL, free));
	ok(NULL == gds_hash_map_new(0, NULL, NULL, free, NULL));
	ok(NULL == gds_hash_map_new(0, NULL, NULL, free, free));
	ok(NULL == gds_hash_map_new(0, NULL, test_cmpkey, NULL, NULL));
	ok(NULL == gds_hash_map_new(0, NULL, test_cmpkey, NULL, free));
	ok(NULL == gds_hash_map_new(0, NULL, test_cmpkey, free, NULL));
	ok(NULL == gds_hash_map_new(0, NULL, test_cmpkey, free, free));
	ok(NULL == gds_hash_map_new(0, gds_hash_djb2, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_new(0, gds_hash_djb2, NULL, NULL, free));
	ok(NULL == gds_hash_map_new(0, gds_hash_djb2, NULL, free, NULL));
	ok(NULL == gds_hash_map_new(0, gds_hash_djb2, NULL, free, free));
	ok(NULL == gds_hash_map_new(0, gds_hash_djb2, test_cmpkey, NULL, NULL));
	ok(NULL == gds_hash_map_new(0, gds_hash_djb2, test_cmpkey, NULL, free));
	ok(NULL == gds_hash_map_new(0, gds_hash_djb2, test_cmpkey, free, NULL));
	ok(NULL == gds_hash_map_new(0, gds_hash_djb2, test_cmpkey, free, free));
	ok(NULL == gds_hash_map_new(32, NULL, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_new(32, NULL, NULL, NULL, free));
	ok(NULL == gds_hash_map_new(32, NULL, NULL, free, NULL));
	ok(NULL == gds_hash_map_new(32, NULL, NULL, free, free));
	ok(NULL == gds_hash_map_new(32, NULL, test_cmpkey, NULL, NULL));
	ok(NULL == gds_hash_map_new(32, NULL, test_cmpkey, NULL, free));
	ok(NULL == gds_hash_map_new(32, NULL, test_cmpkey, free, NULL));
	ok(NULL == gds_hash_map_new(32, NULL, test_cmpkey, free, free));
	ok(NULL == gds_hash_map_new(32, gds_hash_djb2, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_new(32, gds_hash_djb2, NULL, NULL, free));
	ok(NULL == gds_hash_map_new(32, gds_hash_djb2, NULL, free, NULL));
	ok(NULL == gds_hash_map_new(32, gds_hash_djb2, NULL, free, free));

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, NULL, NULL);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_free(hash);

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, NULL, free);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_free(hash);

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, free, NULL);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_free(hash);

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, free, free);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_free(hash);
}

void t_hash_map_set(void)
{
	gds_hash_map_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;
	int rc;

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, NULL, NULL);

	ok(0 > gds_hash_map_set(NULL, NULL, NULL));
	ok(0 > gds_hash_map_set(NULL, NULL, data[0]));
	ok(0 > gds_hash_map_set(NULL, keys[0], NULL));
	ok(0 > gds_hash_map_set(NULL, keys[0], data[0]));

	rc = gds_hash_map_set(hash, keys[0], data[0]);
	is(rc, 0);
	rc = gds_hash_map_set(hash, keys[0], data[0]);
	is(rc, 1);
	rc = gds_hash_map_set(hash, keys[0], data[0]);
	is(rc, 1);

	s = gds_hash_map_get(hash, keys[0]);
	isnt(s, NULL);
	str_eq(s, data[0]);

	gds_hash_map_set(hash, keys[1], data[1]);
	gds_hash_map_set(hash, keys[2], data[2]);

	s = gds_hash_map_get(hash, keys[1]);
	isnt(s, NULL);
	str_eq(s, data[1]);

	s = gds_hash_map_get(hash, keys[2]);
	isnt(s, NULL);
	str_eq(s, data[2]);

	gds_hash_map_free(hash);
}

void t_hash_map_get(void)
{
	gds_hash_map_t *hash;
	char *keys[] = {"one", "two", "three"};
	char *data[] = {"1", "2", "3"};
	char *s;

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, NULL, NULL);

	ok(NULL == gds_hash_map_get(NULL, NULL));
	ok(NULL == gds_hash_map_get(NULL, keys[0]));

	s = gds_hash_map_get(hash, NULL);
	is(s, NULL);
	s = gds_hash_map_get(hash, keys[0]);
	is(s, NULL);
	s = gds_hash_map_get(hash, keys[1]);
	is(s, NULL);
	s = gds_hash_map_get(hash, keys[2]);
	is(s, NULL);

	gds_hash_map_set(hash, keys[0], data[0]);
	s = gds_hash_map_get(hash, keys[0]);
	isnt(s, NULL);
	str_eq(s, data[0]);
	s = gds_hash_map_get(hash, keys[1]);
	is(s, NULL);
	s = gds_hash_map_get(hash, keys[2]);
	is(s, NULL);

	gds_hash_map_set(hash, keys[1], data[1]);
	gds_hash_map_set(hash, keys[2], data[2]);
	s = gds_hash_map_get(hash, keys[0]);
	isnt(s, NULL);
	str_eq(s, data[0]);
	s = gds_hash_map_get(hash, keys[1]);
	isnt(s, NULL);
	str_eq(s, data[1]);
	s = gds_hash_map_get(hash, keys[2]);
	isnt(s, NULL);
	str_eq(s, data[2]);

	gds_hash_map_free(hash);
}

void t_hash_map_unset(void)
{
	gds_hash_map_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * data[] = {"one", "two", "three"};
	int rc;

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, NULL, NULL);

	ok(0 > gds_hash_map_unset(NULL, NULL));
	ok(0 > gds_hash_map_unset(NULL, keys[0]));

	rc = gds_hash_map_unset(hash, NULL);
	is(rc, 1);
	rc = gds_hash_map_unset(hash, keys[0]);
	is(rc, 1);
	rc = gds_hash_map_unset(hash, keys[1]);
	is(rc, 1);
	rc = gds_hash_map_unset(hash, keys[2]);
	is(rc, 1);

	gds_hash_map_set(hash, keys[0], data[0]);
	gds_hash_map_set(hash, keys[1], data[1]);
	gds_hash_map_set(hash, keys[2], data[2]);

	rc = gds_hash_map_unset(hash, NULL);
	is(rc, 1);
	rc = gds_hash_map_unset(hash, keys[0]);
	is(rc, 0);
	rc = gds_hash_map_unset(hash, keys[1]);
	is(rc, 0);
	rc = gds_hash_map_unset(hash, keys[2]);
	is(rc, 0);

	rc = gds_hash_map_unset(hash, NULL);
	is(rc, 1);
	rc = gds_hash_map_unset(hash, keys[0]);
	is(rc, 1);
	rc = gds_hash_map_unset(hash, keys[1]);
	is(rc, 1);
	rc = gds_hash_map_unset(hash, keys[2]);
	is(rc, 1);

	gds_hash_map_free(hash);
}

void t_hash_map_pop(void)
{
	gds_hash_map_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * data[] = {"one", "two", "three"};
	void *d;

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, NULL, NULL);

	ok(NULL == gds_hash_map_pop(NULL, NULL));
	ok(NULL == gds_hash_map_pop(NULL, keys[0]));

	d = gds_hash_map_pop(hash, NULL);
	is(d, NULL);
	d = gds_hash_map_pop(hash, keys[0]);
	is(d, NULL);
	d = gds_hash_map_pop(hash, keys[1]);
	is(d, NULL);
	d = gds_hash_map_pop(hash, keys[2]);
	is(d, NULL);

	gds_hash_map_set(hash, keys[0], data[0]);
	gds_hash_map_set(hash, keys[1], data[1]);
	gds_hash_map_set(hash, keys[2], data[2]);

	d = gds_hash_map_pop(hash, NULL);
	is(d, NULL);
	d = gds_hash_map_pop(hash, keys[0]);
	is(d, data[0]);
	d = gds_hash_map_pop(hash, keys[1]);
	is(d, data[1]);
	d = gds_hash_map_pop(hash, keys[2]);
	is(d, data[2]);

	d = gds_hash_map_pop(hash, NULL);
	is(d, NULL);
	d = gds_hash_map_pop(hash, keys[0]);
	is(d, NULL);
	d = gds_hash_map_pop(hash, keys[1]);
	is(d, NULL);
	d = gds_hash_map_pop(hash, keys[2]);
	is(d, NULL);

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

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		rc = gds_hash_map_set(hash, keys[i], values[i]);
		is(rc, 0);
	}

	it = gds_hash_map_iterator_new(hash);
	isnt(it, NULL);

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	isnt(key, NULL);
	isnt(value, NULL);
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
	gds_hash_map_free(hash);
}

void t_hash_map_keys(void)
{
	gds_hash_map_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *s;
	int check[] = {0, 0, 0};

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_set(hash, keys[i], values[i]);
	}

	gds_slist_t *k = gds_hash_map_keys(hash);
	is(gds_slist_size(k), 3);
	gds_slist_foreach (s, k) {
		if (s == keys[0]) check[0] = 1;
		else if (s == keys[1]) check[1] = 1;
		else if (s == keys[2]) check[2] = 1;
	}
	is(check[0], 1);
	is(check[1], 1);
	is(check[2], 1);

	gds_slist_free(k, NULL, NULL);

	gds_hash_map_free(hash);
}

void t_hash_map_values(void)
{
	gds_hash_map_t *hash;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *s;
	int check[] = {0, 0, 0};

	hash = gds_hash_map_new(32, gds_hash_djb2, test_cmpkey, NULL, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_set(hash, keys[i], values[i]);
	}

	gds_slist_t *v = gds_hash_map_values(hash);
	is(gds_slist_size(v), 3);
	gds_slist_foreach (s, v) {
		if (s == values[0]) check[0] = 1;
		else if (s == values[1]) check[1] = 1;
		else if (s == values[2]) check[2] = 1;
	}
	is(check[0], 1);
	is(check[1], 1);
	is(check[2], 1);

	gds_slist_free(v, NULL, NULL);

	gds_hash_map_free(hash);
}

int main()
{
	plan(110);

	t_hash_map_new();
	t_hash_map_set();
	t_hash_map_get();
	t_hash_map_unset();
	t_hash_map_pop();
	t_hash_map_iterator();
	t_hash_map_keys();
	t_hash_map_values();

	return EXIT_SUCCESS;
}
