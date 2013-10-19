#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "test_macros.h"
#include "rbtree_keyin.h"
#include "iterator.h"
#include "slist.h"
#include "test.h"
#include "tap.h"

int init_suite(void)
{
	return 0;
}

int clean_suite(void)
{
	return 0;
}

#define containerof(ptr) \
	((ptr) != NULL) ? (gds_rbtree_keyin_node_t *)((char *)ptr \
		- offsetof(gds_rbtree_keyin_node_t, rbtree)) : NULL

void gds_rbtree_keyin_print(gds_rbtree_keyin_node_t *root, int depth)
{
	char *key;
	gds_rbtree_keyin_node_t *node;
	if(root != NULL) {
		node = containerof(root->rbtree.son[1]);
		gds_rbtree_keyin_print(node, depth+1);
		for (int i=0; i<depth; i++)
			printf("- ");
		key = test_getkey(root->data);
		if (key == NULL) {
			printf("(NULL)");
		} else {
			printf("%s", key);
		}
		printf(": %d\n", test_getvalue(root->data));
		node = containerof(root->rbtree.son[0]);
		gds_rbtree_keyin_print(node, depth+1);
	}
}

void gds_rbtree_keyin_print_dbg_r(gds_rbtree_keyin_node_t *root, int d)
{
	char *key;
	if (root != NULL) {
		gds_rbtree_keyin_print_dbg_r(containerof(root->rbtree.son[1]), d+1);
		for (int i=0; i<d; i++)
			printf("- ");
		key = test_getkey(root->data);
		printf("root %p rbtree_keyin %p [%s] (0: %p, 1: %p) %s\n",
			root, &(root->rbtree), root->rbtree.red ? "red" : "black",
			root->rbtree.son[0], root->rbtree.son[1], key);
		gds_rbtree_keyin_print_dbg_r(containerof(root->rbtree.son[0]), d+1);
	}
}
void gds_rbtree_keyin_print_dbg(gds_rbtree_keyin_node_t *root)
{
	printf("\n---------------------------------------------------------\n");
	gds_rbtree_keyin_print_dbg_r(root, 0);
	printf("---------------------------------------------------------\n");
}

int gds_rbtree_keyin_is_valid(gds_rbtree_keyin_node_t *root, gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	int lh, rh;
	int cmp = 0, cmp2 = 0;
	gds_rbtree_keyin_node_t *node;

	if (root == NULL)
		return 1;

	gds_inline_rbtree_node_t *ln = root->rbtree.son[0];
	gds_inline_rbtree_node_t *rn = root->rbtree.son[1];

	/* Consecutive red links */
	if (root->rbtree.red) {
		if ((ln && ln->red) || (rn && rn->red)) {
			printf("Red violation\n");
			return 0;
		}
	}

	node = containerof(ln);
	lh = gds_rbtree_keyin_is_valid(node, getkey_cb, cmpkey_cb);
	node = containerof(rn);
	rh = gds_rbtree_keyin_is_valid(node, getkey_cb, cmpkey_cb);

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

void t_rbtree_keyin_add(void)
{
	gds_rbtree_keyin_node_t *root = NULL;
	test_t *t = test_new("key", 4);
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb)test_getkey;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(NULL , NULL, NULL     , NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(NULL , NULL, NULL     , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(NULL , NULL, getkey_cb, NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(NULL , NULL, getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(NULL , t   , NULL     , NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(NULL , t   , NULL     , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(NULL , t   , getkey_cb, NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(NULL , t   , getkey_cb, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(&root, NULL, NULL     , NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(&root, NULL, NULL     , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(&root, NULL, getkey_cb, NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(&root, t   , NULL     , NULL     ));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(&root, t   , NULL     , cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_add(&root, t   , getkey_cb, NULL     ));

	/* Tree is unchanged */
	isnull(root, NULL);
	ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);

	/* Add NULL as data, first call should success, others must return a
	 * positive value, meaning that data is already in the tree */
	ok(0 == gds_rbtree_keyin_add(&root, NULL, getkey_cb, cmpkey_cb), NULL);
	ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);
	ok(0 < gds_rbtree_keyin_add(&root, NULL, getkey_cb, cmpkey_cb), NULL);
	ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);

	/* Add t as data, first call should success, others must return a
	 * positive value, meaning that data is already in the tree */
	ok(0 == gds_rbtree_keyin_add(&root, t, getkey_cb, cmpkey_cb), NULL);
	ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);
	ok(0 < gds_rbtree_keyin_add(&root, t, getkey_cb, cmpkey_cb), NULL);
	ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);

	gds_rbtree_keyin_free(root, NULL);
	test_free(t);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		ok(0 == gds_rbtree_keyin_add(&root, test_new(buf, i),
			getkey_cb, cmpkey_cb), NULL);
		ok(gds_rbtree_keyin_is_valid(root, getkey_cb,
			cmpkey_cb), NULL);
	}

	gds_rbtree_keyin_free(root, free_cb);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %02d", i);
		ok(0 == gds_rbtree_keyin_add(&root, test_new(buf, i),
			getkey_cb, cmpkey_cb), NULL);
		ok(gds_rbtree_keyin_is_valid(root, getkey_cb,
			cmpkey_cb), NULL);
	}

	gds_rbtree_keyin_free(root, free_cb);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %02d", j);
		ok(0 <= gds_rbtree_keyin_add(&root, test_new(buf, j),
			getkey_cb, cmpkey_cb), NULL);
		ok(gds_rbtree_keyin_is_valid(root, getkey_cb,
			cmpkey_cb), NULL);
	}

	gds_rbtree_keyin_free(root, free_cb);
}

void t_rbtree_keyin_set(void)
{
	gds_rbtree_keyin_node_t *root = NULL;
	test_t *t = test_new("string", 4);
	char buf[512];
	gds_getkey_cb getkey_cb = (gds_getkey_cb)test_getkey;
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , NULL, getkey_cb, NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , NULL, getkey_cb, NULL     , free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , NULL, getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , NULL, getkey_cb, cmpkey_cb, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , NULL, NULL     , NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , NULL, NULL     , NULL     , free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , NULL, NULL     , cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , NULL, NULL     , cmpkey_cb, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , t   , getkey_cb, NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , t   , getkey_cb, NULL     , free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , t   , getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , t   , getkey_cb, cmpkey_cb, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , t   , NULL     , NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , t   , NULL     , NULL     , free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , t   , NULL     , cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(NULL , t   , NULL     , cmpkey_cb, free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(&root, NULL, getkey_cb, NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(&root, NULL, getkey_cb, NULL     , free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(&root, NULL, getkey_cb, NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(&root, NULL, getkey_cb, NULL     , free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(&root, t   , NULL     , NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(&root, t   , NULL     , NULL     , free));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(&root, t   , NULL     , NULL     , NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_set(&root, t   , NULL     , NULL     , free));

	/* Tree is unchanged */
	isnull(root, NULL);
	ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);

	/* Add NULL as data, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	ok(0 == gds_rbtree_keyin_set(&root, NULL, getkey_cb, cmpkey_cb, NULL), NULL);
	ok(0 <  gds_rbtree_keyin_set(&root, NULL, getkey_cb, cmpkey_cb, NULL), NULL);
	ok(0 <  gds_rbtree_keyin_set(&root, NULL, getkey_cb, cmpkey_cb, NULL), NULL);

	ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);

	/* Add t as data, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	ok(0 == gds_rbtree_keyin_set(&root, t, getkey_cb, cmpkey_cb, NULL), NULL);
	ok(0 <  gds_rbtree_keyin_set(&root, t, getkey_cb, cmpkey_cb, NULL), NULL);

	ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);

	gds_rbtree_keyin_free(root, NULL);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		ok(0 == gds_rbtree_keyin_set(&root, test_new(buf, i), getkey_cb, cmpkey_cb, NULL), NULL);
		ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);
	}

	gds_rbtree_keyin_free(root, free_cb);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %02d", i);
		ok(0 == gds_rbtree_keyin_set(&root, test_new(buf, i), getkey_cb, cmpkey_cb, NULL), NULL);
		ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);
	}

	gds_rbtree_keyin_free(root, free_cb);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %02d", j);
		ok(0 <= gds_rbtree_keyin_set(&root, test_new(buf, i), getkey_cb, cmpkey_cb, free_cb), NULL);
		ok(gds_rbtree_keyin_is_valid(root, getkey_cb, cmpkey_cb), NULL);
	}

	gds_rbtree_keyin_free(root, free_cb);

	test_free(t);
}

void t_rbtree_keyin_get(void)
{
	gds_rbtree_keyin_node_t *root = NULL;
	test_t *data;
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb)test_getkey;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	gds_rbtree_keyin_add(&root, test_new("key", 1),
		getkey_cb, cmpkey_cb);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(NULL, NULL, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(NULL, NULL, getkey_cb, NULL));
	isnull(gds_rbtree_keyin_get(NULL, NULL, getkey_cb, cmpkey_cb), NULL);
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(NULL, "key", NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(NULL, "key", NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(NULL, "key", getkey_cb, NULL));
	isnull(gds_rbtree_keyin_get(NULL, "key", getkey_cb, cmpkey_cb), NULL);
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(root, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(root, NULL, NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(root, NULL, getkey_cb, NULL));
	isnull(gds_rbtree_keyin_get(root, NULL, getkey_cb, cmpkey_cb), NULL);
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(root, "key", NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(root, "key", NULL, cmpkey_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_get(root, "key", getkey_cb, NULL));

	isntnull(gds_rbtree_keyin_get(root, "key", getkey_cb, cmpkey_cb), NULL);
	isntnull((data = gds_rbtree_keyin_get(root, "key", getkey_cb, cmpkey_cb)), NULL);

	gds_rbtree_keyin_free(root, free_cb);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_keyin_add(&root, test_new(buf, i),
			getkey_cb, cmpkey_cb);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		data = gds_rbtree_keyin_get(root, buf,
			getkey_cb, cmpkey_cb);
		isntnull(data, NULL);
		is(i, test_getvalue(data), NULL);
		ok(0 == test_cmpkey(buf, test_getkey(data)), NULL);
	}
	gds_rbtree_keyin_free(root, free_cb);
}

void t_rbtree_keyin_del(void)
{
	gds_rbtree_keyin_node_t *root = NULL;
	test_t *t = test_new("key", 1);
	char buf[512];
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb)test_getkey;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	gds_rbtree_keyin_add(&root, t, getkey_cb, cmpkey_cb);

	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, NULL, NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, NULL, NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, NULL, NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, NULL, getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, NULL, getkey_cb, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, NULL, getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, NULL, getkey_cb, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, "key", NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, "key", NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, "key", NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, "key", NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, "key", getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, "key", getkey_cb, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, "key", getkey_cb, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(NULL, "key", getkey_cb, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, NULL, NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, NULL, NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, NULL, NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, NULL, NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, NULL, getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, NULL, getkey_cb, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, "key", NULL, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, "key", NULL, NULL, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, "key", NULL, cmpkey_cb, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, "key", NULL, cmpkey_cb, free_cb));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, "key", getkey_cb, NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException,
		gds_rbtree_keyin_del(&root, "key", getkey_cb, NULL, free_cb));

	ok(0 < gds_rbtree_keyin_del(&root, NULL, getkey_cb, cmpkey_cb, NULL), NULL);
	ok(0 < gds_rbtree_keyin_del(&root, NULL, getkey_cb, cmpkey_cb, free_cb), NULL);
	ok(0 == gds_rbtree_keyin_del(&root, "key", getkey_cb, cmpkey_cb, NULL), NULL);
	ok(0 < gds_rbtree_keyin_del(&root, "key", getkey_cb, cmpkey_cb, free_cb), NULL);

	test_free(t);

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_keyin_add(&root, test_new(buf, i), getkey_cb,
			cmpkey_cb);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		ok(0 == gds_rbtree_keyin_del(&root, buf, getkey_cb,
			cmpkey_cb, free_cb), NULL);
		ok(gds_rbtree_keyin_is_valid(root, getkey_cb,
			cmpkey_cb), NULL);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		ok(0 < gds_rbtree_keyin_del(&root, buf, getkey_cb,
			cmpkey_cb, free_cb), NULL);
	}
}

void t_rbtree_keyin_iterator(void)
{
	gds_rbtree_keyin_node_t *root = NULL;
	gds_iterator_t *it;
	test_t *t;
	char buf[512];
	int i;
	const char *key;
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb)test_cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb)test_getkey;
	gds_free_cb free_cb = (gds_free_cb)test_free;

	for(i=0; i<5; i++) {
		sprintf(buf, "key %02d", i);
		t = test_new(buf, i);
		gds_rbtree_keyin_add(&root, t, getkey_cb, cmpkey_cb);
	}

	it = gds_rbtree_keyin_iterator_new(root, getkey_cb);
	isntnull(it, NULL);

	isnull(gds_iterator_get(it), NULL);
	isnull(gds_iterator_getkey(it), NULL);

	ok(0 == gds_iterator_step(it), NULL);
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isntnull(t, NULL);
	isntnull(key, NULL);
	str_eq(key, "key 00", NULL);

	ok(0 == gds_iterator_step(it), NULL);
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isntnull(t, NULL);
	isntnull(key, NULL);
	str_eq(key, "key 01", NULL);

	ok(0 == gds_iterator_step(it), NULL);
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isntnull(t, NULL);
	isntnull(key, NULL);
	str_eq(key, "key 02", NULL);

	ok(0 == gds_iterator_step(it), NULL);
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isntnull(t, NULL);
	isntnull(key, NULL);
	str_eq(key, "key 03", NULL);

	ok(0 == gds_iterator_step(it), NULL);
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isntnull(t, NULL);
	isntnull(key, NULL);
	str_eq(key, "key 04", NULL);

	ok(0 < gds_iterator_step(it), NULL);

	gds_iterator_free(it);
	gds_rbtree_keyin_free(root, free_cb);
}

void t_rbtree_keyin_values(void)
{
	gds_rbtree_keyin_node_t *root = NULL;
	gds_slist_t *slist;
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
		gds_rbtree_keyin_add(&root, t, getkey_cb, cmpkey_cb);
	}

	slist = gds_rbtree_keyin_values(root);
	isntnull(slist, NULL);
	it = gds_slist_iterator_new(slist);
	i = 0;
	while(gds_iterator_step(it) == 0) {
		t = gds_iterator_get(it);
		isntnull(t, NULL);
		sprintf(buf, "key %02d", i);
		str_eq(t->key, buf, NULL);
		is(i, t->value, NULL);
		i++;
	}
	gds_iterator_free(it);
	gds_slist_free(slist, NULL, NULL);
	gds_rbtree_keyin_free(root, free_cb);
}


int main()
{
	plan(2231);

	t_rbtree_keyin_add();
	t_rbtree_keyin_set();
	t_rbtree_keyin_get();
	t_rbtree_keyin_del();
	t_rbtree_keyin_iterator();
	t_rbtree_keyin_values();

	return EXIT_SUCCESS;
}
