#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <libtap13/tap.h>
#include "inline/dlist.h"
#include "lambda.h"

typedef struct {
	int data;
	gds_inline_dlist_node_t inline_node;
} test_list_node_t;

test_list_node_t * test_list_node_new(int data)
{
	test_list_node_t *tln = malloc(sizeof(test_list_node_t));
	if (tln) {
		gds_inline_dlist_node_init(&(tln->inline_node));
		tln->data = data;
	}

	return tln;
}

void test_list_node_free(test_list_node_t *tln)
{
	free(tln);
}

test_list_node_t * test_list_node_get_container(
	gds_inline_dlist_node_t *node_inline)
{
	size_t offset;

	offset = offsetof(test_list_node_t, inline_node);
	return (test_list_node_t *) ((char *)node_inline - offset);
}

//void assert_list_equal(test_list_node_t *head, int size, int compare[])
#define assert_list_equal(head, size, ...) ({ \
	int compare[] = {__VA_ARGS__}; \
	gds_inline_dlist_node_t *it; \
	test_list_node_t *tln; \
	int i; \
\
	it = &(head->inline_node); \
	i = 0; \
	while (it != NULL && i < size) { \
		tln = test_list_node_get_container(it); \
		is(tln->data, compare[i]); \
		it = gds_inline_dlist_node_get_next(it); \
		i++; \
	} \
	is(i, size); \
	is(it, NULL); \
})

void print_list_backward(gds_inline_dlist_node_t *tail)
{
	if (tail != NULL) {
		while (tail->prev != NULL) {
			printf("%p -> ", tail);
			tail = tail->prev;
		}
		printf("%p -> ", tail);
	}
	printf("NULL\n");
}

void print_list(gds_inline_dlist_node_t *head)
{
	if (head != NULL) {
		while (head->next != NULL) {
			printf("%p -> ", head);
			head = head->next;
		}
		printf("%p -> ", head);
	}
	printf("NULL\n");
	print_list_backward(head);
}

void test_list_node_inline_free(gds_inline_dlist_node_t *inline_node)
{
	free(test_list_node_get_container(inline_node));
}

void assert_list_is(gds_inline_dlist_node_t *head, size_t size,
	gds_inline_dlist_node_t *nodes[])
{
	gds_inline_dlist_node_t *it;
	unsigned int i = 0;
	int verified = 1;

	it = gds_inline_dlist_node_get_next(head);
	i = 0;
	while (it != NULL && i < size) {
		if (it != nodes[i]) {
			verified = 0;
			break;
		}
		it = gds_inline_dlist_node_get_next(it);
		i++;
	}
	if (it != NULL) {
		verified = 0;
	}

	ok(verified);
}

#define list_is(head, ...) ({ \
	gds_inline_dlist_node_t *_nodes[] = { __VA_ARGS__ }; \
	assert_list_is(head, sizeof(_nodes) / sizeof(*_nodes), _nodes); \
})

void t_gds_inline_dlist_node_init(void)
{
	gds_inline_dlist_node_t node;

	ok(0 > gds_inline_dlist_node_init(NULL));
	ok(0 == gds_inline_dlist_node_init(&node));
}

void t_gds_inline_dlist_node_set_prev(void)
{
	gds_inline_dlist_node_t node, prev;

	gds_inline_dlist_node_init(&node);
	gds_inline_dlist_node_init(&prev);

	ok(0 > gds_inline_dlist_node_set_prev(NULL, NULL));
	ok(0 > gds_inline_dlist_node_set_prev(NULL, &prev));

	ok(0 == gds_inline_dlist_node_set_prev(&node, NULL));
	ok(NULL == gds_inline_dlist_node_get_prev(&node));
	ok(0 == gds_inline_dlist_node_set_prev(&node, &prev));
	ok(&prev == gds_inline_dlist_node_get_prev(&node));
	ok(0 == gds_inline_dlist_node_set_prev(&node, NULL));
	ok(NULL == gds_inline_dlist_node_get_prev(&node));
}

void t_gds_inline_dlist_node_get_prev(void)
{
	gds_inline_dlist_node_t node, prev;

	gds_inline_dlist_node_init(&node);
	gds_inline_dlist_node_init(&prev);

	ok(NULL == gds_inline_dlist_node_get_prev(NULL));
	ok(NULL == gds_inline_dlist_node_get_prev(&node));

	gds_inline_dlist_node_set_prev(&node, &prev);
	ok(&prev == gds_inline_dlist_node_get_prev(&node));
	gds_inline_dlist_node_set_prev(&node, NULL);
	ok(NULL == gds_inline_dlist_node_get_prev(&node));
}

void t_gds_inline_dlist_node_set_next(void)
{
	gds_inline_dlist_node_t node, next;

	gds_inline_dlist_node_init(&node);
	gds_inline_dlist_node_init(&next);

	ok(0 > gds_inline_dlist_node_set_next(NULL, NULL));
	ok(0 > gds_inline_dlist_node_set_next(NULL, &next));

	ok(0 == gds_inline_dlist_node_set_next(&node, NULL));
	ok(NULL == gds_inline_dlist_node_get_next(&node));
	ok(0 == gds_inline_dlist_node_set_next(&node, &next));
	ok(&next == gds_inline_dlist_node_get_next(&node));
	ok(0 == gds_inline_dlist_node_set_next(&node, NULL));
	ok(NULL == gds_inline_dlist_node_get_next(&node));
}

void t_gds_inline_dlist_node_get_next(void)
{
	gds_inline_dlist_node_t node, next;

	gds_inline_dlist_node_init(&node);
	gds_inline_dlist_node_init(&next);

	ok(NULL == gds_inline_dlist_node_get_next(NULL));
	ok(NULL == gds_inline_dlist_node_get_next(&node));

	gds_inline_dlist_node_set_next(&node, &next);
	ok(&next == gds_inline_dlist_node_get_next(&node));
	gds_inline_dlist_node_set_next(&node, NULL);
	ok(NULL == gds_inline_dlist_node_get_next(&node));
}

void t_gds_inline_dlist_node_prepend(void)
{
	gds_inline_dlist_node_t nodes[4];
	gds_inline_dlist_node_t *newhead = NULL;

	int i;

	for (i = 0; i < 4; i++) {
		gds_inline_dlist_node_init(&nodes[i]);
	}

	ok(0 == gds_inline_dlist_node_prepend(NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_node_prepend(NULL, NULL, &newhead));
	ok(0 == gds_inline_dlist_node_prepend(NULL, &nodes[1], NULL));
	ok(0 == gds_inline_dlist_node_prepend(NULL, &nodes[1], &newhead));

	ok(0 == gds_inline_dlist_node_prepend(&nodes[0], NULL, NULL));
	ok(0 == gds_inline_dlist_node_prepend(&nodes[0], NULL, &newhead));
	is(newhead, NULL);
	ok(1 == gds_inline_dlist_node_prepend(&nodes[0], &nodes[1], &newhead));
	is(newhead, &nodes[1]);
	newhead = NULL;
	ok(2 == gds_inline_dlist_node_prepend(&nodes[2], &nodes[0], &newhead));
	is(newhead, &nodes[1]);
	newhead = NULL;
	ok(1 == gds_inline_dlist_node_prepend(&nodes[0], &nodes[3], &newhead));
	is(newhead, NULL);

	list_is(&nodes[1], &nodes[3], &nodes[0], &nodes[2]);
}

void t_gds_inline_dlist_node_append(void)
{
	gds_inline_dlist_node_t nodes[4];
	gds_inline_dlist_node_t *newhead = NULL;

	int i;

	for (i = 0; i < 4; i++) {
		gds_inline_dlist_node_init(&nodes[i]);
	}

	ok(0 == gds_inline_dlist_node_append(NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_node_append(NULL, NULL, &newhead));
	ok(0 == gds_inline_dlist_node_append(NULL, &nodes[1], NULL));
	ok(0 == gds_inline_dlist_node_append(NULL, &nodes[1], &newhead));

	ok(0 == gds_inline_dlist_node_append(&nodes[0], NULL, NULL));
	ok(0 == gds_inline_dlist_node_append(&nodes[0], NULL, &newhead));
	is(newhead, NULL);
	ok(1 == gds_inline_dlist_node_append(&nodes[0], &nodes[1], &newhead));
	is(newhead, &nodes[1]);
	newhead = NULL;
	ok(2 == gds_inline_dlist_node_append(&nodes[2], &nodes[0], &newhead));
	is(newhead, &nodes[1]);
	newhead = NULL;
	ok(1 == gds_inline_dlist_node_append(&nodes[0], &nodes[3], &newhead));
	is(newhead, NULL);

	list_is(&nodes[2], &nodes[0], &nodes[3], &nodes[1]);
}

void t_gds_inline_dlist_insert(void)
{
	gds_inline_dlist_node_t nodes[10];
	gds_inline_dlist_node_t *head1, *head2, *tail;
	int i;

	for (i = 0; i < 10; i++) {
		gds_inline_dlist_node_init(&nodes[i]);
	}

	/* Insert an empty list */
	ok(0 == gds_inline_dlist_insert(NULL, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_insert(NULL, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_insert(NULL, 0, NULL, &head1, NULL));
	ok(0 == gds_inline_dlist_insert(NULL, 0, NULL, &head1, &tail));
	ok(0 == gds_inline_dlist_insert(NULL, 1, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_insert(NULL, 1, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_insert(NULL, 1, NULL, &head1, NULL));
	ok(0 == gds_inline_dlist_insert(NULL, 1, NULL, &head1, &tail));
	ok(0 == gds_inline_dlist_insert(&nodes[0], 0, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_insert(&nodes[0], 0, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_insert(&nodes[0], 0, NULL, &head1, NULL));
	ok(0 == gds_inline_dlist_insert(&nodes[0], 0, NULL, &head1, &tail));
	ok(0 == gds_inline_dlist_insert(&nodes[0], 1, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_insert(&nodes[0], 1, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_insert(&nodes[0], 1, NULL, &head1, NULL));
	ok(0 == gds_inline_dlist_insert(&nodes[0], 1, NULL, &head1, &tail));

	/* Insert a node in an empty list */
	ok(1 == gds_inline_dlist_insert(NULL, 0, &nodes[1], NULL, NULL));
	ok(1 == gds_inline_dlist_insert(NULL, 0, &nodes[1], NULL, &tail));
	ok(1 == gds_inline_dlist_insert(NULL, 0, &nodes[1], &head1, NULL));
	ok(1 == gds_inline_dlist_insert(NULL, 0, &nodes[1], &head1, &tail));
	is(head1, &nodes[1]);
	is(tail, &nodes[1]);
	ok(1 == gds_inline_dlist_insert(NULL, 1, &nodes[1], NULL, NULL));
	ok(1 == gds_inline_dlist_insert(NULL, 1, &nodes[1], NULL, &tail));
	ok(1 == gds_inline_dlist_insert(NULL, 1, &nodes[1], &head1, NULL));
	ok(1 == gds_inline_dlist_insert(NULL, 1, &nodes[1], &head1, &tail));
	is(head1, &nodes[1]);
	is(tail, &nodes[1]);

	/* Create two list of two nodes */
	head1 = &nodes[0];
	gds_inline_dlist_insert(head1, 1, &nodes[4], &head1, NULL);
	list_is(&nodes[0], &nodes[4]);
	head2 = &nodes[1];
	gds_inline_dlist_insert(head2, 1, &nodes[2], &head2, NULL);
	list_is(&nodes[1], &nodes[2]);

	/* And insert one list into the other */
	gds_inline_dlist_insert(head1, 1, head2, &head1, NULL);
	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[4]);

	/* Add a node at a specified offset */
	gds_inline_dlist_insert(head1, 3, &nodes[3], &head1, NULL);
	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[3], &nodes[4]);

	gds_inline_dlist_insert(head1, 5, &nodes[9], &head1, NULL);
	for (i = 5; i < 9; i++) {
		gds_inline_dlist_insert(head1, i, &nodes[i], &head1, NULL);
	}

	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[3], &nodes[4],
		&nodes[5], &nodes[6], &nodes[7], &nodes[8], &nodes[9]);
}

void t_gds_inline_dlist_remove(void)
{
	gds_inline_dlist_node_t nodes[10];
	gds_inline_dlist_node_t *head, *tail = NULL;
	int i;

	for (i = 0; i < 10; i++) {
		gds_inline_dlist_node_init(&nodes[i]);
	}
	head = &nodes[0];
	for (i = 1; i < 10; i++) {
		gds_inline_dlist_insert(head, i, &nodes[i], &head, NULL);
	}

	/* Remove nodes from an empty list, will always return 0 */
	ok(0 == gds_inline_dlist_remove(NULL, 0, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 0, NULL, &head, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 0, NULL, &head, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 0, free, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 0, free, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 0, free, &head, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 0, free, &head, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 1, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 1, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 1, NULL, &head, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 1, NULL, &head, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 1, free, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 1, free, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 1, free, &head, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 0, 1, free, &head, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 0, NULL, &head, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 0, NULL, &head, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 0, free, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 0, free, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 0, free, &head, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 0, free, &head, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 1, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 1, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 1, NULL, &head, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 1, NULL, &head, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 1, free, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 1, free, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 1, free, &head, NULL));
	ok(0 == gds_inline_dlist_remove(NULL, 1, 1, free, &head, &tail));

	/* Remove 0 nodes, will always return 0 */
	ok(0 == gds_inline_dlist_remove(head, 0, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(head, 0, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(head, 0, 0, NULL, &head, NULL));
	ok(0 == gds_inline_dlist_remove(head, 0, 0, NULL, &head, &tail));
	ok(0 == gds_inline_dlist_remove(head, 0, 0, free, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(head, 0, 0, free, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(head, 0, 0, free, &head, NULL));
	ok(0 == gds_inline_dlist_remove(head, 0, 0, free, &head, &tail));
	ok(0 == gds_inline_dlist_remove(head, 1, 0, NULL, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(head, 1, 0, NULL, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(head, 1, 0, NULL, &head, NULL));
	ok(0 == gds_inline_dlist_remove(head, 1, 0, NULL, &head, &tail));
	ok(0 == gds_inline_dlist_remove(head, 1, 0, free, NULL, NULL));
	ok(0 == gds_inline_dlist_remove(head, 1, 0, free, NULL, &tail));
	ok(0 == gds_inline_dlist_remove(head, 1, 0, free, &head, NULL));
	ok(0 == gds_inline_dlist_remove(head, 1, 0, free, &head, &tail));

	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[3], &nodes[4],
		&nodes[5], &nodes[6], &nodes[7], &nodes[8], &nodes[9]);

	gds_inline_dlist_remove(head, 0, 0, NULL, &head, &tail);
	list_is(&nodes[0], &nodes[1], &nodes[2], &nodes[3], &nodes[4],
		&nodes[5], &nodes[6], &nodes[7], &nodes[8], &nodes[9]);
	is(tail, NULL);

	gds_inline_dlist_remove(head, 0, 1, NULL, &head, &tail);
	list_is(&nodes[1], &nodes[2], &nodes[3], &nodes[4], &nodes[5],
		&nodes[6], &nodes[7], &nodes[8], &nodes[9]);
	is(tail, NULL);

	gds_inline_dlist_remove(head, 3, 3, NULL, &head, &tail);
	list_is(&nodes[1], &nodes[2], &nodes[3], &nodes[7], &nodes[8],
		&nodes[9]);
	is(tail, NULL);

	gds_inline_dlist_remove(head, 3, 10, NULL, &head, &tail);
	list_is(&nodes[1], &nodes[2], &nodes[3]);
	is(tail, &nodes[3]);

	gds_inline_dlist_remove(head, 0, 10, NULL, &head, &tail);
	is(head, NULL);
	is(tail, NULL);
}

void t_gds_inline_dlist_splice(void)
{
	test_list_node_t *nodes[10];
	gds_inline_dlist_node_t *head, *tail;
	int i;
	int size;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}
	head = tail = &(nodes[0]->inline_node);
	size = 1;

	assert_list_equal(test_list_node_get_container(head), 1, 0);

	size += gds_inline_dlist_splice(head, 0, 0, NULL, &(nodes[1]->inline_node), &head, &tail);
	is(size, 2);
	assert_list_equal(test_list_node_get_container(head), 2, 1, 0);

	size += gds_inline_dlist_splice(head, 1, 0, NULL, &(nodes[2]->inline_node), &head, &tail);
	is(size, 3);
	assert_list_equal(test_list_node_get_container(head), 3, 1, 2, 0);

	size += gds_inline_dlist_splice(head, 3, 0, NULL, &(nodes[3]->inline_node), &head, &tail);
	is(size, 4);
	assert_list_equal(test_list_node_get_container(head), 4, 1, 2, 0, 3);

	size += gds_inline_dlist_splice(head, 2, 1, NULL, NULL, &head, NULL);
	is(size, 3);
	assert_list_equal(test_list_node_get_container(head), 3, 1, 2, 3);

	size += gds_inline_dlist_splice(head, 1, 1, NULL, &(nodes[4]->inline_node), &head, &tail);
	is(size, 3);
	assert_list_equal(test_list_node_get_container(head), 3, 1, 4, 3);

	size += gds_inline_dlist_splice(head, 0, 1, NULL, &(nodes[5]->inline_node), &head, &tail);
	is(size, 3);
	assert_list_equal(test_list_node_get_container(head), 3, 5, 4, 3);

	size += gds_inline_dlist_splice(head, 2, 5, NULL, &(nodes[6]->inline_node), &head, &tail);
	is(size, 3);
	assert_list_equal(test_list_node_get_container(head), 3, 5, 4, 6);

	gds_inline_dlist_splice(&(nodes[7]->inline_node), 1, 0, NULL, &(nodes[8]->inline_node), NULL, NULL);
	size += gds_inline_dlist_splice(head, 1, 2, NULL, &(nodes[7]->inline_node), &head, &tail);
	is(size, 3);
	assert_list_equal(test_list_node_get_container(head), 3, 5, 7, 8);

	size += gds_inline_dlist_splice(tail, -1, -2, NULL, &(nodes[9]->inline_node), &head, &tail);
	is(size, 2);
	assert_list_equal(test_list_node_get_container(head), 2, 9, 8);

	size += gds_inline_dlist_splice(tail, 0, -2, NULL, NULL, &head, &tail);
	is(size, 0);
	assert_list_equal(test_list_node_get_container(head), 0);

	for (i = 0; i < 10; i++) {
		free(nodes[i]);
		nodes[i] = test_list_node_new(i);
	}
	head = tail = &(nodes[0]->inline_node);
	size = 1;

	size += gds_inline_dlist_splice(head, 0, 0, NULL, &(nodes[1]->inline_node), &head, &tail);
	is(size, 2);
	size += gds_inline_dlist_splice(head, 0, 0, NULL, &(nodes[2]->inline_node), &head, &tail);
	is(size, 3);
	size += gds_inline_dlist_splice(head, 0, 0, NULL, &(nodes[3]->inline_node), &head, &tail);
	is(size, 4);
	size += gds_inline_dlist_splice(head, 0, 0, NULL, &(nodes[4]->inline_node), &head, &tail);
	is(size, 5);
	assert_list_equal(test_list_node_get_container(head), 5, 4, 3, 2, 1, 0);

	size += gds_inline_dlist_splice(&(nodes[2]->inline_node), -1, 3, NULL, &(nodes[5]->inline_node), &head, &tail);
	is(size, 3);
	assert_list_equal(test_list_node_get_container(head), 3, 4, 5, 0);

	for (i = 0; i < 10; i++) {
		free(nodes[i]);
	}
}

void t_gds_inline_dlist_get(void)
{
	test_list_node_t *nodes[10];
	test_list_node_t *tln;
	gds_inline_dlist_node_t *node_inline, *fi, *node_inline2;
	int i, j;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}
	fi = &(nodes[0]->inline_node);
	for (i = 1; i < 10; i++) {
		gds_inline_dlist_splice(fi, i, 0, NULL,
			&(nodes[i]->inline_node), NULL, NULL);
	}

	for (i = 0; i < 10; i++) {
		node_inline = gds_inline_dlist_get(fi, i);
		isnt(node_inline, NULL);
		tln = test_list_node_get_container(node_inline);
		isnt(tln, NULL);
		is(tln->data, i);
		for (j = -i; j < 10-i; j++) {
			node_inline2 = gds_inline_dlist_get(node_inline, j);
			isnt(node_inline2, NULL);
			tln = test_list_node_get_container(node_inline2);
			isnt(tln, NULL);
			is(tln->data, i+j);
		}
	}

	node_inline = gds_inline_dlist_get(fi, -1);
	is(node_inline, NULL);
	node_inline = gds_inline_dlist_get(fi, 10);
	is(node_inline, NULL);

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

void t_gds_inline_dlist_get_head(void)
{
	test_list_node_t *nodes[10];
	gds_inline_dlist_node_t *idn, *head;
	int i;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}
	head = &(nodes[0]->inline_node);
	for (i = 1; i < 10; i++) {
		gds_inline_dlist_splice(head, i, 0, NULL,
			&(nodes[i]->inline_node), NULL, NULL);
	}

	idn = gds_inline_dlist_get_head(head);
	isnt(idn, NULL);
	is(idn, head);

	for (i = 0; i < 10; i++) {
		idn = gds_inline_dlist_get_head(&(nodes[i]->inline_node));
		isnt(idn, NULL);
		is(idn, head);
	}

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

void t_gds_inline_dlist_get_tail(void)
{
	test_list_node_t *nodes[10];
	gds_inline_dlist_node_t *idn, *head, *tail;
	int i;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}
	head = &(nodes[0]->inline_node);
	for (i = 1; i < 10; i++) {
		gds_inline_dlist_splice(head, i, 0, NULL,
			&(nodes[i]->inline_node), NULL, &tail);
	}

	idn = gds_inline_dlist_get_tail(tail);
	isnt(idn, NULL);
	is(idn, tail);

	for (i = 0; i < 10; i++) {
		idn = gds_inline_dlist_get_tail(&(nodes[i]->inline_node));
		isnt(idn, NULL);
		is(idn, tail);
	}

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

void t_gds_inline_dlist_size(void)
{
	test_list_node_t *nodes[10];
	gds_inline_dlist_node_t *head;
	int i;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}
	head = &(nodes[0]->inline_node);
	is(gds_inline_dlist_size(head), 1);
	for (i = 1; i < 10; i++) {
		gds_inline_dlist_splice(head, i, 0, NULL,
			&(nodes[i]->inline_node), NULL, NULL);
		is(gds_inline_dlist_size(head), i+1);
	}

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

void t_gds_inline_dlist_map(void)
{
	test_list_node_t *nodes[10];
	gds_inline_dlist_node_t *head;
	int i;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}
	head = &(nodes[0]->inline_node);
	is(gds_inline_dlist_size(head), 1);
	for (i = 1; i < 10; i++) {
		gds_inline_dlist_splice(head, i, 0, NULL,
			&(nodes[i]->inline_node), NULL, NULL);
	}

	int sum = 0;
	gds_inline_dlist_map(head, gds_lambda(void, (gds_inline_dlist_node_t *n) {
		sum += test_list_node_get_container(n)->data;
	}));
	is(sum, 1+2+3+4+5+6+7+8+9);

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

int main()
{
	plan(610);

	t_gds_inline_dlist_node_init();
	t_gds_inline_dlist_node_set_prev();
	t_gds_inline_dlist_node_get_prev();
	t_gds_inline_dlist_node_set_next();
	t_gds_inline_dlist_node_get_next();
	t_gds_inline_dlist_node_prepend();
	t_gds_inline_dlist_node_append();
	t_gds_inline_dlist_insert();
	t_gds_inline_dlist_remove();
	t_gds_inline_dlist_splice();
	t_gds_inline_dlist_get();
	t_gds_inline_dlist_get_head();
	t_gds_inline_dlist_get_tail();
	t_gds_inline_dlist_size();
	t_gds_inline_dlist_map();

	return EXIT_SUCCESS;
}
