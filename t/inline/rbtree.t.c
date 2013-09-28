#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "inline/rbtree.h"
#include "../tap.h"

typedef struct {
	int data;
	gds_inline_rbtree_node_t inline_node;
} test_rbtree_node_t;

test_rbtree_node_t * test_rbtree_node_new(int data)
{
	test_rbtree_node_t *trn = malloc(sizeof(test_rbtree_node_t));
	if (trn) {
		gds_inline_rbtree_node_init(&(trn->inline_node));
		trn->data = data;
	}

	return trn;
}

void test_rbtree_node_free(test_rbtree_node_t *trn)
{
	free(trn);
}

test_rbtree_node_t * test_rbtree_node_get_container(
	gds_inline_rbtree_node_t *node_inline)
{
	size_t offset;

	if (node_inline != NULL) {
		offset = offsetof(test_rbtree_node_t, inline_node);
		return (test_rbtree_node_t *) ((char *)node_inline - offset);
	}

	return NULL;
}

void test_rbtree_free(test_rbtree_node_t *root)
{
	test_rbtree_node_t *node;

	if (root != NULL) {
		node = test_rbtree_node_get_container(root->inline_node.son[0]);
		test_rbtree_free(node);

		node = test_rbtree_node_get_container(root->inline_node.son[1]);
		test_rbtree_free(node);

		test_rbtree_node_free(root);
	}
}

int32_t test_rbtree_node_compare(gds_inline_rbtree_node_t *n1,
	gds_inline_rbtree_node_t *n2)
{
	test_rbtree_node_t *trn1, *trn2;

	trn1 = test_rbtree_node_get_container(n1);
	trn2 = test_rbtree_node_get_container(n2);

	return trn1->data - trn2->data;
}

int32_t test_rbtree_node_compare_with_key(gds_inline_rbtree_node_t *n, int *i)
{
	test_rbtree_node_t *trn;

	trn = test_rbtree_node_get_container(n);

	return (*i) - trn->data;
}

void t_gds_inline_rbtree_add(void)
{
	test_rbtree_node_t *root, *trn;
	gds_inline_rbtree_node_t *root_inline;

	root = test_rbtree_node_new(0);
	root_inline = &(root->inline_node);
	trn = test_rbtree_node_new(-1);
	gds_inline_rbtree_add(&root_inline, &(trn->inline_node), (gds_rbt_cmp_cb)test_rbtree_node_compare, NULL);
	trn = test_rbtree_node_new(1);
	gds_inline_rbtree_add(&root_inline, &(trn->inline_node), (gds_rbt_cmp_cb)test_rbtree_node_compare, NULL);

	isntnull(root, NULL);
	is(root->data, 0, NULL);
	trn = test_rbtree_node_get_container(root->inline_node.son[0]);
	isntnull(trn, NULL);
	is(trn->data, -1, NULL);
	trn = test_rbtree_node_get_container(root->inline_node.son[1]);
	isntnull(trn, NULL);
	is(trn->data, 1, NULL);

	test_rbtree_free(root);
}

void t_gds_inline_rbtree_del(void)
{
	test_rbtree_node_t *root, *trn;
	gds_inline_rbtree_node_t *root_inline, *inode;
	int i;

	root = test_rbtree_node_new(0);
	root_inline = &(root->inline_node);
	trn = test_rbtree_node_new(-1);
	gds_inline_rbtree_add(&root_inline, &(trn->inline_node), (gds_rbt_cmp_cb)test_rbtree_node_compare, NULL);
	trn = test_rbtree_node_new(1);
	gds_inline_rbtree_add(&root_inline, &(trn->inline_node), (gds_rbt_cmp_cb)test_rbtree_node_compare, NULL);

	i = 1;
	inode = gds_inline_rbtree_del(&root_inline, &i,
		(gds_rbt_cmp_with_key_cb) test_rbtree_node_compare_with_key,
		NULL);
	isntnull(inode, NULL);
	trn = test_rbtree_node_get_container(inode);
	test_rbtree_node_free(trn);
	
	root = test_rbtree_node_get_container(root_inline);
	if (root->data == -1) {
		isnull(root_inline->son[0], NULL);
		isntnull(root_inline->son[1], NULL);
		trn = test_rbtree_node_get_container(root_inline->son[1]);
		is(trn->data, 1, NULL);
	} else {
		isnull(root_inline->son[1], NULL);
		isntnull(root_inline->son[0], NULL);
		trn = test_rbtree_node_get_container(root_inline->son[0]);
		is(trn->data, -1, NULL);
	}

	test_rbtree_free(root);
}

void t_gds_inline_rbtree_get_node(void)
{
	test_rbtree_node_t *root, *trn;
	gds_inline_rbtree_node_t *root_inline, *node_inline;
	int i;

	root = test_rbtree_node_new(0);
	root_inline = &(root->inline_node);
	trn = test_rbtree_node_new(-1);
	gds_inline_rbtree_add(&root_inline, &(trn->inline_node), (gds_rbt_cmp_cb)test_rbtree_node_compare, NULL);
	trn = test_rbtree_node_new(1);
	gds_inline_rbtree_add(&root_inline, &(trn->inline_node), (gds_rbt_cmp_cb)test_rbtree_node_compare, NULL);

	i = 1;
	node_inline = gds_inline_rbtree_get_node(root_inline, &i,
		(gds_rbt_cmp_with_key_cb) test_rbtree_node_compare_with_key, NULL);
	isntnull(node_inline, NULL);
	trn = test_rbtree_node_get_container(node_inline);
	is(trn->data, i, NULL);
	i = -1;
	node_inline = gds_inline_rbtree_get_node(root_inline, &i,
		(gds_rbt_cmp_with_key_cb) test_rbtree_node_compare_with_key, NULL);
	isntnull(node_inline, NULL);
	trn = test_rbtree_node_get_container(node_inline);
	is(trn->data, i, NULL);
	i = 0;
	node_inline = gds_inline_rbtree_get_node(root_inline, &i,
		(gds_rbt_cmp_with_key_cb) test_rbtree_node_compare_with_key, NULL);
	isntnull(node_inline, NULL);
	trn = test_rbtree_node_get_container(node_inline);
	is(trn->data, i, NULL);

	test_rbtree_free(root);
}

void t_gds_inline_rbtree_iterator(void)
{
	test_rbtree_node_t *root, *trn;
	gds_inline_rbtree_node_t *root_inline, *node_inline;
	gds_iterator_t *it;

	root = test_rbtree_node_new(0);
	root_inline = &(root->inline_node);
	trn = test_rbtree_node_new(-1);
	gds_inline_rbtree_add(&root_inline, &(trn->inline_node), (gds_rbt_cmp_cb)test_rbtree_node_compare, NULL);
	trn = test_rbtree_node_new(1);
	gds_inline_rbtree_add(&root_inline, &(trn->inline_node), (gds_rbt_cmp_cb)test_rbtree_node_compare, NULL);

	it = gds_inline_rbtree_iterator_new(root_inline);

	ok(0 == gds_iterator_step(it), NULL);
	node_inline = gds_iterator_get(it);
	trn = test_rbtree_node_get_container(node_inline);
	isntnull(trn, NULL);
	is(trn->data, -1, NULL);

	ok(0 == gds_iterator_step(it), NULL);
	node_inline = gds_iterator_get(it);
	trn = test_rbtree_node_get_container(node_inline);
	isntnull(trn, NULL);
	is(trn->data, 0, NULL);

	ok(0 == gds_iterator_step(it), NULL);
	node_inline = gds_iterator_get(it);
	trn = test_rbtree_node_get_container(node_inline);
	isntnull(trn, NULL);
	is(trn->data, 1, NULL);

	ok(0 < gds_iterator_step(it), NULL);

	gds_iterator_free(it);
	test_rbtree_free(root);
}

int main()
{
	plan(26);

	t_gds_inline_rbtree_add();
	t_gds_inline_rbtree_del();
	t_gds_inline_rbtree_get_node();
	t_gds_inline_rbtree_iterator();

	return EXIT_SUCCESS;
}