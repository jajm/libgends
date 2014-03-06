#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libtap13/tap.h>
#include "rbtree.h"
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
	((ptr) != NULL) ? (gds_rbtree_node_t *)((char *)ptr \
		- offsetof(gds_rbtree_node_t, rbtree)) : NULL

void gds_rbtree_print(gds_rbtree_node_t *root, int depth)
{
	char *key;
	gds_rbtree_node_t *node;
	if(root != NULL) {
		node = containerof(root->rbtree.son[1]);
		gds_rbtree_print(node, depth+1);
		for (int i=0; i<depth; i++)
			printf("- ");
		key = root->key;
		if (key == NULL) {
			printf("(NULL)");
		} else {
			printf("%s", key);
		}
		printf(": %d\n", test_getvalue(root->data));
		node = containerof(root->rbtree.son[0]);
		gds_rbtree_print(node, depth+1);
	}
}

void gds_rbtree_print_dbg_r(gds_rbtree_node_t *root, int d)
{
	char *key;
	if (root != NULL) {
		gds_rbtree_print_dbg_r(containerof(root->rbtree.son[1]), d+1);
		for (int i=0; i<d; i++)
			printf("- ");
		key = root->key;
		printf("root %p rbtree %p [%s] (0: %p, 1: %p) %s\n",
			root, &(root->rbtree), root->rbtree.red ? "red" : "black",
			root->rbtree.son[0], root->rbtree.son[1], key);
		gds_rbtree_print_dbg_r(containerof(root->rbtree.son[0]), d+1);
	}
}
void gds_rbtree_print_dbg(gds_rbtree_node_t *root)
{
	printf("\n---------------------------------------------------------\n");
	gds_rbtree_print_dbg_r(root, 0);
	printf("---------------------------------------------------------\n");
}

int gds_rbtree_is_valid(gds_rbtree_node_t *root, void *test_cmpkey)
{
	int lh, rh;
	int cmp = 0, cmp2 = 0;
	gds_rbtree_node_t *node;
	int (*cmpkey_callback)(void *, void *) = test_cmpkey;

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
	lh = gds_rbtree_is_valid(node, test_cmpkey);
	node = containerof(rn);
	rh = gds_rbtree_is_valid(node, test_cmpkey);

	/* Invalid binary search tree */
	if(ln != NULL) {
		node = containerof(ln);
		cmp = cmpkey_callback(root->key, node->key);
	}
	if(rn != NULL) {
		node = containerof(rn);
		cmp2 = cmpkey_callback(root->key, node->key);
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

void t_rbtree_add(void)
{
	gds_rbtree_node_t *root = NULL;
	test_t *t = test_new("string", 4);
	char buf[512];
	char *key = "key";
	char *key2 = "key2";

	ok(0 > gds_rbtree_add(NULL , NULL, NULL, NULL     ));
	ok(0 > gds_rbtree_add(NULL , NULL, NULL, test_cmpkey));
	ok(0 > gds_rbtree_add(NULL , NULL, t   , NULL     ));
	ok(0 > gds_rbtree_add(NULL , NULL, t   , test_cmpkey));
	ok(0 > gds_rbtree_add(NULL , key , NULL, NULL     ));
	ok(0 > gds_rbtree_add(NULL , key , NULL, test_cmpkey));
	ok(0 > gds_rbtree_add(NULL , key , t   , NULL     ));
	ok(0 > gds_rbtree_add(NULL , key , t   , test_cmpkey));
	ok(0 > gds_rbtree_add(&root, NULL, NULL, NULL     ));
	ok(0 > gds_rbtree_add(&root, NULL, t   , NULL     ));
	ok(0 > gds_rbtree_add(&root, key , NULL, NULL     ));
	ok(0 > gds_rbtree_add(&root, key , t   , NULL     ));

	/* Tree is unchanged */
	is(root, NULL);
	ok(gds_rbtree_is_valid(root, test_cmpkey));

	/* Add a data with NULL key, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	ok(0 == gds_rbtree_add(&root, NULL, NULL, test_cmpkey));
	ok(0 <  gds_rbtree_add(&root, NULL, NULL, test_cmpkey));
	ok(0 <  gds_rbtree_add(&root, NULL, t   , test_cmpkey));

	ok(gds_rbtree_is_valid(root, test_cmpkey));

	/* Add NULL as data, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	ok(0 == gds_rbtree_add(&root, key, NULL, test_cmpkey));
	ok(0 <  gds_rbtree_add(&root, key, NULL, test_cmpkey));

	ok(gds_rbtree_is_valid(root, test_cmpkey));

	/* Add t as data, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	ok(0 == gds_rbtree_add(&root, key2, t, test_cmpkey));
	ok(0 <  gds_rbtree_add(&root, key2, t, test_cmpkey));

	ok(gds_rbtree_is_valid(root, test_cmpkey));

	gds_rbtree_free(root, NULL, test_free);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		ok(0 == gds_rbtree_add(&root, key_alloc(buf), test_new(buf, i), test_cmpkey));
		ok(gds_rbtree_is_valid(root, test_cmpkey));
	}

	gds_rbtree_free(root, free, test_free);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %02d", i);
		ok(0 == gds_rbtree_add(&root, key_alloc(buf), test_new(buf, i), test_cmpkey));
		ok(gds_rbtree_is_valid(root, test_cmpkey));
	}

	gds_rbtree_free(root, free, test_free);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %02d", j);
		ok(0 <= gds_rbtree_add(&root, key_alloc(buf), test_new(buf, i), test_cmpkey));
		ok(gds_rbtree_is_valid(root, test_cmpkey));
	}

	gds_rbtree_free(root, free, test_free);
}

void t_rbtree_set(void)
{
	gds_rbtree_node_t *root = NULL;
	test_t *t = test_new("string", 4);
	char buf[512];
	char *key = "key";
	char *key2 = "key2";

	ok(0 > gds_rbtree_set(NULL , NULL, NULL, NULL     , NULL, NULL));
	ok(0 > gds_rbtree_set(NULL , NULL, NULL, NULL     , NULL, free));
	ok(0 > gds_rbtree_set(NULL , NULL, NULL, NULL     , free, NULL));
	ok(0 > gds_rbtree_set(NULL , NULL, NULL, NULL     , free, free));
	ok(0 > gds_rbtree_set(NULL , NULL, NULL, test_cmpkey, NULL, NULL));
	ok(0 > gds_rbtree_set(NULL , NULL, NULL, test_cmpkey, NULL, free));
	ok(0 > gds_rbtree_set(NULL , NULL, NULL, test_cmpkey, free, NULL));
	ok(0 > gds_rbtree_set(NULL , NULL, NULL, test_cmpkey, free, free));
	ok(0 > gds_rbtree_set(NULL , NULL, t   , NULL     , NULL, NULL));
	ok(0 > gds_rbtree_set(NULL , NULL, t   , NULL     , NULL, free));
	ok(0 > gds_rbtree_set(NULL , NULL, t   , NULL     , free, NULL));
	ok(0 > gds_rbtree_set(NULL , NULL, t   , NULL     , free, free));
	ok(0 > gds_rbtree_set(NULL , NULL, t   , test_cmpkey, NULL, NULL));
	ok(0 > gds_rbtree_set(NULL , NULL, t   , test_cmpkey, NULL, free));
	ok(0 > gds_rbtree_set(NULL , NULL, t   , test_cmpkey, free, NULL));
	ok(0 > gds_rbtree_set(NULL , NULL, t   , test_cmpkey, free, free));
	ok(0 > gds_rbtree_set(NULL , key , NULL, NULL     , NULL, NULL));
	ok(0 > gds_rbtree_set(NULL , key , NULL, NULL     , NULL, free));
	ok(0 > gds_rbtree_set(NULL , key , NULL, NULL     , free, NULL));
	ok(0 > gds_rbtree_set(NULL , key , NULL, NULL     , free, free));
	ok(0 > gds_rbtree_set(NULL , key , NULL, test_cmpkey, NULL, NULL));
	ok(0 > gds_rbtree_set(NULL , key , NULL, test_cmpkey, NULL, free));
	ok(0 > gds_rbtree_set(NULL , key , NULL, test_cmpkey, free, NULL));
	ok(0 > gds_rbtree_set(NULL , key , NULL, test_cmpkey, free, free));
	ok(0 > gds_rbtree_set(NULL , key , t   , NULL     , NULL, NULL));
	ok(0 > gds_rbtree_set(NULL , key , t   , NULL     , NULL, free));
	ok(0 > gds_rbtree_set(NULL , key , t   , NULL     , free, NULL));
	ok(0 > gds_rbtree_set(NULL , key , t   , NULL     , free, free));
	ok(0 > gds_rbtree_set(NULL , key , t   , test_cmpkey, NULL, NULL));
	ok(0 > gds_rbtree_set(NULL , key , t   , test_cmpkey, NULL, free));
	ok(0 > gds_rbtree_set(NULL , key , t   , test_cmpkey, free, NULL));
	ok(0 > gds_rbtree_set(NULL , key , t   , test_cmpkey, free, free));
	ok(0 > gds_rbtree_set(&root, NULL, NULL, NULL     , NULL, NULL));
	ok(0 > gds_rbtree_set(&root, NULL, NULL, NULL     , NULL, free));
	ok(0 > gds_rbtree_set(&root, NULL, NULL, NULL     , free, NULL));
	ok(0 > gds_rbtree_set(&root, NULL, NULL, NULL     , free, free));
	ok(0 > gds_rbtree_set(&root, NULL, t   , NULL     , NULL, NULL));
	ok(0 > gds_rbtree_set(&root, NULL, t   , NULL     , NULL, free));
	ok(0 > gds_rbtree_set(&root, NULL, t   , NULL     , free, NULL));
	ok(0 > gds_rbtree_set(&root, NULL, t   , NULL     , free, free));
	ok(0 > gds_rbtree_set(&root, key , NULL, NULL     , NULL, NULL));
	ok(0 > gds_rbtree_set(&root, key , NULL, NULL     , NULL, free));
	ok(0 > gds_rbtree_set(&root, key , NULL, NULL     , free, NULL));
	ok(0 > gds_rbtree_set(&root, key , NULL, NULL     , free, free));
	ok(0 > gds_rbtree_set(&root, key , t   , NULL     , NULL, NULL));
	ok(0 > gds_rbtree_set(&root, key , t   , NULL     , NULL, free));
	ok(0 > gds_rbtree_set(&root, key , t   , NULL     , free, NULL));
	ok(0 > gds_rbtree_set(&root, key , t   , NULL     , free, free));

	/* Tree is unchanged */
	is(root, NULL);
	ok(gds_rbtree_is_valid(root, test_cmpkey));

	/* Add a data with NULL key, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	ok(0 == gds_rbtree_set(&root, NULL, NULL, test_cmpkey, NULL, NULL));
	ok(0 <  gds_rbtree_set(&root, NULL, NULL, test_cmpkey, NULL, NULL));
	ok(0 <  gds_rbtree_set(&root, NULL, t   , test_cmpkey, NULL, NULL));

	ok(gds_rbtree_is_valid(root, test_cmpkey));

	/* Add NULL as data, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	ok(0 == gds_rbtree_set(&root, key, NULL, test_cmpkey, NULL, NULL));
	ok(0 <  gds_rbtree_set(&root, key, NULL, test_cmpkey, NULL, NULL));

	ok(gds_rbtree_is_valid(root, test_cmpkey));

	/* Add t as data, first call should success, others must
	 * return a positive value, meaning that key is already in the tree. */
	ok(0 == gds_rbtree_set(&root, key2, t, test_cmpkey, NULL, NULL));
	ok(0 <  gds_rbtree_set(&root, key2, t, test_cmpkey, NULL, NULL));

	ok(gds_rbtree_is_valid(root, test_cmpkey));

	gds_rbtree_free(root, NULL, NULL);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		ok(0 == gds_rbtree_set(&root, key_alloc(buf), test_new(buf, i), test_cmpkey, NULL, NULL));
		ok(gds_rbtree_is_valid(root, test_cmpkey));
	}

	gds_rbtree_free(root, free, test_free);
	root = NULL;

	for (int i=100; i>0; i--) {
		sprintf(buf, "key %02d", i);
		ok(0 == gds_rbtree_set(&root, key_alloc(buf), test_new(buf, i), test_cmpkey, NULL, NULL));
		ok(gds_rbtree_is_valid(root, test_cmpkey));
	}

	gds_rbtree_free(root, free, test_free);
	root = NULL;

	srand(time(NULL));
	for (int i=0; i<100; i++) {
		int j = rand();
		sprintf(buf, "key %02d", j);
		ok(0 <= gds_rbtree_set(&root, key_alloc(buf), test_new(buf, i), test_cmpkey, NULL, NULL));
		ok(gds_rbtree_is_valid(root, test_cmpkey));
	}

	gds_rbtree_free(root, free, test_free);

	test_free(t);
}

void t_rbtree_get(void)
{
	gds_rbtree_node_t *root = NULL;
	test_t *data;
	char buf[512];

	gds_rbtree_add(&root, "key", test_new("string", 1), test_cmpkey);

	ok(NULL == gds_rbtree_get(NULL, NULL, NULL));
	ok(NULL == gds_rbtree_get(NULL, "key", NULL));
	ok(NULL == gds_rbtree_get(root, NULL, NULL));
	ok(NULL == gds_rbtree_get(root, "key", NULL));
	is(gds_rbtree_get(NULL, NULL, test_cmpkey), NULL);
	is(gds_rbtree_get(NULL, "key", test_cmpkey), NULL);
	is(gds_rbtree_get(root, NULL, test_cmpkey), NULL);
	isnt(gds_rbtree_get(root, "key", test_cmpkey), NULL);

	gds_rbtree_free(root, NULL, test_free);
	root = NULL;

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_add(&root, key_alloc(buf), test_new(buf, i), test_cmpkey);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		data = gds_rbtree_get(root, buf, test_cmpkey);
		isnt(data, NULL);
		is(i, test_getvalue(data));
	}
	gds_rbtree_free(root, free, test_free);
}

void t_rbtree_del(void)
{
	gds_rbtree_node_t *root = NULL;
	test_t *t = test_new("key", 1);
	char buf[512];

	gds_rbtree_add(&root, "key", t, test_cmpkey);

	ok(0 > gds_rbtree_del(NULL , NULL , NULL     , NULL       , NULL));
	ok(0 > gds_rbtree_del(NULL , NULL , NULL     , NULL       , test_free));
	ok(0 > gds_rbtree_del(NULL , NULL , NULL     , free, NULL));
	ok(0 > gds_rbtree_del(NULL , NULL , NULL     , free, test_free));
	ok(0 > gds_rbtree_del(NULL , NULL , test_cmpkey, NULL       , NULL));
	ok(0 > gds_rbtree_del(NULL , NULL , test_cmpkey, NULL       , test_free));
	ok(0 > gds_rbtree_del(NULL , NULL , test_cmpkey, free, NULL));
	ok(0 > gds_rbtree_del(NULL , NULL , test_cmpkey, free, test_free));
	ok(0 > gds_rbtree_del(NULL , "key", NULL     , NULL       , NULL));
	ok(0 > gds_rbtree_del(NULL , "key", NULL     , NULL       , test_free));
	ok(0 > gds_rbtree_del(NULL , "key", NULL     , free, NULL));
	ok(0 > gds_rbtree_del(NULL , "key", NULL     , free, test_free));
	ok(0 > gds_rbtree_del(NULL , "key", test_cmpkey, NULL       , NULL));
	ok(0 > gds_rbtree_del(NULL , "key", test_cmpkey, NULL       , test_free));
	ok(0 > gds_rbtree_del(NULL , "key", test_cmpkey, free, NULL));
	ok(0 > gds_rbtree_del(NULL , "key", test_cmpkey, free, test_free));
	ok(0 > gds_rbtree_del(&root, NULL , NULL     , NULL       , NULL));
	ok(0 > gds_rbtree_del(&root, NULL , NULL     , NULL       , test_free));
	ok(0 > gds_rbtree_del(&root, NULL , NULL     , free, NULL));
	ok(0 > gds_rbtree_del(&root, NULL , NULL     , free, test_free));
	ok(0 > gds_rbtree_del(&root, "key", NULL     , NULL       , NULL));
	ok(0 > gds_rbtree_del(&root, "key", NULL     , NULL       , test_free));
	ok(0 > gds_rbtree_del(&root, "key", NULL     , free, NULL));
	ok(0 > gds_rbtree_del(&root, "key", NULL     , free, test_free));

	ok(0 < gds_rbtree_del(&root, NULL, test_cmpkey, NULL, NULL));
	ok(0 < gds_rbtree_del(&root, NULL, test_cmpkey, NULL, test_free));
	ok(0 < gds_rbtree_del(&root, NULL, test_cmpkey, free, NULL));
	ok(0 < gds_rbtree_del(&root, NULL, test_cmpkey, free, test_free));
	ok(0 == gds_rbtree_del(&root, "key", test_cmpkey, NULL, NULL));
	ok(0 < gds_rbtree_del(&root, "key", test_cmpkey, NULL, NULL));
	ok(0 < gds_rbtree_del(&root, "key", test_cmpkey, NULL, test_free));
	ok(0 < gds_rbtree_del(&root, "key", test_cmpkey, free, NULL));
	ok(0 < gds_rbtree_del(&root, "key", test_cmpkey, free, test_free));

	test_free(t);

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_add(&root, key_alloc(buf), test_new(buf, i), test_cmpkey);
	}
	for (int i=50; i<100; i++) {
		sprintf(buf, "key %d", i);
		ok(0 == gds_rbtree_del(&root, buf,
			test_cmpkey, free, test_free), NULL);
		ok(gds_rbtree_is_valid(root, test_cmpkey));
	}
	for (int i=49; i>=0; i--) {
		sprintf(buf, "key %d", i);
		ok(0 == gds_rbtree_del(&root, buf,
			test_cmpkey, free, test_free), NULL);
		ok(gds_rbtree_is_valid(root, test_cmpkey));
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		ok(0 < gds_rbtree_del(&root, buf,
			test_cmpkey, free, test_free), NULL);
	}
}

void t_rbtree_pop(void)
{
	gds_rbtree_node_t *root = NULL;
	test_t *t = test_new("key", 1);
	char buf[512];
	test_t *t2;

	gds_rbtree_add(&root, "key", t, test_cmpkey);

	ok(NULL == gds_rbtree_pop(NULL , NULL , NULL       , NULL));
	ok(NULL == gds_rbtree_pop(NULL , NULL , NULL       , free));
	ok(NULL == gds_rbtree_pop(NULL , NULL , test_cmpkey, NULL));
	ok(NULL == gds_rbtree_pop(NULL , NULL , test_cmpkey, free));
	ok(NULL == gds_rbtree_pop(NULL , "key", NULL       , NULL));
	ok(NULL == gds_rbtree_pop(NULL , "key", NULL       , free));
	ok(NULL == gds_rbtree_pop(NULL , "key", test_cmpkey, NULL));
	ok(NULL == gds_rbtree_pop(NULL , "key", test_cmpkey, free));
	ok(NULL == gds_rbtree_pop(&root, NULL , NULL       , NULL));
	ok(NULL == gds_rbtree_pop(&root, NULL , NULL       , free));
	ok(NULL == gds_rbtree_pop(&root, "key", NULL       , NULL));
	ok(NULL == gds_rbtree_pop(&root, "key", NULL       , free));

	is(gds_rbtree_pop(&root, NULL , test_cmpkey, NULL), NULL);
	is(gds_rbtree_pop(&root, NULL , test_cmpkey, free), NULL);
	t2 = gds_rbtree_pop(&root, "key", test_cmpkey, NULL);
	isnt(t2, NULL);
	is(t2, t);
	test_free(t2);
	is(gds_rbtree_pop(&root, "key", test_cmpkey, free), NULL);

	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		gds_rbtree_add(&root, key_alloc(buf), test_new(buf, i), test_cmpkey);
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		t2 = gds_rbtree_pop(&root, buf, test_cmpkey, free);
		str_eq(t2->key, buf);
		test_free(t2);
		ok(gds_rbtree_is_valid(root, test_cmpkey));
	}
	for (int i=0; i<100; i++) {
		sprintf(buf, "key %d", i);
		ok(0 < gds_rbtree_del(&root, buf,
			test_cmpkey, free, test_free), NULL);
	}
}

void t_rbtree_iterator(void)
{
	gds_rbtree_node_t *root = NULL;
	gds_iterator_t *it;
	test_t *t;
	char buf[512];
	int i;
	const char *key;

	for(i=0; i<5; i++) {
		sprintf(buf, "key %02d", i);
		t = test_new(buf, i);
		gds_rbtree_add(&root, key_alloc(buf), t, test_cmpkey);
	}

	it = gds_rbtree_iterator_new(root);
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
	gds_rbtree_free(root, free, test_free);
}

void t_rbtree_keys()
{
	char *keys[] = { "01", "02", "03" };
	char *data[] = { "foo", "bar", "baz" };
	char *s;
	gds_rbtree_node_t *root = NULL;
	gds_slist_t *list;
	gds_iterator_t *it;
	int i;

	for (i=0; i<3; i++) {
		gds_rbtree_add(&root, keys[i], data[i], test_cmpkey);
	}

	list = gds_rbtree_keys(root);
	it = gds_slist_iterator_new(list);
	gds_iterator_step(it);
	s = gds_iterator_get(it);
	str_eq(s, "01");
	gds_iterator_step(it);
	s = gds_iterator_get(it);
	str_eq(s, "02");
	gds_iterator_step(it);
	s = gds_iterator_get(it);
	str_eq(s, "03");
	ok(0 < gds_iterator_step(it));

	gds_iterator_free(it);
	gds_slist_free(list);
	gds_rbtree_free(root, NULL, NULL);
}

void t_rbtree_values(void)
{
	gds_rbtree_node_t *root = NULL;
	gds_slist_t *slist;
	gds_iterator_t *it;
	test_t *t;
	char buf[512];
	int i;

	for(i=0; i<100; i++) {
		sprintf(buf, "key %02d", i);
		t = test_new(buf, i);
		gds_rbtree_add(&root, key_alloc(buf), t, test_cmpkey);
	}

	slist = gds_rbtree_values(root);
	isnt(slist, NULL);
	it = gds_slist_iterator_new(slist);
	i = 0;
	while(gds_iterator_step(it) == 0) {
		t = gds_iterator_get(it);
		isnt(t, NULL);
		is(i, t->value);
		i++;
	}
	gds_iterator_free(it);
	gds_slist_free(slist);
	gds_rbtree_free(root, free, test_free);
}


int main()
{
	plan(2371);

	t_rbtree_add();
	t_rbtree_set();
	t_rbtree_get();
	t_rbtree_del();
	t_rbtree_pop();
	t_rbtree_iterator();
	t_rbtree_keys();
	t_rbtree_values();

	return EXIT_SUCCESS;
}
