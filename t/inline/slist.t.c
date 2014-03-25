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

void assert_list_is(gds_inline_slist_node_t *head, size_t size,
	gds_inline_slist_node_t *nodes[])
{
	gds_inline_slist_node_t *it;
	unsigned int i = 0;
	int verified = 1;

	it = gds_inline_slist_node_get_next(head);
	i = 0;
	while (it != NULL && i < size) {
		if (it != nodes[i]) {
			verified = 0;
			break;
		}
		it = gds_inline_slist_node_get_next(it);
		i++;
	}
	if (it != NULL) {
		verified = 0;
	}

	ok(verified);
}

#define list_is(head, ...) ({ \
	gds_inline_slist_node_t *_nodes[] = { __VA_ARGS__ }; \
	assert_list_is(head, sizeof(_nodes) / sizeof(*_nodes), _nodes); \
})

void t_gds_inline_slist_node_init(void)
{
	gds_inline_slist_node_t node;

	ok(0 > gds_inline_slist_node_init(NULL));
	ok(0 == gds_inline_slist_node_init(&node));
}

void t_gds_inline_slist_node_set_next(void)
{
	gds_inline_slist_node_t node1, node2;

	gds_inline_slist_node_init(&node1);
	gds_inline_slist_node_init(&node2);

	ok(0 > gds_inline_slist_node_set_next(NULL, NULL));
	ok(0 > gds_inline_slist_node_set_next(NULL, &node2));

	ok(0 == gds_inline_slist_node_set_next(&node1, NULL));
	ok(0 == gds_inline_slist_node_set_next(&node1, &node2));
	ok(&node2 == gds_inline_slist_node_get_next(&node1));
	ok(0 == gds_inline_slist_node_set_next(&node1, NULL));
	ok(NULL == gds_inline_slist_node_get_next(&node1));
}

void t_gds_inline_slist_node_get_next(void)
{
	gds_inline_slist_node_t node, node2;

	gds_inline_slist_node_init(&node);

	ok(NULL == gds_inline_slist_node_get_next(NULL));
	ok(NULL == gds_inline_slist_node_get_next(&node));

	gds_inline_slist_node_init(&node2);
	gds_inline_slist_node_set_next(&node, &node2);

	ok(&node2 == gds_inline_slist_node_get_next(&node));
}

void t_gds_inline_slist_node_append(void)
{
	gds_inline_slist_node_t nodes[4];
	gds_inline_slist_node_t *newtail = NULL;

	int i;

	for (i = 0; i < 4; i++) {
		gds_inline_slist_node_init(&nodes[i]);
	}

	ok(0 == gds_inline_slist_node_append(NULL, NULL, NULL));
	ok(0 == gds_inline_slist_node_append(NULL, NULL, &newtail));
	ok(0 == gds_inline_slist_node_append(NULL, &nodes[1], NULL));
	ok(0 == gds_inline_slist_node_append(NULL, &nodes[1], &newtail));

	ok(0 == gds_inline_slist_node_append(&nodes[0], NULL, NULL));
	ok(0 == gds_inline_slist_node_append(&nodes[0], NULL, &newtail));
	is(newtail, NULL);
	ok(1 == gds_inline_slist_node_append(&nodes[0], &nodes[1], &newtail));
	is(newtail, &nodes[1]);
	newtail = NULL;
	ok(2 == gds_inline_slist_node_append(&nodes[2], &nodes[0], &newtail));
	is(newtail, &nodes[1]);
	newtail = NULL;
	ok(1 == gds_inline_slist_node_append(&nodes[0], &nodes[3], &newtail));
	is(newtail, NULL);

	list_is(&nodes[2], &nodes[0], &nodes[3], &nodes[1]);
}

void t_gds_inline_slist_insert(void)
{
	gds_inline_slist_node_t nodes[10];
	gds_inline_slist_node_t *head1, *head2, *tail;
	int i;

	for (i = 0; i < 10; i++) {
		gds_inline_slist_node_init(&nodes[i]);
	}

	/* Insert an empty list */
	ok(0 == gds_inline_slist_insert(NULL, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_insert(NULL, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_insert(NULL, 0, NULL, &head1, NULL));
	ok(0 == gds_inline_slist_insert(NULL, 0, NULL, &head1, &tail));
	ok(0 == gds_inline_slist_insert(NULL, 1, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_insert(NULL, 1, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_insert(NULL, 1, NULL, &head1, NULL));
	ok(0 == gds_inline_slist_insert(NULL, 1, NULL, &head1, &tail));
	ok(0 == gds_inline_slist_insert(&nodes[0], 0, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_insert(&nodes[0], 0, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_insert(&nodes[0], 0, NULL, &head1, NULL));
	ok(0 == gds_inline_slist_insert(&nodes[0], 0, NULL, &head1, &tail));
	ok(0 == gds_inline_slist_insert(&nodes[0], 1, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_insert(&nodes[0], 1, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_insert(&nodes[0], 1, NULL, &head1, NULL));
	ok(0 == gds_inline_slist_insert(&nodes[0], 1, NULL, &head1, &tail));

	/* Insert a node in an empty list */
	ok(1 == gds_inline_slist_insert(NULL, 0, &nodes[1], NULL, NULL));
	ok(1 == gds_inline_slist_insert(NULL, 0, &nodes[1], NULL, &tail));
	ok(1 == gds_inline_slist_insert(NULL, 0, &nodes[1], &head1, NULL));
	ok(1 == gds_inline_slist_insert(NULL, 0, &nodes[1], &head1, &tail));
	is(head1, &nodes[1]);
	is(tail, &nodes[1]);
	ok(1 == gds_inline_slist_insert(NULL, 1, &nodes[1], NULL, NULL));
	ok(1 == gds_inline_slist_insert(NULL, 1, &nodes[1], NULL, &tail));
	ok(1 == gds_inline_slist_insert(NULL, 1, &nodes[1], &head1, NULL));
	ok(1 == gds_inline_slist_insert(NULL, 1, &nodes[1], &head1, &tail));
	is(head1, &nodes[1]);
	is(tail, &nodes[1]);

	/* Create two list of two nodes */
	head1 = &nodes[0];
	gds_inline_slist_insert(head1, 1, &nodes[4], &head1, NULL);
	list_is(&nodes[0], &nodes[4]);
	head2 = &nodes[1];
	gds_inline_slist_insert(head2, 1, &nodes[2], &head2, NULL);
	list_is(&nodes[1], &nodes[2]);

	/* And insert one list into the other */
	gds_inline_slist_insert(head1, 1, head2, &head1, NULL);
	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[4]);

	/* Add a node at a specified offset */
	gds_inline_slist_insert(head1, 3, &nodes[3], &head1, NULL);
	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[3], &nodes[4]);

	gds_inline_slist_insert(head1, 5, &nodes[9], &head1, NULL);
	for (i = 5; i < 9; i++) {
		gds_inline_slist_insert(head1, i, &nodes[i], &head1, NULL);
	}

	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[3], &nodes[4],
		&nodes[5], &nodes[6], &nodes[7], &nodes[8], &nodes[9]);
}

void t_gds_inline_slist_remove(void)
{
	gds_inline_slist_node_t nodes[10];
	gds_inline_slist_node_t *head, *tail = NULL;
	int i;

	for (i = 0; i < 10; i++) {
		gds_inline_slist_node_init(&nodes[i]);
	}
	head = &nodes[0];
	for (i = 1; i < 10; i++) {
		gds_inline_slist_insert(head, i, &nodes[i], &head, NULL);
	}

	/* Remove nodes from an empty list, will always return 0 */
	ok(0 == gds_inline_slist_remove(NULL, 0, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 0, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 0, 0, NULL, &head, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 0, 0, NULL, &head, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 0, 0, free, NULL, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 0, 0, free, NULL, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 0, 0, free, &head, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 0, 0, free, &head, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 0, 1, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 0, 1, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 0, 1, NULL, &head, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 0, 1, NULL, &head, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 0, 1, free, NULL, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 0, 1, free, NULL, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 0, 1, free, &head, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 0, 1, free, &head, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 1, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 1, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 1, 0, NULL, &head, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 1, 0, NULL, &head, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 1, 0, free, NULL, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 1, 0, free, NULL, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 1, 0, free, &head, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 1, 0, free, &head, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 1, 1, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 1, 1, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 1, 1, NULL, &head, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 1, 1, NULL, &head, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 1, 1, free, NULL, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 1, 1, free, NULL, &tail));
	ok(0 == gds_inline_slist_remove(NULL, 1, 1, free, &head, NULL));
	ok(0 == gds_inline_slist_remove(NULL, 1, 1, free, &head, &tail));

	/* Remove 0 nodes, will always return 0 */
	ok(0 == gds_inline_slist_remove(head, 0, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_remove(head, 0, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_remove(head, 0, 0, NULL, &head, NULL));
	ok(0 == gds_inline_slist_remove(head, 0, 0, NULL, &head, &tail));
	ok(0 == gds_inline_slist_remove(head, 0, 0, free, NULL, NULL));
	ok(0 == gds_inline_slist_remove(head, 0, 0, free, NULL, &tail));
	ok(0 == gds_inline_slist_remove(head, 0, 0, free, &head, NULL));
	ok(0 == gds_inline_slist_remove(head, 0, 0, free, &head, &tail));
	ok(0 == gds_inline_slist_remove(head, 1, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_slist_remove(head, 1, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_slist_remove(head, 1, 0, NULL, &head, NULL));
	ok(0 == gds_inline_slist_remove(head, 1, 0, NULL, &head, &tail));
	ok(0 == gds_inline_slist_remove(head, 1, 0, free, NULL, NULL));
	ok(0 == gds_inline_slist_remove(head, 1, 0, free, NULL, &tail));
	ok(0 == gds_inline_slist_remove(head, 1, 0, free, &head, NULL));
	ok(0 == gds_inline_slist_remove(head, 1, 0, free, &head, &tail));

	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[3], &nodes[4],
		&nodes[5], &nodes[6], &nodes[7], &nodes[8], &nodes[9]);

	gds_inline_slist_remove(head, 0, 0, NULL, &head, &tail);
	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[3], &nodes[4],
		&nodes[5], &nodes[6], &nodes[7], &nodes[8], &nodes[9]);
	is(tail, NULL);

	gds_inline_slist_remove(head, 0, 1, NULL, &head, &tail);
	list_is(&nodes[1], &nodes[2], &nodes[3], &nodes[4], &nodes[5],
		&nodes[6], &nodes[7], &nodes[8], &nodes[9]);
	is(tail, NULL);

	gds_inline_slist_remove(head, 3, 3, NULL, &head, &tail);
	list_is(&nodes[1], &nodes[2], &nodes[3], &nodes[7], &nodes[8],
		&nodes[9]);
	is(tail, NULL);

	gds_inline_slist_remove(head, 3, 10, NULL, &head, &tail);
	list_is(&nodes[1], &nodes[2], &nodes[3]);
	is(tail, &nodes[3]);

	gds_inline_slist_remove(head, 0, 10, NULL, &head, &tail);
	is(head, NULL);
	is(tail, NULL);
}

void t_gds_inline_slist_get(void)
{
	gds_inline_slist_node_t nodes[10];
	gds_inline_slist_node_t *head, *n;
	int i;

	for (i = 0; i < 10; i++) {
		gds_inline_slist_node_init(&nodes[i]);
	}

	head = &nodes[0];
	for (i = 1; i < 10; i++) {
		gds_inline_slist_insert(head, i, &nodes[i], &head, NULL);
	}

	ok(NULL == gds_inline_slist_get(NULL, 0));
	ok(NULL == gds_inline_slist_get(head, 10));

	for (i = 0; i < 10; i++) {
		n = gds_inline_slist_get(head, i);
		isnt(n, NULL);
		is(n, &nodes[i]);
	}
}

int main()
{
	plan(140);

	t_gds_inline_slist_node_init();
	t_gds_inline_slist_node_set_next();
	t_gds_inline_slist_node_get_next();
	t_gds_inline_slist_node_append();
	t_gds_inline_slist_insert();
	t_gds_inline_slist_remove();
	t_gds_inline_slist_get();

	return EXIT_SUCCESS;
}
