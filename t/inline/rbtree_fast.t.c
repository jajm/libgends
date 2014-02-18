#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <libtap13/tap.h>
#include "inline/rbtree_fast.h"

typedef struct {
	int data;
	gds_inline_rbtree_fast_node_t inline_node;
} test_rbtree_fast_node_t;

test_rbtree_fast_node_t * test_rbtree_fast_node_new(int data)
{
	test_rbtree_fast_node_t *trn = malloc(sizeof(test_rbtree_fast_node_t));
	if (trn) {
		gds_inline_rbtree_fast_node_init(&(trn->inline_node));
		trn->data = data;
	}

	return trn;
}

void test_rbtree_fast_node_free(test_rbtree_fast_node_t *trn)
{
	free(trn);
}

test_rbtree_fast_node_t * test_rbtree_fast_node_get_container(
	gds_inline_rbtree_fast_node_t *node_inline)
{
	size_t offset;

	if (node_inline != NULL) {
		offset = offsetof(test_rbtree_fast_node_t, inline_node);
		return (test_rbtree_fast_node_t *) ((char *)node_inline - offset);
	}

	return NULL;
}

int * test_rbtree_fast_node_get_key(int *i)
{
	return i;
}

void test_rbtree_fast_free(test_rbtree_fast_node_t *root)
{
	test_rbtree_fast_node_t *node;

	if (root != NULL) {
		node = test_rbtree_fast_node_get_container(root->inline_node.left);
		test_rbtree_fast_free(node);

		node = test_rbtree_fast_node_get_container(root->inline_node.right);
		test_rbtree_fast_free(node);

		test_rbtree_fast_node_free(root);
	}
}

gds_inline_rbtree_fast_node_t * test_rbtree_fast_node_create_node(int *i)
{
	test_rbtree_fast_node_t *node;

	node = test_rbtree_fast_node_new(*i);

	return &(node->inline_node);
}

void test_rbtree_fast_node_set_data(gds_inline_rbtree_fast_node_t *inode,
	int *i)
{
	test_rbtree_fast_node_t *node;

	node = test_rbtree_fast_node_get_container(inode);
	node->data = *i;
}

int test_rbtree_fast_node_cmp(gds_inline_rbtree_fast_node_t *n1, gds_inline_rbtree_fast_node_t *n2)
{
	test_rbtree_fast_node_t *trfn1, *trfn2;

	trfn1 = test_rbtree_fast_node_get_container(n1);
	trfn2 = test_rbtree_fast_node_get_container(n2);

	return trfn1->data - trfn2->data;
}

int test_rbtree_fast_node_cmp_with_key(gds_inline_rbtree_fast_node_t *n, int *i)
{
	test_rbtree_fast_node_t *trfn;

	trfn = test_rbtree_fast_node_get_container(n);

	return (*i) - trfn->data;
}

void t_gds_inline_rbtree_fast_add(void)
{
	gds_inline_rbtree_fast_node_t *root = NULL, node;
	test_rbtree_fast_node_t *trfn;
	int rc;

	ok(0 > gds_inline_rbtree_fast_add(NULL, NULL, NULL, NULL));
	ok(0 > gds_inline_rbtree_fast_add(NULL, NULL, test_rbtree_fast_node_cmp, NULL));
	ok(0 > gds_inline_rbtree_fast_add(NULL, &node, NULL, NULL));
	ok(0 > gds_inline_rbtree_fast_add(NULL, &node, test_rbtree_fast_node_cmp, NULL));
	ok(0 > gds_inline_rbtree_fast_add(&root, NULL, NULL, NULL));
	ok(0 > gds_inline_rbtree_fast_add(&root, NULL, test_rbtree_fast_node_cmp, NULL));
	ok(0 > gds_inline_rbtree_fast_add(&root, &node, NULL, NULL));

	trfn = test_rbtree_fast_node_new(1);
	rc = gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	is(rc, 0);
	rc = gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	is(rc, 1);
	trfn = test_rbtree_fast_node_new(0);
	rc = gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	is(rc, 0);
	rc = gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	is(rc, 1);
	trfn = test_rbtree_fast_node_new(2);
	rc = gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	is(rc, 0);
	rc = gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	is(rc, 1);

	isnt(root, NULL);
	trfn = test_rbtree_fast_node_get_container(root);
	isnt(trfn, NULL);
	is(trfn->data, 1);
	trfn = test_rbtree_fast_node_get_container(root->left);
	isnt(trfn, NULL);
	is(trfn->data, 0);
	trfn = test_rbtree_fast_node_get_container(root->right);
	isnt(trfn, NULL);
	is(trfn->data, 2);

	test_rbtree_fast_free(test_rbtree_fast_node_get_container(root));
}

void t_gds_inline_rbtree_fast_del(void)
{
	gds_inline_rbtree_fast_node_t *root = NULL, *inode;
	test_rbtree_fast_node_t *trfn;
	int i = 1;

	trfn = test_rbtree_fast_node_new(1);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(0);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(2);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);

	ok(NULL == gds_inline_rbtree_fast_del(NULL, NULL, NULL, NULL));
	ok(NULL == gds_inline_rbtree_fast_del(NULL, NULL, test_rbtree_fast_node_cmp_with_key, NULL));
	ok(NULL == gds_inline_rbtree_fast_del(NULL, &i, NULL, NULL));
	ok(NULL == gds_inline_rbtree_fast_del(NULL, &i, test_rbtree_fast_node_cmp_with_key, NULL));
	ok(NULL == gds_inline_rbtree_fast_del(&root, NULL, NULL, NULL));
	ok(NULL == gds_inline_rbtree_fast_del(&root, &i, NULL, NULL));

	inode = gds_inline_rbtree_fast_del(&root, &i,
		test_rbtree_fast_node_cmp_with_key, NULL);
	isnt(inode, NULL);
	trfn = test_rbtree_fast_node_get_container(inode);
	is(trfn->data, 1);
	test_rbtree_fast_node_free(trfn);
	
	trfn = test_rbtree_fast_node_get_container(root);
	if (trfn->data == 0) {
		is(root->left, NULL);
		isnt(root->right, NULL);
		trfn = test_rbtree_fast_node_get_container(root->right);
		is(trfn->data, 2);
	} else {
		is(root->right, NULL);
		isnt(root->left, NULL);
		trfn = test_rbtree_fast_node_get_container(root->left);
		is(trfn->data, 0);
	}

	test_rbtree_fast_free(test_rbtree_fast_node_get_container(root));
}

void t_gds_inline_rbtree_fast_get_node(void)
{
	gds_inline_rbtree_fast_node_t *root = NULL, *node_inline;
	test_rbtree_fast_node_t *trfn;
	int i = 0;

	trfn = test_rbtree_fast_node_new(1);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(0);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(2);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);

	ok(NULL == gds_inline_rbtree_fast_get_node(NULL, NULL, NULL, NULL));
	ok(NULL == gds_inline_rbtree_fast_get_node(NULL, NULL, test_rbtree_fast_node_cmp_with_key, NULL));
	ok(NULL == gds_inline_rbtree_fast_get_node(NULL, &i, NULL, NULL));
	ok(NULL == gds_inline_rbtree_fast_get_node(NULL, &i, test_rbtree_fast_node_cmp_with_key, NULL));
	ok(NULL == gds_inline_rbtree_fast_get_node(root, NULL, NULL, NULL));
	ok(NULL == gds_inline_rbtree_fast_get_node(root, &i, NULL, NULL));

	node_inline = gds_inline_rbtree_fast_get_node(root, &i,
		test_rbtree_fast_node_cmp_with_key, NULL);
	isnt(node_inline, NULL);
	trfn = test_rbtree_fast_node_get_container(node_inline);
	is(trfn->data, i);
	i = 1;
	node_inline = gds_inline_rbtree_fast_get_node(root, &i,
		test_rbtree_fast_node_cmp_with_key, NULL);
	isnt(node_inline, NULL);
	trfn = test_rbtree_fast_node_get_container(node_inline);
	is(trfn->data, i);
	i = 2;
	node_inline = gds_inline_rbtree_fast_get_node(root, &i,
		test_rbtree_fast_node_cmp_with_key, NULL);
	isnt(node_inline, NULL);
	trfn = test_rbtree_fast_node_get_container(node_inline);
	is(trfn->data, i);

	test_rbtree_fast_free(test_rbtree_fast_node_get_container(root));
}

void t_gds_inline_rbtree_fast_iterator(void)
{
	gds_inline_rbtree_fast_node_t *root = NULL, *node_inline;
	test_rbtree_fast_node_t *trfn;
	gds_iterator_t *it;

	trfn = test_rbtree_fast_node_new(1);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(0);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(2);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		test_rbtree_fast_node_cmp, NULL);

	ok(NULL == gds_inline_rbtree_fast_iterator_new(NULL));

	it = gds_inline_rbtree_fast_iterator_new(root);

	ok(0 == gds_iterator_step(it));
	node_inline = gds_iterator_get(it);
	trfn = test_rbtree_fast_node_get_container(node_inline);
	isnt(trfn, NULL);
	is(trfn->data, 0);

	ok(0 == gds_iterator_step(it));
	node_inline = gds_iterator_get(it);
	trfn = test_rbtree_fast_node_get_container(node_inline);
	isnt(trfn, NULL);
	is(trfn->data, 1);

	ok(0 == gds_iterator_step(it));
	node_inline = gds_iterator_get(it);
	trfn = test_rbtree_fast_node_get_container(node_inline);
	isnt(trfn, NULL);
	is(trfn->data, 2);

	ok(0 < gds_iterator_step(it));

	gds_iterator_free(it);
	test_rbtree_fast_free(test_rbtree_fast_node_get_container(root));
}

int main()
{
	plan(54);

	t_gds_inline_rbtree_fast_add();
	t_gds_inline_rbtree_fast_del();
	t_gds_inline_rbtree_fast_get_node();
	t_gds_inline_rbtree_fast_iterator();

	return EXIT_SUCCESS;
}
