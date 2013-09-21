#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <CUnit/Basic.h>
#include <libexception/exception.h>
#include "hash_map.h"

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

void t_hash_map_iterator(void)
{
	gds_hash_map_t *hash;
	gds_iterator_t *it;
	char * keys[] = {"1", "2", "3"};
	char * values[] = {"one", "two", "three"};
	char *key, *value;

	hash = gds_hash_map_new(32, (gds_hash_cb) test_hash,
		(gds_cmpkey_cb) test_cmpkey);
	for (int i = 0; i < 3; i++) {
		gds_hash_map_set(hash, keys[i], values[i], NULL);
	}

	it = gds_hash_map_iterator_new(hash);

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	CU_ASSERT_STRING_EQUAL(key, "1");
	CU_ASSERT_STRING_EQUAL(value, "one");

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	CU_ASSERT_STRING_EQUAL(key, "2");
	CU_ASSERT_STRING_EQUAL(value, "two");

	gds_iterator_step(it);
	key = gds_iterator_getkey(it);
	value = gds_iterator_get(it);
	CU_ASSERT_STRING_EQUAL(key, "3");
	CU_ASSERT_STRING_EQUAL(value, "three");

	CU_ASSERT(0 < gds_iterator_step(it));

	gds_iterator_free(it);
	gds_hash_map_free(hash, NULL, NULL);
}

int main()
{
	CU_pSuite pSuite = NULL;
	int fails = 0;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Compact Red-Black tree",
		init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if(
	   (NULL == CU_add_test(pSuite, "gds_hash_map_iterator", t_hash_map_iterator))
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	try {
		CU_basic_run_tests();
	} catch() as (e) {
		fprintf(stderr, "\nTests returned an unexpected exception\n");
		fprintf(stderr, "\tType: %s\n", e->type());
		fprintf(stderr, "\tMessage: %s\n", e->message());
		fprintf(stderr, "\tFile: %s\n", e->filename());
		fprintf(stderr, "\tFunction: %s\n", e->function());
		fprintf(stderr, "\tLine: %d\n", e->line());
		return EXIT_FAILURE;
	}

	fails = CU_get_number_of_failures();
	CU_cleanup_registry();

	return fails ? EXIT_FAILURE : EXIT_SUCCESS;
}
