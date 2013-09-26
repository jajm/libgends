#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <CUnit/Basic.h>
#include "test_macros.h"
#include "rbtree_fast.h"
#include "iterator.h"
#include "slist.h"
#include "test.h"

int init_suite(void)
{
	return 0;
}

int clean_suite(void)
{
	return 0;
}

#define containerof(ptr) \
	((ptr) != NULL) ? (gds_rbtree_fast_node_t *)((char *)ptr \
		- offsetof(gds_rbtree_fast_node_t, rbtree)) : NULL

void gds_rbtree_fast_print(gds_rbtree_fast_node_t *root, uint8_t depth)
{
	char *key;
	gds_rbtree_fast_node_t *node;
	if(root != NULL) {
		node = containerof(root->rbtree.right);
		gds_rbtree_fast_print(node, depth+1);
		for (uint8_t i=0; i<depth; i++)
			printf("- ");
		key = root->key;
		if (key == NULL) {
			printf("(NULL)");
		} else {
			printf("%s", key);
		}
		printf(": %d\n", test_getvalue(root->data));
		node = containerof(root->rbtree.left);
		gds_rbtree_fast_print(node, depth+1);
	}
}

void gds_rbtree_fast_print_dbg_r(gds_rbtree_fast_node_t *root, uint8_t d)
{
	char *key;
	if (root != NULL) {
		gds_rbtree_fast_print_dbg_r(containerof(root->rbtree.right), d+1);
		for (uint8_t i=0; i<d; i++)
			printf("- ");
		key = root->key;
		printf("root %p rbtree %p [%s] (0: %p, 1: %p) %s\n",
			root, &(root->rbtree), root->rbtree.red ? "red" : "black",
			root->rbtree.left, root->rbtree.right, key);
		gds_rbtree_fast_print_dbg_r(containerof(root->rbtree.left), d+1);
	}
}
void gds_rbtree_fast_print_dbg(gds_rbtree_fast_node_t *root)
{
	printf("\n---------------------------------------------------------\n");
	gds_rbtree_fast_print_dbg_r(root, 0);
	printf("---------------------------------------------------------\n");
}

int8_t gds_rbtree_fast_is_valid(gds_rbtree_fast_node_t *root,
	gds_cmpkey_cb cmpkey_cb)
{
	int lh, rh;
	int cmp = 0, cmp2 = 0;
	gds_rbtree_fast_node_t *node;

	if (root == NULL)
		return 1;

	gds_inline_rbtree_fast_node_t *ln = root->rbtree.left;
	gds_inline_rbtree_fast_node_t *rn = root->rbtree.right;

	/* Consecutive red links */
	if (root->rbtree.red) {
		if ((ln && ln->red) || (rn && rn->red)) {
			printf("Red violation\n");
			return 0;
		}
	}

	node = containerof(ln);
	lh = gds_rbtree_fast_is_valid(node, cmpkey_cb);
	node = containerof(rn);
	rh = gds_rbtree_fast_is_valid(node, cmpkey_cb);

	/* Invalid binary search tree */
	if(ln != NULL) {
		node = containerof(ln);
		cmp = cmpkey_cb(root->key, node->key);
	}
	if(rn != NULL) {
		node = containerof(rn);
		cmp2 = cmpkey_cb(root->key, node->key);
	}
	if ((ln != NULL && cmp <= 0)
	|| (rn != NULL && cmp2 >= 0)) {
		printf("Binary tree violation\n");
		return 0;
	}

	/* Black height mismatch */
	if ( lh != 0 && rh != 0 && lh != rh ) {
		printf("Black violation\n");
		return 0;
	}

	/* Only count black links */
	if (lh != 0 && rh != 0)
		return root->rbtree.red ? lh : lh + 1;
	else
		return 0;
}

char * key_alloc(const char *key)
{
	char *s = NULL;
	size_t len;
	
	if (key != NULL) {
		len = strlen(key);
		s = malloc(len+1);
		strncpy(s, key, len+1);
	}

	return s;
}

void t_rbtree_fast_add(void)
{
	gds_rbtree_fast_node_t *root = NULL;
	test_t *t = test_new("string", 4);
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_free_cb key_free_cb = (gds_free_cb)free;
	gds_free_cb free_cb = (gds_free_cb)test_free;
	char *key = "key";
	char *key2 = "key2";

	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(NULL , NULL, NULL, NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(NULL , NULL, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(NULL , NULL, t   , NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(NULL , NULL, t   , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(NULL , key , NULL, NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(NULL , key , NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(NULL , key , t   , NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(NULL , key , t   , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(&root, NULL, NULL, NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(&root, NULL, t   , NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(&root, key , NULL, NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_add(&root, key , t   , NULL     ));

	/* Tree is unchanged */
	CU_ASSERT_PTR_NULL(root);
	CU_ASSERT(gds_rbtree_fast_is_valid(root, cmpkey_cb));

	/* Add a data with NULL key, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	CU_ASSERT(0 == gds_rbtree_fast_add(&root, NULL, NULL, cmpkey_cb));
	CU_ASSERT(0 <  gds_rbtree_fast_add(&root, NULL, NULL, cmpkey_cb));
	CU_ASSERT(0 <  gds_rbtree_fast_add(&root, NULL, t   , cmpkey_cb));

	CU_ASSERT(gds_rbtree_fast_is_valid(root, cmpkey_cb));

	/* Add NULL as data, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	CU_ASSERT(0 == gds_rbtree_fast_add(&root, key, NULL, cmpkey_cb));
	CU_ASSERT(0 <  gds_rbtree_fast_add(&root, key, NULL, cmpkey_cb));

	CU_ASSERT(gds_rbtree_fast_is_valid(root, cmpkey_cb));

	/* Add t as data, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	CU_ASSERT(0 == gds_rbtree_fast_add(&root, key2, t, cmpkey_cb));
	CU_ASSERT(0 <  gds_rbtree_fast_add(&root, key2, t, cmpkey_cb));

	CU_ASSERT(gds_rbtree_fast_is_valid(root, cmpkey_cb));

	gds_rbtree_fast_free(root, NULL, (gds_free_cb)test_free);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		CU_ASSERT(0 == gds_rbtree_fast_add(&root, key_alloc(buf), test_new(buf, i), cmpkey_cb));
		CU_ASSERT(gds_rbtree_fast_is_valid(root, cmpkey_cb));
	}

	gds_rbtree_fast_free(root, key_free_cb, free_cb);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %02d", i);
		CU_ASSERT(0 == gds_rbtree_fast_add(&root, key_alloc(buf), test_new(buf, i), cmpkey_cb));
		CU_ASSERT(gds_rbtree_fast_is_valid(root, cmpkey_cb));
	}

	gds_rbtree_fast_free(root, key_free_cb, free_cb);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %02d", j);
		CU_ASSERT(0 <= gds_rbtree_fast_add(&root, key_alloc(buf), test_new(buf, i), cmpkey_cb));
		CU_ASSERT(gds_rbtree_fast_is_valid(root, cmpkey_cb));
	}

	gds_rbtree_fast_free(root, key_free_cb, free_cb);
}

void t_rbtree_fast_get(void)
{
	gds_rbtree_fast_node_t *root = NULL;
	test_t *data;
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_free_cb key_free_cb = (gds_free_cb)free;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	gds_rbtree_fast_add(&root, "key", test_new("string", 1), cmpkey_cb);

	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_fast_get(NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_fast_get(NULL, "key", NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_fast_get(root, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_fast_get(root, "key", NULL));
	CU_ASSERT_PTR_NULL(gds_rbtree_fast_get(NULL, NULL, cmpkey_cb));
	CU_ASSERT_PTR_NULL(gds_rbtree_fast_get(NULL, "key", cmpkey_cb));
	CU_ASSERT_PTR_NULL(gds_rbtree_fast_get(root, NULL, cmpkey_cb));
	CU_ASSERT_PTR_NOT_NULL(gds_rbtree_fast_get(root, "key", cmpkey_cb));

	gds_rbtree_fast_free(root, NULL, free_cb);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_fast_add(&root, key_alloc(buf), test_new(buf, i), cmpkey_cb);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		data = gds_rbtree_fast_get(root, buf, cmpkey_cb);
		CU_ASSERT_PTR_NOT_NULL(data);
		CU_ASSERT_EQUAL(i, test_getvalue(data));
	}
	gds_rbtree_fast_free(root, key_free_cb, free_cb);
}

void t_rbtree_fast_del(void)
{
	gds_rbtree_fast_node_t *root = NULL;
	test_t *t = test_new("key", 1);
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_free_cb free_cb = (gds_free_cb)test_free;
	gds_free_cb key_free_cb = (gds_free_cb)free;

	gds_rbtree_fast_add(&root, "key", t, cmpkey_cb);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , NULL , NULL     , NULL       , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , NULL , NULL     , NULL       , free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , NULL , NULL     , key_free_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , NULL , NULL     , key_free_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , NULL , cmpkey_cb, NULL       , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , NULL , cmpkey_cb, NULL       , free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , NULL , cmpkey_cb, key_free_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , NULL , cmpkey_cb, key_free_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , "key", NULL     , NULL       , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , "key", NULL     , NULL       , free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , "key", NULL     , key_free_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , "key", NULL     , key_free_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , "key", cmpkey_cb, NULL       , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , "key", cmpkey_cb, NULL       , free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , "key", cmpkey_cb, key_free_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(NULL , "key", cmpkey_cb, key_free_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(&root, NULL , NULL     , NULL       , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(&root, NULL , NULL     , NULL       , free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(&root, NULL , NULL     , key_free_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(&root, NULL , NULL     , key_free_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(&root, "key", NULL     , NULL       , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(&root, "key", NULL     , NULL       , free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(&root, "key", NULL     , key_free_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_fast_del(&root, "key", NULL     , key_free_cb, free_cb));

	CU_ASSERT(0 < gds_rbtree_fast_del(&root, NULL, cmpkey_cb, NULL, NULL));
	CU_ASSERT(0 < gds_rbtree_fast_del(&root, NULL, cmpkey_cb, NULL, free_cb));
	CU_ASSERT(0 < gds_rbtree_fast_del(&root, NULL, cmpkey_cb, key_free_cb, NULL));
	CU_ASSERT(0 < gds_rbtree_fast_del(&root, NULL, cmpkey_cb, key_free_cb, free_cb));
	CU_ASSERT(0 == gds_rbtree_fast_del(&root, "key", cmpkey_cb, NULL, NULL));
	CU_ASSERT(0 < gds_rbtree_fast_del(&root, "key", cmpkey_cb, NULL, NULL));
	CU_ASSERT(0 < gds_rbtree_fast_del(&root, "key", cmpkey_cb, NULL, free_cb));
	CU_ASSERT(0 < gds_rbtree_fast_del(&root, "key", cmpkey_cb, key_free_cb, NULL));
	CU_ASSERT(0 < gds_rbtree_fast_del(&root, "key", cmpkey_cb, key_free_cb, free_cb));

	test_free(t);

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_fast_add(&root, key_alloc(buf), test_new(buf, i), cmpkey_cb);
	}
	for (int i=50; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 == gds_rbtree_fast_del(&root, buf,
			cmpkey_cb, key_free_cb, free_cb));
		CU_ASSERT(gds_rbtree_fast_is_valid(root, cmpkey_cb));
	}
	for (int i=49; i>=0; i--) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 == gds_rbtree_fast_del(&root, buf,
			cmpkey_cb, key_free_cb, free_cb));
		CU_ASSERT(gds_rbtree_fast_is_valid(root, cmpkey_cb));
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 < gds_rbtree_fast_del(&root, buf,
			cmpkey_cb, key_free_cb, free_cb));
	}
}

void t_rbtree_fast_iterator(void)
{
	gds_rbtree_fast_node_t *root = NULL;
	gds_iterator_t *it;
	test_t *t;
	char buf[512];
	int i;
	const char *key;
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_free_cb free_cb = (gds_free_cb)test_free;
	gds_free_cb key_free_cb = (gds_free_cb)free;

	for(i=0; i<5; i++) {
		sprintf(buf, "key %02d", i);
		t = test_new(buf, i);
		gds_rbtree_fast_add(&root, key_alloc(buf), t, cmpkey_cb);
	}

	it = gds_rbtree_fast_iterator_new(root);
	CU_ASSERT_PTR_NOT_NULL_FATAL(it);

	CU_ASSERT_PTR_NULL(gds_iterator_get(it));
	CU_ASSERT_PTR_NULL(gds_iterator_getkey(it));

	CU_ASSERT(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	CU_ASSERT_PTR_NOT_NULL_FATAL(t);
	CU_ASSERT_PTR_NOT_NULL_FATAL(key);
	CU_ASSERT_STRING_EQUAL(key, "key 00");

	CU_ASSERT(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	CU_ASSERT_PTR_NOT_NULL_FATAL(t);
	CU_ASSERT_PTR_NOT_NULL_FATAL(key);
	CU_ASSERT_STRING_EQUAL(key, "key 01");

	CU_ASSERT(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	CU_ASSERT_PTR_NOT_NULL_FATAL(t);
	CU_ASSERT_PTR_NOT_NULL_FATAL(key);
	CU_ASSERT_STRING_EQUAL(key, "key 02");

	CU_ASSERT(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	CU_ASSERT_PTR_NOT_NULL_FATAL(t);
	CU_ASSERT_PTR_NOT_NULL_FATAL(key);
	CU_ASSERT_STRING_EQUAL(key, "key 03");

	CU_ASSERT(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	CU_ASSERT_PTR_NOT_NULL_FATAL(t);
	CU_ASSERT_PTR_NOT_NULL_FATAL(key);
	CU_ASSERT_STRING_EQUAL(key, "key 04");

	CU_ASSERT(0 < gds_iterator_step(it));

	gds_iterator_free(it);
	gds_rbtree_fast_free(root, key_free_cb, free_cb);
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
	   (NULL == CU_add_test(pSuite, "gds_rbtree_fast_add()", t_rbtree_fast_add))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_fast_get()", t_rbtree_fast_get))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_fast_del()", t_rbtree_fast_del))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_fast_iterator", t_rbtree_fast_iterator))
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
