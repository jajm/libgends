#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <CUnit/Basic.h>
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

int32_t test_rbtree_fast_node_cmp(gds_inline_rbtree_fast_node_t *n1, gds_inline_rbtree_fast_node_t *n2)
{
	test_rbtree_fast_node_t *trfn1, *trfn2;

	trfn1 = test_rbtree_fast_node_get_container(n1);
	trfn2 = test_rbtree_fast_node_get_container(n2);

	return trfn1->data - trfn2->data;
}

int32_t test_rbtree_fast_node_cmp_with_key(gds_inline_rbtree_fast_node_t *n, int *i)
{
	test_rbtree_fast_node_t *trfn;

	trfn = test_rbtree_fast_node_get_container(n);

	return (*i) - trfn->data;
}

void t_gds_inline_rbtree_fast_add(void)
{
	gds_inline_rbtree_fast_node_t *root = NULL;
	test_rbtree_fast_node_t *trfn;

	trfn = test_rbtree_fast_node_new(1);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(0);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(2);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);

	CU_ASSERT_PTR_NOT_NULL(root);
	trfn = test_rbtree_fast_node_get_container(root);
	CU_ASSERT_PTR_NOT_NULL(trfn);
	CU_ASSERT_EQUAL(trfn->data, 1);
	trfn = test_rbtree_fast_node_get_container(root->left);
	CU_ASSERT_PTR_NOT_NULL(trfn);
	CU_ASSERT_EQUAL(trfn->data, 0);
	trfn = test_rbtree_fast_node_get_container(root->right);
	CU_ASSERT_PTR_NOT_NULL(trfn);
	CU_ASSERT_EQUAL(trfn->data, 2);

	test_rbtree_fast_free(test_rbtree_fast_node_get_container(root));
}

void t_gds_inline_rbtree_fast_del(void)
{
	gds_inline_rbtree_fast_node_t *root = NULL, *inode;
	test_rbtree_fast_node_t *trfn;

	trfn = test_rbtree_fast_node_new(1);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(0);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(2);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);

	int i = 1;
	inode = gds_inline_rbtree_fast_del(&root, &i,
		(gds_rbtf_cmp_with_key_cb) test_rbtree_fast_node_cmp_with_key, NULL);
	CU_ASSERT_PTR_NOT_NULL_FATAL(inode);
	trfn = test_rbtree_fast_node_get_container(inode);
	CU_ASSERT_EQUAL(trfn->data, 1);
	test_rbtree_fast_node_free(trfn);
	
	trfn = test_rbtree_fast_node_get_container(root);
	if (trfn->data == 0) {
		CU_ASSERT_PTR_NULL(root->left);
		CU_ASSERT_PTR_NOT_NULL(root->right);
		trfn = test_rbtree_fast_node_get_container(root->right);
		CU_ASSERT_EQUAL(trfn->data, 2);
	} else {
		CU_ASSERT_PTR_NULL(root->right);
		CU_ASSERT_PTR_NOT_NULL_FATAL(root->left);
		trfn = test_rbtree_fast_node_get_container(root->left);
		CU_ASSERT_EQUAL(trfn->data, 0);
	}

	test_rbtree_fast_free(test_rbtree_fast_node_get_container(root));
}

void t_gds_inline_rbtree_fast_get_node(void)
{
	gds_inline_rbtree_fast_node_t *root = NULL, *node_inline;
	test_rbtree_fast_node_t *trfn;

	trfn = test_rbtree_fast_node_new(1);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(0);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(2);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);

	int i = 0;
	node_inline = gds_inline_rbtree_fast_get_node(root, &i,
		(gds_rbtf_cmp_with_key_cb) test_rbtree_fast_node_cmp_with_key, NULL);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node_inline)
	trfn = test_rbtree_fast_node_get_container(node_inline);
	CU_ASSERT_EQUAL(trfn->data, i);
	i = 1;
	node_inline = gds_inline_rbtree_fast_get_node(root, &i,
		(gds_rbtf_cmp_with_key_cb) test_rbtree_fast_node_cmp_with_key, NULL);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node_inline)
	trfn = test_rbtree_fast_node_get_container(node_inline);
	CU_ASSERT_EQUAL(trfn->data, i);
	i = 2;
	node_inline = gds_inline_rbtree_fast_get_node(root, &i,
		(gds_rbtf_cmp_with_key_cb) test_rbtree_fast_node_cmp_with_key, NULL);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node_inline)
	trfn = test_rbtree_fast_node_get_container(node_inline);
	CU_ASSERT_EQUAL(trfn->data, i);

	test_rbtree_fast_free(test_rbtree_fast_node_get_container(root));
}

void t_gds_inline_rbtree_fast_iterator(void)
{
	gds_inline_rbtree_fast_node_t *root = NULL, *node_inline;
	test_rbtree_fast_node_t *trfn;
	gds_iterator_t *it;

	trfn = test_rbtree_fast_node_new(1);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(0);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);
	trfn = test_rbtree_fast_node_new(2);
	gds_inline_rbtree_fast_add(&root, &(trfn->inline_node),
		(gds_rbtf_cmp_cb) test_rbtree_fast_node_cmp, NULL);

	it = gds_inline_rbtree_fast_iterator_new(root);

	CU_ASSERT(0 == gds_iterator_step(it));
	node_inline = gds_iterator_get(it);
	trfn = test_rbtree_fast_node_get_container(node_inline);
	CU_ASSERT_PTR_NOT_NULL_FATAL(trfn);
	CU_ASSERT_EQUAL(trfn->data, 0);

	CU_ASSERT(0 == gds_iterator_step(it));
	node_inline = gds_iterator_get(it);
	trfn = test_rbtree_fast_node_get_container(node_inline);
	CU_ASSERT_PTR_NOT_NULL_FATAL(trfn);
	CU_ASSERT_EQUAL(trfn->data, 1);

	CU_ASSERT(0 == gds_iterator_step(it));
	node_inline = gds_iterator_get(it);
	trfn = test_rbtree_fast_node_get_container(node_inline);
	CU_ASSERT_PTR_NOT_NULL_FATAL(trfn);
	CU_ASSERT_EQUAL(trfn->data, 2);

	CU_ASSERT(0 < gds_iterator_step(it));

	gds_iterator_free(it);
	test_rbtree_fast_free(test_rbtree_fast_node_get_container(root));
}

int main()
{
	CU_pSuite pSuite = NULL;
	int fails = 0;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Inline structure for red-black trees",
		NULL, NULL);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if(
	   (NULL == CU_add_test(pSuite, "gds_inline_rbtree_fast_add()", t_gds_inline_rbtree_fast_add))
	   || (NULL == CU_add_test(pSuite, "gds_inline_rbtree_fast_del()", t_gds_inline_rbtree_fast_del))
	   || (NULL == CU_add_test(pSuite, "gds_inline_rbtree_fast_get_node()", t_gds_inline_rbtree_fast_get_node))
	   || (NULL == CU_add_test(pSuite, "gds_inline_rbtree_fast_iterator", t_gds_inline_rbtree_fast_iterator))
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();

	fails = CU_get_number_of_failures();
	CU_cleanup_registry();

	return fails ? EXIT_FAILURE : EXIT_SUCCESS;
}
