#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <CUnit/Basic.h>
#include "test_macros.h"
#include "rbtree_keyin_fast.h"
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
	((ptr) != NULL) ? (gds_rbtree_keyin_fast_node_t *)((char *)ptr \
		- offsetof(gds_rbtree_keyin_fast_node_t, rbtree)) : NULL

void gds_rbtree_keyin_fast_print(gds_rbtree_keyin_fast_node_t *root, uint8_t depth)
{
	char *key;
	gds_rbtree_keyin_fast_node_t *node;
	if(root != NULL) {
		node = containerof(root->rbtree.right);
		gds_rbtree_keyin_fast_print(node, depth+1);
		for (uint8_t i=0; i<depth; i++)
			printf("- ");
		key = test_getkey(root->data);
		if (key == NULL) {
			printf("(NULL)");
		} else {
			printf("%s", key);
		}
		printf(": %d\n", test_getvalue(root->data));
		node = containerof(root->rbtree.left);
		gds_rbtree_keyin_fast_print(node, depth+1);
	}
}

int8_t gds_rbtree_keyin_fast_is_valid(gds_rbtree_keyin_fast_node_t *root, gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	int lh, rh;
	int cmp = 0, cmp2 = 0;
	gds_rbtree_keyin_fast_node_t *node;

	if ( root == NULL )
		return 1;

	gds_inline_rbtree_fast_node_t *ln = root->rbtree.left;
	gds_inline_rbtree_fast_node_t *rn = root->rbtree.right;

	/* Consecutive red links */
	if (root && root->rbtree.red) {
		if ((ln && ln->red) || (rn && rn->red)) {
			printf("Red violation\n");
			return 0;
		}
	}

	node = containerof(ln);
	lh = gds_rbtree_keyin_fast_is_valid(node, getkey_cb, cmpkey_cb);
	node = containerof(rn);
	rh = gds_rbtree_keyin_fast_is_valid(node, getkey_cb, cmpkey_cb);

	/* Invalid binary search tree */
	if(ln != NULL) {
		node = containerof(ln);
		cmp = cmpkey_cb((void *)getkey_cb(root->data), getkey_cb(node->data));
	}
	if(rn != NULL) {
		node = containerof(rn);
		cmp2 = cmpkey_cb((void *)getkey_cb(root->data), getkey_cb(node->data));
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
		return (root && root->rbtree.red) ? lh : lh + 1;
	else
		return 0;
}

void t_rbtree_keyin_fast_add(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	test_t *t = test_new("key", 4);
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb) test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb) test_getkey;
	gds_free_cb free_cb = (gds_free_cb) test_free;

	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(NULL, NULL, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(NULL, NULL, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(NULL, NULL, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(NULL, t, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(NULL, t, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(NULL, t, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(NULL, t, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(&root, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(&root, NULL, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(&root, NULL, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(&root, t, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(&root, t, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_add(&root, t, getkey_cb, NULL));

	/* Tree is unchanged */
	CU_ASSERT(NULL == root);
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));

	/* Add NULL as data, first call should success, others must return a
	 * positive value, meaning that data is already in the tree */
	CU_ASSERT(0 == gds_rbtree_keyin_fast_add(&root, NULL, getkey_cb, cmpkey_cb));
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(0 < gds_rbtree_keyin_fast_add(&root, NULL, getkey_cb, cmpkey_cb));
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));

	/* Add t as data, first call should success, others must return a
	 * positive value, meaning that data is already in the tree */
	CU_ASSERT(0 == gds_rbtree_keyin_fast_add(&root, t, getkey_cb, cmpkey_cb));
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(0 < gds_rbtree_keyin_fast_add(&root, t, getkey_cb, cmpkey_cb));
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));

	gds_rbtree_keyin_fast_free(root, NULL);
	test_free(t);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 == gds_rbtree_keyin_fast_add(&root, test_new(buf, i),
			getkey_cb, cmpkey_cb));
		CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	}

	gds_rbtree_keyin_fast_free(root, free_cb);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 == gds_rbtree_keyin_fast_add(&root, test_new(buf, i),
			getkey_cb, cmpkey_cb));
		CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	}

	gds_rbtree_keyin_fast_free(root, free_cb);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %d", j);
		CU_ASSERT(0 <= gds_rbtree_keyin_fast_add(&root, test_new(buf, j),
			getkey_cb, cmpkey_cb));
		CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	}

	gds_rbtree_keyin_fast_free(root, free_cb);
}

void t_rbtree_keyin_fast_set(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	test_t *t = test_new("key", 4);
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb) test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb) test_getkey;
	gds_free_cb free_cb = (gds_free_cb) test_free;

	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , NULL, NULL     , NULL     , NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , NULL, NULL     , NULL     , free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , NULL, NULL     , cmpkey_cb, NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , NULL, NULL     , cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , NULL, getkey_cb, NULL     , NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , NULL, getkey_cb, NULL     , free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , NULL, getkey_cb, cmpkey_cb, NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , NULL, getkey_cb, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , t   , NULL     , NULL     , NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , t   , NULL     , NULL     , free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , t   , NULL     , cmpkey_cb, NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , t   , NULL     , cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , t   , getkey_cb, NULL     , NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , t   , getkey_cb, NULL     , free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , t   , getkey_cb, cmpkey_cb, NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(NULL , t   , getkey_cb, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, NULL, NULL     , NULL     , NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, NULL, NULL     , NULL     , free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, NULL, NULL     , cmpkey_cb, NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, NULL, NULL     , cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, NULL, getkey_cb, NULL     , NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, NULL, getkey_cb, NULL     , free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, t   , NULL     , NULL     , NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, t   , NULL     , NULL     , free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, t   , NULL     , cmpkey_cb, NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, t   , NULL     , cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, t   , getkey_cb, NULL     , NULL   ));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_set(&root, t   , getkey_cb, NULL     , free_cb));

	/* Tree is unchanged */
	CU_ASSERT(NULL == root);
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));

	gds_rbtree_keyin_fast_set(&root, NULL, getkey_cb, cmpkey_cb, NULL);
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(NULL == gds_rbtree_keyin_fast_get(root, NULL, getkey_cb, cmpkey_cb));
	gds_rbtree_keyin_fast_set(&root, NULL, getkey_cb, cmpkey_cb, NULL);
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(NULL == gds_rbtree_keyin_fast_get(root, NULL, getkey_cb, cmpkey_cb));

	gds_rbtree_keyin_fast_set(&root, t, getkey_cb, cmpkey_cb, NULL);
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(t == gds_rbtree_keyin_fast_get(root, getkey_cb(t), getkey_cb, cmpkey_cb));
	gds_rbtree_keyin_fast_set(&root, t, getkey_cb, cmpkey_cb, NULL);
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(t == gds_rbtree_keyin_fast_get(root, getkey_cb(t), getkey_cb, cmpkey_cb));
	gds_rbtree_keyin_fast_set(&root, test_alloc(t), getkey_cb, cmpkey_cb, NULL);
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(t != gds_rbtree_keyin_fast_get(root, getkey_cb(t), getkey_cb, cmpkey_cb));
	gds_rbtree_keyin_fast_set(&root, test_alloc(t), getkey_cb, cmpkey_cb, free_cb);
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(t != gds_rbtree_keyin_fast_get(root, getkey_cb(t), getkey_cb, cmpkey_cb));
	gds_rbtree_keyin_fast_set(&root, t, getkey_cb, cmpkey_cb, free_cb);
	CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(t == gds_rbtree_keyin_fast_get(root, getkey_cb(t), getkey_cb, cmpkey_cb));

	gds_rbtree_keyin_fast_free(root, free_cb);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		gds_rbtree_keyin_fast_set(&root, test_new(buf, i), getkey_cb,
			cmpkey_cb, NULL);
		CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	}

	gds_rbtree_keyin_fast_free(root, free_cb);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %02d", i);
		gds_rbtree_keyin_fast_set(&root, test_new(buf, i), getkey_cb,
			cmpkey_cb, NULL);
		CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	}

	gds_rbtree_keyin_fast_free(root, free_cb);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %02d", j);
		gds_rbtree_keyin_fast_set(&root, test_new(buf, j), getkey_cb,
			cmpkey_cb, NULL);
		CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	}

	gds_rbtree_keyin_fast_free(root, free_cb);
}

void t_rbtree_keyin_fast_get(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	test_t *data;
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb) test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb) test_getkey;
	gds_free_cb free_cb = (gds_free_cb) test_free;

	gds_rbtree_keyin_fast_add(&root, test_new("key", 1), getkey_cb, cmpkey_cb);

	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, NULL, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, NULL, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, NULL, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, NULL, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, NULL, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, NULL, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, "key", NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, "key", NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, "key", NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, "key", NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, "key", getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, "key", getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, "key", getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(NULL, "key", getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, NULL, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, NULL, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, NULL, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, NULL, getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, NULL, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, NULL, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, "key", NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, "key", NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, "key", NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, "key", NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, "key", getkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_get(root, "key", getkey_cb, NULL));
	CU_ASSERT(NULL != gds_rbtree_keyin_fast_get(root, "key", getkey_cb, cmpkey_cb));
	CU_ASSERT(NULL != (data = gds_rbtree_keyin_fast_get(root, "key", getkey_cb, cmpkey_cb)));

	gds_rbtree_keyin_fast_free(root, free_cb);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_keyin_fast_add(&root, test_new(buf, i), getkey_cb, cmpkey_cb);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		data = gds_rbtree_keyin_fast_get(root, buf, getkey_cb, cmpkey_cb);
		CU_ASSERT(NULL != data);
		CU_ASSERT(i == test_getvalue(data));
		CU_ASSERT(0 == test_cmpkey(buf, test_getkey(data)));
	}
	gds_rbtree_keyin_fast_free(root, free_cb);
}

void t_rbtree_keyin_fast_del(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	test_t *t = test_new("key", 1);
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb) test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb) test_getkey;
	gds_free_cb free_cb = (gds_free_cb) test_free;

	gds_rbtree_keyin_fast_add(&root, t, getkey_cb, cmpkey_cb);

	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, NULL, NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, NULL, NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, NULL, NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, NULL, getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, NULL, getkey_cb, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, NULL, getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, NULL, getkey_cb, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, "key", NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, "key", NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, "key", NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, "key", NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, "key", getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, "key", getkey_cb, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, "key", getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(NULL, "key", getkey_cb, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, NULL, NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, NULL, NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, NULL, NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, NULL, getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, NULL, getkey_cb, NULL, free_cb));
	CU_ASSERT(0 < gds_rbtree_keyin_fast_del(&root, NULL, getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT(0 < gds_rbtree_keyin_fast_del(&root, NULL, getkey_cb, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, "key", NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, "key", NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, "key", NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, "key", NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, "key", getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_rbtree_keyin_fast_del(&root, "key", getkey_cb, NULL, free_cb));
	CU_ASSERT(0 == gds_rbtree_keyin_fast_del(&root, "key", getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT(0 < gds_rbtree_keyin_fast_del(&root, "key", getkey_cb, cmpkey_cb, free_cb));

	test_free(t);

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_keyin_fast_add(&root, test_new(buf, i), getkey_cb, cmpkey_cb);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 == gds_rbtree_keyin_fast_del(&root, buf, getkey_cb, cmpkey_cb,
			free_cb));
		CU_ASSERT(gds_rbtree_keyin_fast_is_valid(root, getkey_cb, cmpkey_cb));
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 < gds_rbtree_keyin_fast_del(&root, buf, getkey_cb, cmpkey_cb,
			free_cb));
	}
}

int main()
{
	CU_pSuite pSuite = NULL;
	int fails = 0;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Red-Black tree",
		init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if(
	   (NULL == CU_add_test(pSuite, "gds_rbtree_keyin_fast_add()", t_rbtree_keyin_fast_add))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_keyin_fast_set()", t_rbtree_keyin_fast_set))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_keyin_fast_get()", t_rbtree_keyin_fast_get))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_keyin_fast_del()", t_rbtree_keyin_fast_del))
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
