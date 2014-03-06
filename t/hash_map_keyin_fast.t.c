#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libexception/exception.h>
#include <libtap13/tap.h>
#include "hash_map_keyin_fast.h"
#include "hash_functions.h"
#include "slist.h"
#include "test.h"

void t_hash_map_keyin_fast_new(void)
{
	gds_hash_map_keyin_fast_t *hash;

	ok(NULL == gds_hash_map_keyin_fast_new(0, NULL, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(0, NULL, NULL, NULL, free));
	ok(NULL == gds_hash_map_keyin_fast_new(0, NULL, NULL, test_cmpkey, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(0, NULL, NULL, test_cmpkey, free));
	ok(NULL == gds_hash_map_keyin_fast_new(0, NULL, test_getkey, NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(0, NULL, test_getkey, NULL, free));
	ok(NULL == gds_hash_map_keyin_fast_new(0, NULL, test_getkey, test_cmpkey, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(0, NULL, test_getkey, test_cmpkey, free));
	ok(NULL == gds_hash_map_keyin_fast_new(0, gds_hash_sdbm, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(0, gds_hash_sdbm, NULL, NULL, free));
	ok(NULL == gds_hash_map_keyin_fast_new(0, gds_hash_sdbm, NULL, test_cmpkey, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(0, gds_hash_sdbm, NULL, test_cmpkey, free));
	ok(NULL == gds_hash_map_keyin_fast_new(0, gds_hash_sdbm, test_getkey, NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(0, gds_hash_sdbm, test_getkey, NULL, free));
	ok(NULL == gds_hash_map_keyin_fast_new(0, gds_hash_sdbm, test_getkey, test_cmpkey, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(0, gds_hash_sdbm, test_getkey, test_cmpkey, free));
	ok(NULL == gds_hash_map_keyin_fast_new(32, NULL, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(32, NULL, NULL, NULL, free));
	ok(NULL == gds_hash_map_keyin_fast_new(32, NULL, NULL, test_cmpkey, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(32, NULL, NULL, test_cmpkey, free));
	ok(NULL == gds_hash_map_keyin_fast_new(32, NULL, test_getkey, NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(32, NULL, test_getkey, NULL, free));
	ok(NULL == gds_hash_map_keyin_fast_new(32, NULL, test_getkey, test_cmpkey, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(32, NULL, test_getkey, test_cmpkey, free));
	ok(NULL == gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, NULL, NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, NULL, NULL, free));
	ok(NULL == gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, NULL, test_cmpkey, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, NULL, test_cmpkey, free));
	ok(NULL == gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, test_getkey, NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, test_getkey, NULL, free));

	hash = gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, test_getkey, test_cmpkey, NULL);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_keyin_fast_free(hash);

	hash = gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, test_getkey, test_cmpkey, free);
	isnt(hash, NULL, "hash creation succeeded");
	gds_hash_map_keyin_fast_free(hash);
}

void t_hash_map_keyin_fast_set(void)
{
	gds_hash_map_keyin_fast_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	test_t *t;
	int rc;

	hash = gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, test_getkey, test_cmpkey, NULL);

	ok(0 > gds_hash_map_keyin_fast_set(NULL, NULL));
	ok(0 > gds_hash_map_keyin_fast_set(NULL, &data[0]));

	rc = gds_hash_map_keyin_fast_set(hash, &data[0]);
	is(rc, 0);
	rc = gds_hash_map_keyin_fast_set(hash, &data[0]);
	is(rc, 1);
	rc = gds_hash_map_keyin_fast_set(hash, &data[0]);
	is(rc, 1);

	t = gds_hash_map_keyin_fast_get(hash, data[0].key);
	isnt(t, NULL);
	is(t, &data[0]);

	gds_hash_map_keyin_fast_set(hash, &data[1]);
	gds_hash_map_keyin_fast_set(hash, &data[2]);

	t = gds_hash_map_keyin_fast_get(hash, data[1].key);
	isnt(t, NULL);
	is(t, &data[1]);

	t = gds_hash_map_keyin_fast_get(hash, data[2].key);
	isnt(t, NULL);
	is(t, &data[2]);

	gds_hash_map_keyin_fast_free(hash);
}

void t_hash_map_keyin_fast_get(void)
{
	gds_hash_map_keyin_fast_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	test_t *t;

	hash = gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, test_getkey, test_cmpkey, NULL);

	ok(NULL == gds_hash_map_keyin_fast_get(NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_get(NULL, data[0].key));

	t = gds_hash_map_keyin_fast_get(hash, NULL);
	is(t, NULL);
	t = gds_hash_map_keyin_fast_get(hash, data[0].key);
	is(t, NULL);
	t = gds_hash_map_keyin_fast_get(hash, data[1].key);
	is(t, NULL);
	t = gds_hash_map_keyin_fast_get(hash, data[2].key);
	is(t, NULL);

	gds_hash_map_keyin_fast_set(hash, &data[0]);
	t = gds_hash_map_keyin_fast_get(hash, data[0].key);
	isnt(t, NULL);
	is(t, &data[0]);
	t = gds_hash_map_keyin_fast_get(hash, data[1].key);
	is(t, NULL);
	t = gds_hash_map_keyin_fast_get(hash, data[2].key);
	is(t, NULL);

	gds_hash_map_keyin_fast_set(hash, &data[1]);
	gds_hash_map_keyin_fast_set(hash, &data[2]);
	t = gds_hash_map_keyin_fast_get(hash, data[0].key);
	isnt(t, NULL);
	is(t, &data[0]);
	t = gds_hash_map_keyin_fast_get(hash, data[1].key);
	isnt(t, NULL);
	is(t, &data[1]);
	t = gds_hash_map_keyin_fast_get(hash, data[2].key);
	isnt(t, NULL);
	is(t, &data[2]);

	gds_hash_map_keyin_fast_free(hash);
}

void t_hash_map_keyin_fast_unset(void)
{
	gds_hash_map_keyin_fast_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	int rc;

	hash = gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, test_getkey, test_cmpkey, NULL);

	ok(0 > gds_hash_map_keyin_fast_unset(NULL, NULL));
	ok(0 > gds_hash_map_keyin_fast_unset(NULL, data[0].key));

	rc = gds_hash_map_keyin_fast_unset(hash, NULL);
	is(rc, 1);
	rc = gds_hash_map_keyin_fast_unset(hash, data[0].key);
	is(rc, 1);
	rc = gds_hash_map_keyin_fast_unset(hash, data[1].key);
	is(rc, 1);
	rc = gds_hash_map_keyin_fast_unset(hash, data[2].key);
	is(rc, 1);

	gds_hash_map_keyin_fast_set(hash, &data[0]);
	gds_hash_map_keyin_fast_set(hash, &data[1]);
	gds_hash_map_keyin_fast_set(hash, &data[2]);

	rc = gds_hash_map_keyin_fast_unset(hash, NULL);
	is(rc, 1);
	rc = gds_hash_map_keyin_fast_unset(hash, data[0].key);
	is(rc, 0);
	rc = gds_hash_map_keyin_fast_unset(hash, data[1].key);
	is(rc, 0);
	rc = gds_hash_map_keyin_fast_unset(hash, data[2].key);
	is(rc, 0);

	rc = gds_hash_map_keyin_fast_unset(hash, NULL);
	is(rc, 1);
	rc = gds_hash_map_keyin_fast_unset(hash, data[0].key);
	is(rc, 1);
	rc = gds_hash_map_keyin_fast_unset(hash, data[1].key);
	is(rc, 1);
	rc = gds_hash_map_keyin_fast_unset(hash, data[2].key);
	is(rc, 1);

	gds_hash_map_keyin_fast_free(hash);
}

void t_hash_map_keyin_fast_pop(void)
{
	gds_hash_map_keyin_fast_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	void *d;

	hash = gds_hash_map_keyin_fast_new(32, gds_hash_djb2, test_getkey, test_cmpkey, NULL);

	ok(NULL == gds_hash_map_keyin_fast_pop(NULL, NULL));
	ok(NULL == gds_hash_map_keyin_fast_pop(NULL, data[0].key));

	d = gds_hash_map_keyin_fast_pop(hash, NULL);
	is(d, NULL);
	d = gds_hash_map_keyin_fast_pop(hash, data[0].key);
	is(d, NULL);
	d = gds_hash_map_keyin_fast_pop(hash, data[1].key);
	is(d, NULL);
	d = gds_hash_map_keyin_fast_pop(hash, data[2].key);
	is(d, NULL);

	gds_hash_map_keyin_fast_set(hash, &data[0]);
	gds_hash_map_keyin_fast_set(hash, &data[1]);
	gds_hash_map_keyin_fast_set(hash, &data[2]);

	d = gds_hash_map_keyin_fast_pop(hash, NULL);
	is(d, NULL);
	d = gds_hash_map_keyin_fast_pop(hash, data[0].key);
	is(d, &data[0]);
	d = gds_hash_map_keyin_fast_pop(hash, data[1].key);
	is(d, &data[1]);
	d = gds_hash_map_keyin_fast_pop(hash, data[2].key);
	is(d, &data[2]);

	d = gds_hash_map_keyin_fast_pop(hash, NULL);
	is(d, NULL);
	d = gds_hash_map_keyin_fast_pop(hash, data[0].key);
	is(d, NULL);
	d = gds_hash_map_keyin_fast_pop(hash, data[1].key);
	is(d, NULL);
	d = gds_hash_map_keyin_fast_pop(hash, data[2].key);
	is(d, NULL);

	gds_hash_map_keyin_fast_free(hash);
}

void t_hash_map_keyin_fast_iterator(void)
{
	gds_hash_map_keyin_fast_t *hash;
	gds_iterator_t *it;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	char *key;
	test_t *value;
	int rc;

	hash = gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, test_getkey, test_cmpkey, NULL);
	for (int i = 0; i < 3; i++) {
		rc = gds_hash_map_keyin_fast_set(hash, &data[i]);
		is(rc, 0);
	}

	it = gds_hash_map_keyin_fast_iterator_new(hash);
	isnt(it, NULL);

	while (!gds_iterator_step(it)) {
		key = gds_iterator_getkey(it);
		value = gds_iterator_get(it);
		isnt(key, NULL);
		isnt(value, NULL);

		if (value == &data[0]) {
			str_eq(key, data[0].key);
		} else if (value == &data[1]) {
			str_eq(key, data[1].key);
		} else if (value == &data[2]) {
			str_eq(key, data[2].key);
		} else {
			fail("Unknown data in hash");
		}
	}

	ok(0 < gds_iterator_step(it));

	gds_iterator_free(it);
	gds_hash_map_keyin_fast_free(hash);
}

void t_hash_map_keyin_fast_values(void)
{
	gds_hash_map_keyin_fast_t *hash;
	test_t data[] = {
		{.key = "one", .value = 1},
		{.key = "two", .value = 2},
		{.key = "three", .value = 3}
	};
	test_t *t;
	int check[] = {0, 0, 0};

	hash = gds_hash_map_keyin_fast_new(32, gds_hash_sdbm, test_getkey, test_cmpkey, NULL);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_keyin_fast_set(hash, &data[i]);
	}

	gds_slist_t *v = gds_hash_map_keyin_fast_values(hash);
	is(gds_slist_size(v), 3);
	gds_slist_foreach (t, v) {
		if (t == &data[0]) check[0] = 1;
		else if (t == &data[1]) check[1] = 1;
		else if (t == &data[2]) check[2] = 1;
	}
	is(check[0], 1);
	is(check[1], 1);
	is(check[2], 1);

	gds_slist_free(v);

	gds_hash_map_keyin_fast_free(hash);
}

int main()
{
	plan(105);

	t_hash_map_keyin_fast_new();
	t_hash_map_keyin_fast_set();
	t_hash_map_keyin_fast_get();
	t_hash_map_keyin_fast_unset();
	t_hash_map_keyin_fast_pop();
	t_hash_map_keyin_fast_iterator();
	t_hash_map_keyin_fast_values();

	return EXIT_SUCCESS;
}
