#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libtap13/tap.h>
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

void gds_rbtree_keyin_fast_print(gds_rbtree_keyin_fast_node_t *root, int depth)
{
	char *key;
	gds_rbtree_keyin_fast_node_t *node;
	if(root != NULL) {
		node = containerof(root->rbtree.right);
		gds_rbtree_keyin_fast_print(node, depth+1);
		for (int i=0; i<depth; i++)
			printf("- ");
		key = test_getkey(root->data);
		if (key == NULL) {
			printf("(NULL)");
		} else {
			printf("%s", key);
		}
		printf(": %d", test_getvalue(root->data));
		printf(" (%p)\n", root);
		node = containerof(root->rbtree.left);
		gds_rbtree_keyin_fast_print(node, depth+1);
	}
}

int gds_rbtree_keyin_fast_is_valid(gds_rbtree_keyin_fast_node_t *root,
	void *test_getkey, void *test_cmpkey)
{
	int lh, rh;
	int cmp = 0, cmp2 = 0;
	gds_rbtree_keyin_fast_node_t *node;
	void * (*getkey_callback)(void *) = test_getkey;
	int (*cmpkey_callback)(void *, void *) = test_cmpkey;

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
	lh = gds_rbtree_keyin_fast_is_valid(node, test_getkey, test_cmpkey);
	node = containerof(rn);
	rh = gds_rbtree_keyin_fast_is_valid(node, test_getkey, test_cmpkey);

	/* Invalid binary search tree */
	if(ln != NULL) {
		node = containerof(ln);
		cmp = cmpkey_callback(getkey_callback(root->data), getkey_callback(node->data));
	}
	if(rn != NULL) {
		node = containerof(rn);
		cmp2 = cmpkey_callback(getkey_callback(root->data), getkey_callback(node->data));
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

	ok(0 > gds_rbtree_keyin_fast_add(NULL, NULL, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_add(NULL, NULL, NULL, test_cmpkey));
	ok(0 > gds_rbtree_keyin_fast_add(NULL, NULL, test_getkey, NULL));
	ok(0 > gds_rbtree_keyin_fast_add(NULL, NULL, test_getkey, test_cmpkey));
	ok(0 > gds_rbtree_keyin_fast_add(NULL, t, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_add(NULL, t, NULL, test_cmpkey));
	ok(0 > gds_rbtree_keyin_fast_add(NULL, t, test_getkey, NULL));
	ok(0 > gds_rbtree_keyin_fast_add(NULL, t, test_getkey, test_cmpkey));
	ok(0 > gds_rbtree_keyin_fast_add(&root, NULL, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_add(&root, NULL, NULL, test_cmpkey));
	ok(0 > gds_rbtree_keyin_fast_add(&root, NULL, test_getkey, NULL));
	ok(0 > gds_rbtree_keyin_fast_add(&root, t, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_add(&root, t, NULL, test_cmpkey));
	ok(0 > gds_rbtree_keyin_fast_add(&root, t, test_getkey, NULL));

	/* Tree is unchanged */
	ok(NULL == root);
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));

	/* Add NULL as data, first call should success, others must return a
	 * positive value, meaning that data is already in the tree */
	ok(0 == gds_rbtree_keyin_fast_add(&root, NULL, test_getkey, test_cmpkey));
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	ok(0 < gds_rbtree_keyin_fast_add(&root, NULL, test_getkey, test_cmpkey));
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));

	/* Add t as data, first call should success, others must return a
	 * positive value, meaning that data is already in the tree */
	ok(0 == gds_rbtree_keyin_fast_add(&root, t, test_getkey, test_cmpkey));
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	ok(0 < gds_rbtree_keyin_fast_add(&root, t, test_getkey, test_cmpkey));
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));

	gds_rbtree_keyin_fast_free(root, NULL);
	test_free(t);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		ok(0 == gds_rbtree_keyin_fast_add(&root, test_new(buf, i),
			test_getkey, test_cmpkey), NULL);
		ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	}

	gds_rbtree_keyin_fast_free(root, test_free);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %d", i);
		ok(0 == gds_rbtree_keyin_fast_add(&root, test_new(buf, i),
			test_getkey, test_cmpkey), NULL);
		ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	}

	gds_rbtree_keyin_fast_free(root, test_free);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %d", j);
		ok(0 <= gds_rbtree_keyin_fast_add(&root, test_new(buf, j),
			test_getkey, test_cmpkey), NULL);
		ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	}

	gds_rbtree_keyin_fast_free(root, test_free);
}

void t_rbtree_keyin_fast_set(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	test_t *t = test_new("key", 4);
	char buf[512];
	int rc;

	ok(0 > gds_rbtree_keyin_fast_set(NULL , NULL, NULL     , NULL     , NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , NULL, NULL     , NULL     , test_free));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , NULL, NULL     , test_cmpkey, NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , NULL, NULL     , test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , NULL, test_getkey, NULL     , NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , NULL, test_getkey, NULL     , test_free));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , NULL, test_getkey, test_cmpkey, NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , NULL, test_getkey, test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , t   , NULL     , NULL     , NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , t   , NULL     , NULL     , test_free));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , t   , NULL     , test_cmpkey, NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , t   , NULL     , test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , t   , test_getkey, NULL     , NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , t   , test_getkey, NULL     , test_free));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , t   , test_getkey, test_cmpkey, NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(NULL , t   , test_getkey, test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_set(&root, NULL, NULL     , NULL     , NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(&root, NULL, NULL     , NULL     , test_free));
	ok(0 > gds_rbtree_keyin_fast_set(&root, NULL, NULL     , test_cmpkey, NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(&root, NULL, NULL     , test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_set(&root, NULL, test_getkey, NULL     , NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(&root, NULL, test_getkey, NULL     , test_free));
	ok(0 > gds_rbtree_keyin_fast_set(&root, t   , NULL     , NULL     , NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(&root, t   , NULL     , NULL     , test_free));
	ok(0 > gds_rbtree_keyin_fast_set(&root, t   , NULL     , test_cmpkey, NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(&root, t   , NULL     , test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_set(&root, t   , test_getkey, NULL     , NULL   ));
	ok(0 > gds_rbtree_keyin_fast_set(&root, t   , test_getkey, NULL     , test_free));

	/* Tree is unchanged */
	ok(NULL == root);
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));

	rc = gds_rbtree_keyin_fast_set(&root, NULL, test_getkey, test_cmpkey, NULL);
	is(rc, 0);
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, test_getkey, test_cmpkey));
	rc = gds_rbtree_keyin_fast_set(&root, NULL, test_getkey, test_cmpkey, NULL);
	is(rc, 1);
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, test_getkey, test_cmpkey));

	rc = gds_rbtree_keyin_fast_set(&root, t, test_getkey, test_cmpkey, NULL);
	is(rc, 0);
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	ok(t == gds_rbtree_keyin_fast_get(root, test_getkey(t), test_getkey, test_cmpkey));
	rc = gds_rbtree_keyin_fast_set(&root, t, test_getkey, test_cmpkey, NULL);
	is(rc, 1);
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	ok(t == gds_rbtree_keyin_fast_get(root, test_getkey(t), test_getkey, test_cmpkey));
	rc = gds_rbtree_keyin_fast_set(&root, test_alloc(t), test_getkey, test_cmpkey, NULL);
	is(rc, 1);
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	ok(t != gds_rbtree_keyin_fast_get(root, test_getkey(t), test_getkey, test_cmpkey));
	rc = gds_rbtree_keyin_fast_set(&root, test_alloc(t), test_getkey, test_cmpkey, test_free);
	is(rc, 1);
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	ok(t != gds_rbtree_keyin_fast_get(root, test_getkey(t), test_getkey, test_cmpkey));
	rc = gds_rbtree_keyin_fast_set(&root, t, test_getkey, test_cmpkey, test_free);
	is(rc, 1);
	ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	ok(t == gds_rbtree_keyin_fast_get(root, test_getkey(t), test_getkey, test_cmpkey));

	gds_rbtree_keyin_fast_free(root, test_free);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		gds_rbtree_keyin_fast_set(&root, test_new(buf, i), test_getkey,
			test_cmpkey, NULL);
		ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	}

	gds_rbtree_keyin_fast_free(root, test_free);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %02d", i);
		gds_rbtree_keyin_fast_set(&root, test_new(buf, i), test_getkey,
			test_cmpkey, NULL);
		ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	}

	gds_rbtree_keyin_fast_free(root, test_free);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %02d", j);
		gds_rbtree_keyin_fast_set(&root, test_new(buf, j), test_getkey,
			test_cmpkey, NULL);
		ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	}

	gds_rbtree_keyin_fast_free(root, test_free);
}

void t_rbtree_keyin_fast_get(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	test_t *data;
	char buf[512];

	gds_rbtree_keyin_fast_add(&root, test_new("key", 1), test_getkey, test_cmpkey);

	ok(NULL == gds_rbtree_keyin_fast_get(NULL, NULL, NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, NULL, NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, NULL, NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, NULL, NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, NULL, test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, NULL, test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, NULL, test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, NULL, test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, "key", NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, "key", NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, "key", NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, "key", NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, "key", test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, "key", test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, "key", test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(NULL, "key", test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(root, NULL, test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(root, "key", NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(root, "key", NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(root, "key", NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(root, "key", NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_get(root, "key", test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_get(root, "key", test_getkey, NULL));
	ok(NULL != gds_rbtree_keyin_fast_get(root, "key", test_getkey, test_cmpkey));
	ok(NULL != (data = gds_rbtree_keyin_fast_get(root, "key", test_getkey, test_cmpkey)));

	gds_rbtree_keyin_fast_free(root, test_free);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_keyin_fast_add(&root, test_new(buf, i), test_getkey, test_cmpkey);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		data = gds_rbtree_keyin_fast_get(root, buf, test_getkey, test_cmpkey);
		ok(NULL != data);
		ok(i == test_getvalue(data));
		ok(0 == test_cmpkey(buf, test_getkey(data)));
	}
	gds_rbtree_keyin_fast_free(root, test_free);
}

void t_rbtree_keyin_fast_del(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	test_t *t = test_new("key", 1);
	char buf[512];

	gds_rbtree_keyin_fast_add(&root, t, test_getkey, test_cmpkey);

	ok(0 > gds_rbtree_keyin_fast_del(NULL, NULL, NULL, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, NULL, NULL, NULL, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, NULL, NULL, test_cmpkey, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, NULL, NULL, test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, NULL, test_getkey, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, NULL, test_getkey, NULL, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, NULL, test_getkey, test_cmpkey, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, NULL, test_getkey, test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, "key", NULL, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, "key", NULL, NULL, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, "key", NULL, test_cmpkey, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, "key", NULL, test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, "key", test_getkey, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, "key", test_getkey, NULL, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, "key", test_getkey, test_cmpkey, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(NULL, "key", test_getkey, test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(&root, NULL, NULL, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(&root, NULL, NULL, NULL, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(&root, NULL, NULL, test_cmpkey, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(&root, NULL, NULL, test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(&root, NULL, test_getkey, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(&root, NULL, test_getkey, NULL, test_free));
	ok(0 < gds_rbtree_keyin_fast_del(&root, NULL, test_getkey, test_cmpkey, NULL));
	ok(0 < gds_rbtree_keyin_fast_del(&root, NULL, test_getkey, test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(&root, "key", NULL, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(&root, "key", NULL, NULL, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(&root, "key", NULL, test_cmpkey, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(&root, "key", NULL, test_cmpkey, test_free));
	ok(0 > gds_rbtree_keyin_fast_del(&root, "key", test_getkey, NULL, NULL));
	ok(0 > gds_rbtree_keyin_fast_del(&root, "key", test_getkey, NULL, test_free));
	ok(0 == gds_rbtree_keyin_fast_del(&root, "key", test_getkey, test_cmpkey, NULL));
	ok(0 < gds_rbtree_keyin_fast_del(&root, "key", test_getkey, test_cmpkey, test_free));

	test_free(t);

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_keyin_fast_add(&root, test_new(buf, i), test_getkey, test_cmpkey);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		ok(0 == gds_rbtree_keyin_fast_del(&root, buf, test_getkey, test_cmpkey,
			test_free), NULL);
		ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey, test_cmpkey));
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		ok(0 < gds_rbtree_keyin_fast_del(&root, buf, test_getkey, test_cmpkey,
			test_free), NULL);
	}
}

void t_rbtree_keyin_fast_pop(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	test_t *t = test_new("key", 1);
	char buf[512];
	test_t *t2;

	gds_rbtree_keyin_fast_add(&root, t, test_getkey, test_cmpkey);

	ok(NULL == gds_rbtree_keyin_fast_pop(NULL, NULL, NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_pop(NULL, NULL, NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_pop(NULL, NULL, test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_pop(NULL, NULL, test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_pop(NULL, "key", NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_pop(NULL, "key", NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_pop(NULL, "key", test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_pop(NULL, "key", test_getkey, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_pop(&root, NULL, NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_pop(&root, NULL, NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_pop(&root, NULL, test_getkey, NULL));
	ok(NULL == gds_rbtree_keyin_fast_pop(&root, "key", NULL, NULL));
	ok(NULL == gds_rbtree_keyin_fast_pop(&root, "key", NULL, test_cmpkey));
	ok(NULL == gds_rbtree_keyin_fast_pop(&root, "key", test_getkey, NULL));

	is(gds_rbtree_keyin_fast_pop(&root, NULL, test_getkey, test_cmpkey), NULL);
	t2 = gds_rbtree_keyin_fast_pop(&root, "key", test_getkey, test_cmpkey);
	isnt(t2, NULL);
	is(t2, t);
	test_free(t2);
	is(gds_rbtree_keyin_fast_pop(&root, "key", test_getkey, test_cmpkey), NULL);

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_keyin_fast_add(&root, test_new(buf, i), test_getkey,
			test_cmpkey);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		t2 = gds_rbtree_keyin_fast_pop(&root, buf, test_getkey,
			test_cmpkey);
		str_eq(t2->key, buf);
		test_free(t2);
		ok(gds_rbtree_keyin_fast_is_valid(root, test_getkey,
			test_cmpkey), NULL);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		is(gds_rbtree_keyin_fast_pop(&root, buf, test_getkey,
			test_cmpkey), NULL);
	}
}

void t_rbtree_keyin_fast_iterator(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	gds_iterator_t *it;
	test_t *t;
	char buf[512];
	int i;
	const char *key;

	for(i=0; i<5; i++) {
		sprintf(buf, "key %02d", i);
		t = test_new(buf, i);
		gds_rbtree_keyin_fast_add(&root, t, test_getkey, test_cmpkey);
	}

	it = gds_rbtree_keyin_fast_iterator_new(root, test_getkey);
	isnt(it, NULL);

	is(gds_iterator_get(it), NULL);
	is(gds_iterator_getkey(it), NULL);

	ok(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isnt(t, NULL);
	isnt(key, NULL);
	str_eq(key, "key 00");

	ok(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isnt(t, NULL);
	isnt(key, NULL);
	str_eq(key, "key 01");

	ok(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isnt(t, NULL);
	isnt(key, NULL);
	str_eq(key, "key 02");

	ok(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isnt(t, NULL);
	isnt(key, NULL);
	str_eq(key, "key 03");

	ok(0 == gds_iterator_step(it));
	t = gds_iterator_get(it);
	key = gds_iterator_getkey(it);
	isnt(t, NULL);
	isnt(key, NULL);
	str_eq(key, "key 04");

	ok(0 < gds_iterator_step(it));

	gds_iterator_free(it);
	gds_rbtree_keyin_fast_free(root, test_free);
}

void t_rbtree_keyin_fast_values(void)
{
	gds_rbtree_keyin_fast_node_t *root = NULL;
	gds_slist_t *slist;
	gds_iterator_t *it;
	test_t *t;
	char buf[512];
	int i;

	for(i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		t = test_new(buf, i);
		gds_rbtree_keyin_fast_add(&root, t, test_getkey, test_cmpkey);
	}

	slist = gds_rbtree_keyin_fast_values(root);
	isnt(slist, NULL);
	it = gds_slist_iterator_new(slist);
	i = 0;
	while(gds_iterator_step(it) == 0) {
		t = gds_iterator_get(it);
		isnt(t, NULL);
		sprintf(buf, "key %02d", i);
		str_eq(t->key, buf);
		is(i, t->value);
		i++;
	}
	gds_iterator_free(it);
	gds_slist_free(slist);
	gds_rbtree_keyin_fast_free(root, test_free);
}

int main()
{
	plan(2282);

	t_rbtree_keyin_fast_add();
	t_rbtree_keyin_fast_set();
	t_rbtree_keyin_fast_get();
	t_rbtree_keyin_fast_del();
	t_rbtree_keyin_fast_pop();
	t_rbtree_keyin_fast_iterator();
	t_rbtree_keyin_fast_values();

	return EXIT_SUCCESS;
}
