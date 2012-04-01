#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <CUnit/Basic.h>
#include "rbtree.h"
#include "test.h"

int init_suite(void)
{
	return 0;
}

int clean_suite(void)
{
	return 0;
}

void gds_rbtree_print(gds_rbtree_node_t *root, uint8_t depth)
{
	char *key;
	if(root != NULL) {
		gds_rbtree_print(root->right, depth+1);
		for (uint8_t i=0; i<depth; i++)
			printf("- ");
		key = test_getkey(root->data);
		if (key == NULL) {
			printf("(NULL)");
		} else {
			printf("%s", key);
		}
		printf(": %d\n", test_getvalue(root->data));
		gds_rbtree_print(root->left, depth+1);
	}
}

int8_t gds_rbtree_is_valid(gds_rbtree_node_t *root, gds_func_ptr_t getkey_f,
	gds_func_ptr_t cmpkey_f)
{
	int lh, rh;
	int cmp = 0, cmp2 = 0;

	if ( root == NULL )
		return 1;

	gds_rbtree_node_t *ln = root->left;
	gds_rbtree_node_t *rn = root->right;

	/* Consecutive red links */
	if (gds_rbtree_node_is_red(root)) {
		if (gds_rbtree_node_is_red(ln)
		|| gds_rbtree_node_is_red(rn)) {
			printf("Red violation\n");
			return 0;
		}
	}

	lh = gds_rbtree_is_valid(ln, getkey_f, cmpkey_f);
	rh = gds_rbtree_is_valid(rn, getkey_f, cmpkey_f);

	/* Invalid binary search tree */
	if(ln != NULL)
		cmp = cmpkey_f((void *)getkey_f(root->data), getkey_f(ln->data));
	if(rn != NULL)
		cmp2 = cmpkey_f((void *)getkey_f(root->data), getkey_f(rn->data));
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
		return gds_rbtree_node_is_red(root) ? lh : lh + 1;
	else
		return 0;
}

void t_rbtree_add(void)
{
	gds_rbtree_node_t *root = NULL;
	test_t *t = test_new("key", 4);
	char buf[512];

	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, NULL, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, NULL, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, NULL, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, NULL, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, NULL, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, NULL, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, NULL, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, NULL, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(NULL, t, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, NULL, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, NULL, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, NULL, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, NULL, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, NULL, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, NULL, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, NULL, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, NULL, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(0 > gds_rbtree_add(&root, t, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));

	/* Tree is unchanged */
	CU_ASSERT(NULL == root);
	CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey));

	/* Add NULL as data, first call should success, others must return a positive value, meaning that data is already in the tree */
	CU_ASSERT(0 == gds_rbtree_add(&root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey));
	CU_ASSERT(0 < gds_rbtree_add(&root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey));
	CU_ASSERT(0 < gds_rbtree_add(&root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey));

	/* Add t as data, first call should success, others must return a positive value, meaning that data is already in the tree */
	CU_ASSERT(0 == gds_rbtree_add(&root, t, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey));
	CU_ASSERT(0 < gds_rbtree_add(&root, t, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey));
	CU_ASSERT(0 < gds_rbtree_add(&root, t, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey));

	gds_rbtree_free(root, false, NULL);
	test_free(t);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 == gds_rbtree_add(&root, test_new(buf, i),
			(gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey, false, NULL));
		CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey));
	}

	gds_rbtree_free(root, true, (gds_func_ptr_t)test_free);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 == gds_rbtree_add(&root, test_new(buf, i),
			(gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey, false, NULL));
		CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey));
	}

	gds_rbtree_free(root, true, (gds_func_ptr_t)test_free);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %d", j);
		CU_ASSERT(0 <= gds_rbtree_add(&root, test_new(buf, j),
			(gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey, false, NULL));
		CU_ASSERT(gds_rbtree_is_valid(root, (gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey));
	}

	gds_rbtree_free(root, true, (gds_func_ptr_t)test_free);
}

void t_rbtree_get(void)
{
	gds_rbtree_node_t *root = NULL;
	test_t *data;
	char buf[512];

	gds_rbtree_add(&root, test_new("key", 1),
		(gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey,
		false, NULL);

	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, NULL, NULL, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, NULL, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, NULL, NULL, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, NULL, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", NULL, NULL, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", NULL, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", NULL, NULL, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", NULL, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(NULL, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, NULL, NULL, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, NULL, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, NULL, NULL, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, NULL, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", NULL, NULL, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", NULL, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", NULL, NULL, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", NULL, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL != gds_rbtree_get(root, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(NULL != gds_rbtree_get(root, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_alloc));
	CU_ASSERT(NULL == gds_rbtree_get(root, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(NULL != (data = gds_rbtree_get(root, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_alloc)));
	test_free(data);

	gds_rbtree_free(root, true, (gds_func_ptr_t)test_free);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_add(&root, test_new(buf, i),
			(gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey, false, NULL);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		data = gds_rbtree_get(root, buf,
			(gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey, false, NULL);
		CU_ASSERT(NULL != data);
		CU_ASSERT(i == test_getvalue(data));
		CU_ASSERT(0 == test_cmpkey(buf, test_getkey(data)));
	}
	gds_rbtree_free(root, true, (gds_func_ptr_t)test_free);
}

void t_rbtree_del(void)
{
	gds_rbtree_node_t *root = NULL;
	test_t *t = test_new("key", 1);
	char buf[512];

	gds_rbtree_add(&root, t, (gds_func_ptr_t)test_getkey,
		(gds_func_ptr_t)test_cmpkey, false, NULL);

	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, NULL, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, NULL, NULL, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, NULL, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, NULL, NULL, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", NULL, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", NULL, NULL, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", NULL, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", NULL, NULL, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(NULL, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, NULL, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, NULL, NULL, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, NULL, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, NULL, NULL, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 < gds_rbtree_del(&root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 < gds_rbtree_del(&root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 < gds_rbtree_del(&root, NULL, (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", NULL, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", NULL, NULL, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", NULL, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", NULL, NULL, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", NULL, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", NULL, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", NULL, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", NULL, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", (gds_func_ptr_t)test_getkey, NULL, false, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", (gds_func_ptr_t)test_getkey, NULL, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", (gds_func_ptr_t)test_getkey, NULL, true, NULL));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", (gds_func_ptr_t)test_getkey, NULL, true, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 == gds_rbtree_del(&root, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, NULL));
	CU_ASSERT(0 < gds_rbtree_del(&root, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, false, (gds_func_ptr_t)test_free));
	CU_ASSERT(0 > gds_rbtree_del(&root, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, NULL));
	CU_ASSERT(0 < gds_rbtree_del(&root, "key", (gds_func_ptr_t)test_getkey, (gds_func_ptr_t)test_cmpkey, true, (gds_func_ptr_t)test_free));

	test_free(t);

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_add(&root, test_new(buf, i),
			(gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey, false, NULL);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 == gds_rbtree_del(&root, buf,
			(gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey, true,
			(gds_func_ptr_t)test_free));
		CU_ASSERT(gds_rbtree_is_valid(root,
			(gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey));
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 < gds_rbtree_del(&root, buf,
			(gds_func_ptr_t)test_getkey,
			(gds_func_ptr_t)test_cmpkey, true,
			(gds_func_ptr_t)test_free));
	}
}

int main()
{
	CU_pSuite pSuite = NULL;

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
	   (NULL == CU_add_test(pSuite, "gds_rbtree_add()", t_rbtree_add))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_get()", t_rbtree_get))
	|| (NULL == CU_add_test(pSuite, "gds_rbtree_del()", t_rbtree_del))
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
