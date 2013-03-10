#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <CUnit/Basic.h>
#include "inline/slist.h"

typedef struct {
	int data;
	gds_inline_slist_node_t inline_node;
} test_list_node_t;

test_list_node_t * test_list_node_new(int data)
{
	test_list_node_t *tln = malloc(sizeof(test_list_node_t));
	if (tln) {
		gds_inline_slist_node_init(&(tln->inline_node));
		tln->data = data;
	}

	return tln;
}

void test_list_node_free(test_list_node_t *tln)
{
	free(tln);
}

test_list_node_t * test_list_node_get_container(
	gds_inline_slist_node_t *node_inline)
{
	size_t offset;

	offset = offsetof(test_list_node_t, inline_node);
	return (test_list_node_t *) ((char *)node_inline - offset);
}

void assert_list_equal(test_list_node_t *head, int size, int compare[])
{
	gds_inline_slist_node_t *it;
	test_list_node_t *tln;
	int i;

	it = &(head->inline_node);
	i = 0;
	while (it != NULL && i < size) {
		tln = test_list_node_get_container(it);
		CU_ASSERT_EQUAL(tln->data, compare[i]);
		it = gds_inline_slist_node_get_next(it);
		i++;
	}
	CU_ASSERT_EQUAL(i, size);
	CU_ASSERT_PTR_NULL(it);
}

void t_gds_inline_slist_insert(void)
{
	test_list_node_t *nodes[10];
	gds_inline_slist_node_t *fi, *fi2;
	int i;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}

	/* Create two list of two nodes */
	fi = &(nodes[0]->inline_node);
	gds_inline_slist_insert(&fi, 1, &(nodes[4]->inline_node), NULL);
	fi2 = &(nodes[1]->inline_node);
	gds_inline_slist_insert(&fi2, 1, &(nodes[2]->inline_node), NULL);
	/* And insert one list into the other */
	gds_inline_slist_insert(&fi, 1, fi2, NULL);
	/* Add a node at a specified offset */
	gds_inline_slist_insert(&fi, 3, &(nodes[3]->inline_node), NULL);

	assert_list_equal(nodes[0], 5, (int[]) {0,1,2,3,4});

	gds_inline_slist_insert(&fi, 5, &(nodes[9]->inline_node), NULL);
	for (i = 5; i < 9; i++) {
		gds_inline_slist_insert(&fi, i, &(nodes[i]->inline_node),
			NULL);
	}

	assert_list_equal(nodes[0], 10, (int[]) {0,1,2,3,4,5,6,7,8,9});

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

void test_list_node_inline_free(gds_inline_slist_node_t *inline_node)
{
	free(test_list_node_get_container(inline_node));
}

void t_gds_inline_slist_remove(void)
{
	test_list_node_t *nodes[10];
	gds_inline_slist_node_t *fi, *li = NULL;
	int i;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}
	fi = &(nodes[0]->inline_node);
	for (i = 1; i < 10; i++) {
		gds_inline_slist_insert(&fi, i, &(nodes[i]->inline_node),
			NULL);
	}

	assert_list_equal(test_list_node_get_container(fi), 10, (int[]) {0,1,2,3,4,5,6,7,8,9});

	gds_inline_slist_remove(&fi, 0, 0, test_list_node_inline_free,
		NULL, &li);
	assert_list_equal(test_list_node_get_container(fi), 10, (int[]) {0,1,2,3,4,5,6,7,8,9});
	CU_ASSERT_PTR_NULL(li);

	gds_inline_slist_remove(&fi, 0, 1, test_list_node_inline_free,
		NULL, &li);
	assert_list_equal(test_list_node_get_container(fi), 9, (int[]) {1,2,3,4,5,6,7,8,9});
	CU_ASSERT_PTR_NULL(li);

	gds_inline_slist_remove(&fi, 3, 3, test_list_node_inline_free,
		NULL, &li);
	assert_list_equal(test_list_node_get_container(fi), 6, (int[]) {1,2,3,7,8,9});
	CU_ASSERT_PTR_NULL(li);

	gds_inline_slist_remove(&fi, 3, 10, test_list_node_inline_free,
		NULL, &li);
	assert_list_equal(test_list_node_get_container(fi), 3, (int[]) {1,2,3});
	CU_ASSERT_PTR_EQUAL(li, &(nodes[3]->inline_node));

	gds_inline_slist_remove(&fi, 0, 10, test_list_node_inline_free,
		NULL, &li);
	assert_list_equal(test_list_node_get_container(fi), 0, (int[]) {});
	CU_ASSERT_PTR_NULL(li);
}

void t_gds_inline_slist_get(void)
{
	test_list_node_t *nodes[10];
	test_list_node_t *tln;
	gds_inline_slist_node_t *node_inline, *fi;
	int i;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}
	fi = &(nodes[0]->inline_node);
	for (i = 1; i < 10; i++) {
		gds_inline_slist_insert(&fi, i, &(nodes[i]->inline_node),
			NULL);
	}

	for (i = 0; i < 10; i++) {
		node_inline = gds_inline_slist_get(fi, i);
		CU_ASSERT_PTR_NOT_NULL(node_inline);
		tln = test_list_node_get_container(node_inline);
		CU_ASSERT_PTR_NOT_NULL(tln);
		CU_ASSERT_EQUAL(tln->data, i);
	}

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Inline structure for singly linked list",
		NULL, NULL);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if(
	   (NULL == CU_add_test(pSuite, "gds_inline_slist_insert()", t_gds_inline_slist_insert))
	   || (NULL == CU_add_test(pSuite, "gds_inline_slist_remove()", t_gds_inline_slist_remove))
	   || (NULL == CU_add_test(pSuite, "gds_inline_slist_get()", t_gds_inline_slist_get))
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
