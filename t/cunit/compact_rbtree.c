#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <CUnit/Basic.h>
#include "exception.h"
#include "test_macros.h"
#include "compact_rbtree_node.h"
#include "compact_rbtree.h"
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
	((ptr) != NULL) ? (gds_compact_rbtree_node_t *)((char *)ptr \
		- offsetof(gds_compact_rbtree_node_t, rbtree)) : NULL

void gds_compact_rbtree_print(gds_compact_rbtree_node_t *root, uint8_t depth)
{
	char *key;
	gds_compact_rbtree_node_t *node;
	if(root != NULL) {
		node = containerof(root->rbtree.son[1]);
		gds_compact_rbtree_print(node, depth+1);
		for (uint8_t i=0; i<depth; i++)
			printf("- ");
		key = test_getkey(root->data);
		if (key == NULL) {
			printf("(NULL)");
		} else {
			printf("%s", key);
		}
		printf(": %d\n", test_getvalue(root->data));
		node = containerof(root->rbtree.son[0]);
		gds_compact_rbtree_print(node, depth+1);
	}
}

void gds_compact_rbtree_print_dbg_r(gds_compact_rbtree_node_t *root, uint8_t d)
{
	char *key;
	if (root != NULL) {
		gds_compact_rbtree_print_dbg_r(containerof(root->rbtree.son[1]), d+1);
		for (uint8_t i=0; i<d; i++)
			printf("- ");
		key = test_getkey(root->data);
		printf("root %p rbtree %p [%s] (0: %p, 1: %p) %s\n",
			root, &(root->rbtree), root->rbtree.red ? "red" : "black",
			root->rbtree.son[0], root->rbtree.son[1], key);
		gds_compact_rbtree_print_dbg_r(containerof(root->rbtree.son[0]), d+1);
	}
}
void gds_compact_rbtree_print_dbg(gds_compact_rbtree_node_t *root)
{
	printf("\n---------------------------------------------------------\n");
	gds_compact_rbtree_print_dbg_r(root, 0);
	printf("---------------------------------------------------------\n");
}

int8_t gds_compact_rbtree_is_valid(gds_compact_rbtree_node_t *root, gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	int lh, rh;
	int cmp = 0, cmp2 = 0;
	gds_compact_rbtree_node_t *node;

	if (root == NULL)
		return 1;

	gds_inline_compact_rbtree_node_t *ln = root->rbtree.son[0];
	gds_inline_compact_rbtree_node_t *rn = root->rbtree.son[1];

	/* Consecutive red links */
	if (root->rbtree.red) {
		if ((ln && ln->red) || (rn && rn->red)) {
			printf("Red violation\n");
			return 0;
		}
	}

	node = containerof(ln);
	lh = gds_compact_rbtree_is_valid(node, getkey_cb, cmpkey_cb);
	node = containerof(rn);
	rh = gds_compact_rbtree_is_valid(node, getkey_cb, cmpkey_cb);

	/* Invalid binary search tree */
	if(ln != NULL) {
		node = containerof(ln);
		cmp = cmpkey_cb(getkey_cb(root->data), getkey_cb(node->data));
	}
	if(rn != NULL) {
		node = containerof(rn);
		cmp2 = cmpkey_cb(getkey_cb(root->data), getkey_cb(node->data));
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

void t_compact_rbtree_add(void)
{
	gds_compact_rbtree_node_t *root = NULL;
	test_t *t = test_new("key", 4);
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb)test_getkey;
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , NULL, NULL     , NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , NULL, NULL     , NULL     , alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , NULL, NULL     , cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , NULL, NULL     , cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , NULL, getkey_cb, NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , NULL, getkey_cb, NULL     , alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , NULL, getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , NULL, getkey_cb, cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , t   , NULL     , NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , t   , NULL     , NULL     , alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , t   , NULL     , cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , t   , NULL     , cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , t   , getkey_cb, NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , t   , getkey_cb, NULL     , alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , t   , getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(NULL , t   , getkey_cb, cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, NULL, NULL     , NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, NULL, NULL     , NULL     , alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, NULL, NULL     , cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, NULL, NULL     , cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, NULL, getkey_cb, NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, NULL, getkey_cb, NULL     , alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, t   , NULL     , NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, t   , NULL     , NULL     , alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, t   , NULL     , cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, t   , NULL     , cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, t   , getkey_cb, NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_add(&root, t   , getkey_cb, NULL     , alloc_cb));

	/* Tree is unchanged */
	CU_ASSERT_PTR_NULL(root);
	CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb, cmpkey_cb));

	/* Add NULL as data, first call should success, others must return a positive value, meaning that data is already in the tree */
	CU_ASSERT(0 == gds_compact_rbtree_add(&root, NULL, getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(0 < gds_compact_rbtree_add(&root, NULL, getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(0 < gds_compact_rbtree_add(&root, NULL, getkey_cb, cmpkey_cb, alloc_cb));
	CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb, cmpkey_cb));

	/* Add t as data, first call should success, others must return a positive value, meaning that data is already in the tree */
	CU_ASSERT(0 == gds_compact_rbtree_add(&root, t, getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(0 < gds_compact_rbtree_add(&root, t, getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb, cmpkey_cb));
	CU_ASSERT(0 < gds_compact_rbtree_add(&root, t, getkey_cb, cmpkey_cb, alloc_cb));
	CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb, cmpkey_cb));

	gds_compact_rbtree_free(root, NULL);
	test_free(t);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		CU_ASSERT(0 == gds_compact_rbtree_add(&root, test_new(buf, i),
			getkey_cb, cmpkey_cb, NULL));
		CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb,
			cmpkey_cb));
	}

	gds_compact_rbtree_free(root, free_cb);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %02d", i);
		CU_ASSERT(0 == gds_compact_rbtree_add(&root, test_new(buf, i),
			getkey_cb, cmpkey_cb, NULL));
		CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb,
			cmpkey_cb));
	}

	gds_compact_rbtree_free(root, free_cb);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %02d", j);
		CU_ASSERT(0 <= gds_compact_rbtree_add(&root, test_new(buf, j),
			getkey_cb, cmpkey_cb, NULL));
		CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb,
			cmpkey_cb));
	}

	gds_compact_rbtree_free(root, free_cb);
}

void t_compact_rbtree_get(void)
{
	gds_compact_rbtree_node_t *root = NULL;
	test_t *data;
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb)test_getkey;
	gds_alloc_cb alloc_cb = (gds_alloc_cb)test_alloc;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	gds_compact_rbtree_add(&root, test_new("key", 1),
		getkey_cb, cmpkey_cb, NULL);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, NULL, NULL, NULL, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, NULL, NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, NULL, NULL, cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, NULL, getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, NULL, getkey_cb, NULL, alloc_cb));
	CU_ASSERT_PTR_NULL(gds_compact_rbtree_get(NULL, NULL, getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT_PTR_NULL(gds_compact_rbtree_get(NULL, NULL, getkey_cb, cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, "key", NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, "key", NULL, NULL, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, "key", NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, "key", NULL, cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, "key", getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(NULL, "key", getkey_cb, NULL, alloc_cb));
	CU_ASSERT_PTR_NULL(gds_compact_rbtree_get(NULL, "key", getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT_PTR_NULL(gds_compact_rbtree_get(NULL, "key", getkey_cb, cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, NULL, NULL, NULL, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, NULL, NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, NULL, NULL, cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, NULL, getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, NULL, getkey_cb, NULL, alloc_cb));
	CU_ASSERT_PTR_NULL(gds_compact_rbtree_get(root, NULL, getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT_PTR_NULL(gds_compact_rbtree_get(root, NULL, getkey_cb, cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, "key", NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, "key", NULL, NULL, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, "key", NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, "key", NULL, cmpkey_cb, alloc_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, "key", getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_get(root, "key", getkey_cb, NULL, alloc_cb));

	CU_ASSERT_PTR_NOT_NULL(gds_compact_rbtree_get(root, "key", getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT_PTR_NOT_NULL((data = gds_compact_rbtree_get(root, "key", getkey_cb, cmpkey_cb, alloc_cb)));
	test_free(data);

	gds_compact_rbtree_free(root, free_cb);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_compact_rbtree_add(&root, test_new(buf, i),
			getkey_cb, cmpkey_cb, NULL);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		data = gds_compact_rbtree_get(root, buf,
			getkey_cb, cmpkey_cb, NULL);
		CU_ASSERT_PTR_NOT_NULL(data);
		CU_ASSERT_EQUAL(i, test_getvalue(data));
		CU_ASSERT(0 == test_cmpkey(buf, test_getkey(data)));
	}
	gds_compact_rbtree_free(root, free_cb);
}

void t_compact_rbtree_del(void)
{
	gds_compact_rbtree_node_t *root = NULL;
	test_t *t = test_new("key", 1);
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb)test_getkey;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	gds_compact_rbtree_add(&root, t, getkey_cb, cmpkey_cb, NULL);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, NULL, NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, NULL, NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, NULL, NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, NULL, getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, NULL, getkey_cb, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, NULL, getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, NULL, getkey_cb, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, "key", NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, "key", NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, "key", NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, "key", NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, "key", getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, "key", getkey_cb, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, "key", getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(NULL, "key", getkey_cb, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, NULL, NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, NULL, NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, NULL, NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, NULL, getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, NULL, getkey_cb, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, "key", NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, "key", NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, "key", NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, "key", NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, "key", getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_compact_rbtree_del(&root, "key", getkey_cb, NULL, free_cb));

	CU_ASSERT(0 < gds_compact_rbtree_del(&root, NULL, getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT(0 < gds_compact_rbtree_del(&root, NULL, getkey_cb, cmpkey_cb, free_cb));
	CU_ASSERT(0 == gds_compact_rbtree_del(&root, "key", getkey_cb, cmpkey_cb, NULL));
	CU_ASSERT(0 < gds_compact_rbtree_del(&root, "key", getkey_cb, cmpkey_cb, free_cb));

	test_free(t);

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_compact_rbtree_add(&root, test_new(buf, i), getkey_cb,
			cmpkey_cb, NULL);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 == gds_compact_rbtree_del(&root, buf, getkey_cb,
			cmpkey_cb, free_cb));
		CU_ASSERT(gds_compact_rbtree_is_valid(root, getkey_cb,
			cmpkey_cb));
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		CU_ASSERT(0 < gds_compact_rbtree_del(&root, buf, getkey_cb,
			cmpkey_cb, free_cb));
	}
}

void t_compact_rbtree_values(void)
{
	gds_compact_rbtree_node_t *root = NULL;
	gds_slist_node_t *slist;
	gds_iterator_t *it;
	test_t *t;
	char buf[512];
	int i;
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb)test_getkey;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	for(i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		t = test_new(buf, i);
		gds_compact_rbtree_add(&root, t, getkey_cb, cmpkey_cb, NULL);
	}

	slist = gds_compact_rbtree_values(root, NULL);
	CU_ASSERT_PTR_NOT_NULL(slist);
	it = gds_slist_iterator_new(slist);
	i = 0;
	while(gds_iterator_step(it) == 0) {
		t = gds_iterator_get(it);
		CU_ASSERT_PTR_NOT_NULL(t);
		sprintf(buf, "key %02d", i);
		CU_ASSERT_STRING_EQUAL(t->key, buf);
		CU_ASSERT_EQUAL(i, t->value);
		i++;
	}
	gds_slist_iterator_free(it);
	gds_slist_free(slist, NULL);
	gds_compact_rbtree_free(root, free_cb);
}


int main()
{
	CU_pSuite pSuite = NULL;

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
	   (NULL == CU_add_test(pSuite, "gds_compact_rbtree_add()", t_compact_rbtree_add))
	|| (NULL == CU_add_test(pSuite, "gds_compact_rbtree_get()", t_compact_rbtree_get))
	|| (NULL == CU_add_test(pSuite, "gds_compact_rbtree_del()", t_compact_rbtree_del))
	|| (NULL == CU_add_test(pSuite, "gds_compact_rbtree_values()", t_compact_rbtree_values))
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
	CU_cleanup_registry();
	return CU_get_error();
}
