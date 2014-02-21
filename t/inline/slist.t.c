#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <libtap13/tap.h>
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
		is(tln->data, compare[i]);
		it = gds_inline_slist_node_get_next(it);
		i++;
	}
	is(i, size);
	is(it, NULL);
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
	gds_inline_slist_insert(fi, 1, &(nodes[4]->inline_node), &fi, NULL);
	assert_list_equal(nodes[0], 2, (int[]) {0,4});
	fi2 = &(nodes[1]->inline_node);
	gds_inline_slist_insert(fi2, 1, &(nodes[2]->inline_node), &fi2, NULL);
	assert_list_equal(nodes[1], 2, (int[]) {1,2});
	/* And insert one list into the other */
	gds_inline_slist_insert(fi, 1, fi2, &fi, NULL);
	assert_list_equal(nodes[0], 4, (int[]) {0,1,2,4});
	/* Add a node at a specified offset */
	gds_inline_slist_insert(fi, 3, &(nodes[3]->inline_node), &fi, NULL);

	assert_list_equal(nodes[0], 5, (int[]) {0,1,2,3,4});

	gds_inline_slist_insert(fi, 5, &(nodes[9]->inline_node), &fi, NULL);
	for (i = 5; i < 9; i++) {
		gds_inline_slist_insert(fi, i, &(nodes[i]->inline_node), &fi,
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
		gds_inline_slist_insert(fi, i, &(nodes[i]->inline_node), &fi,
			NULL);
	}

	assert_list_equal(test_list_node_get_container(fi), 10, (int[]) {0,1,2,3,4,5,6,7,8,9});

	gds_inline_slist_remove(fi, 0, 0, test_list_node_inline_free,
		NULL, &fi, &li);
	assert_list_equal(test_list_node_get_container(fi), 10, (int[]) {0,1,2,3,4,5,6,7,8,9});
	is(li, NULL);

	gds_inline_slist_remove(fi, 0, 1, test_list_node_inline_free,
		NULL, &fi, &li);
	assert_list_equal(test_list_node_get_container(fi), 9, (int[]) {1,2,3,4,5,6,7,8,9});
	is(li, NULL);

	gds_inline_slist_remove(fi, 3, 3, test_list_node_inline_free,
		NULL, &fi, &li);
	assert_list_equal(test_list_node_get_container(fi), 6, (int[]) {1,2,3,7,8,9});
	is(li, NULL);

	gds_inline_slist_remove(fi, 3, 10, test_list_node_inline_free,
		NULL, &fi, &li);
	assert_list_equal(test_list_node_get_container(fi), 3, (int[]) {1,2,3});
	is(li, &(nodes[3]->inline_node));

	gds_inline_slist_remove(fi, 0, 10, test_list_node_inline_free,
		NULL, &fi, &li);
	assert_list_equal(test_list_node_get_container(fi), 0, (int[]) {});
	is(li, NULL);
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
		gds_inline_slist_insert(fi, i, &(nodes[i]->inline_node), &fi,
			NULL);
	}

	for (i = 0; i < 10; i++) {
		node_inline = gds_inline_slist_get(fi, i);
		isnt(node_inline, NULL);
		tln = test_list_node_get_container(node_inline);
		isnt(tln, NULL);
		is(tln->data, i);
	}

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

int main()
{
	plan(118);

	t_gds_inline_slist_insert();
	t_gds_inline_slist_remove();
	t_gds_inline_slist_get();

	return EXIT_SUCCESS;
}
