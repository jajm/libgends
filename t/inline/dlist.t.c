#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "inline/dlist.h"
#include "lambda.h"
#include "../tap.h"

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
		is(tln->data, compare[i], NULL); \
		it = gds_inline_dlist_node_get_next(it); \
		i++; \
	} \
	is(i, size, NULL); \
	isnull(it, NULL); \
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

	size += gds_inline_dlist_splice(head, 0, 0, NULL, NULL, &(nodes[1]->inline_node), &head, &tail);
	is(size, 2, NULL);
	assert_list_equal(test_list_node_get_container(head), 2, 1, 0);

	size += gds_inline_dlist_splice(head, 1, 0, NULL, NULL, &(nodes[2]->inline_node), &head, &tail);
	is(size, 3, NULL);
	assert_list_equal(test_list_node_get_container(head), 3, 1, 2, 0);

	size += gds_inline_dlist_splice(head, 3, 0, NULL, NULL, &(nodes[3]->inline_node), &head, &tail);
	is(size, 4, NULL);
	assert_list_equal(test_list_node_get_container(head), 4, 1, 2, 0, 3);

	size += gds_inline_dlist_splice(head, 2, 1, NULL, NULL, NULL, &head, NULL);
	is(size, 3, NULL);
	assert_list_equal(test_list_node_get_container(head), 3, 1, 2, 3);

	size += gds_inline_dlist_splice(head, 1, 1, NULL, NULL, &(nodes[4]->inline_node), &head, &tail);
	is(size, 3, NULL);
	assert_list_equal(test_list_node_get_container(head), 3, 1, 4, 3);

	size += gds_inline_dlist_splice(head, 0, 1, NULL, NULL, &(nodes[5]->inline_node), &head, &tail);
	is(size, 3, NULL);
	assert_list_equal(test_list_node_get_container(head), 3, 5, 4, 3);

	size += gds_inline_dlist_splice(head, 2, 5, NULL, NULL, &(nodes[6]->inline_node), &head, &tail);
	is(size, 3, NULL);
	assert_list_equal(test_list_node_get_container(head), 3, 5, 4, 6);

	gds_inline_dlist_splice(&(nodes[7]->inline_node), 1, 0, NULL, NULL, &(nodes[8]->inline_node), NULL, NULL);
	size += gds_inline_dlist_splice(head, 1, 2, NULL, NULL, &(nodes[7]->inline_node), &head, &tail);
	is(size, 3, NULL);
	assert_list_equal(test_list_node_get_container(head), 3, 5, 7, 8);

	size += gds_inline_dlist_splice(tail, -1, -2, NULL, NULL, &(nodes[9]->inline_node), &head, &tail);
	is(size, 2, NULL);
	assert_list_equal(test_list_node_get_container(head), 2, 9, 8);

	size += gds_inline_dlist_splice(tail, 0, -2, NULL, NULL, NULL, &head, &tail);
	is(size, 0, NULL);
	assert_list_equal(test_list_node_get_container(head), 0);

	for (i = 0; i < 10; i++) {
		free(nodes[i]);
		nodes[i] = test_list_node_new(i);
	}
	head = tail = &(nodes[0]->inline_node);
	size = 1;

	size += gds_inline_dlist_splice(head, 0, 0, NULL, NULL, &(nodes[1]->inline_node), &head, &tail);
	is(size, 2, NULL);
	size += gds_inline_dlist_splice(head, 0, 0, NULL, NULL, &(nodes[2]->inline_node), &head, &tail);
	is(size, 3, NULL);
	size += gds_inline_dlist_splice(head, 0, 0, NULL, NULL, &(nodes[3]->inline_node), &head, &tail);
	is(size, 4, NULL);
	size += gds_inline_dlist_splice(head, 0, 0, NULL, NULL, &(nodes[4]->inline_node), &head, &tail);
	is(size, 5, NULL);
	assert_list_equal(test_list_node_get_container(head), 5, 4, 3, 2, 1, 0);

	size += gds_inline_dlist_splice(&(nodes[2]->inline_node), -1, 3, NULL, NULL, &(nodes[5]->inline_node), &head, &tail);
	is(size, 3, NULL);
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
		gds_inline_dlist_splice(fi, i, 0, NULL, NULL,
			&(nodes[i]->inline_node), NULL, NULL);
	}

	for (i = 0; i < 10; i++) {
		node_inline = gds_inline_dlist_get(fi, i);
		isntnull(node_inline, NULL);
		tln = test_list_node_get_container(node_inline);
		isntnull(tln, NULL);
		is(tln->data, i, NULL);
		for (j = -i; j < 10-i; j++) {
			node_inline2 = gds_inline_dlist_get(node_inline, j);
			isntnull(node_inline2, NULL);
			tln = test_list_node_get_container(node_inline2);
			isntnull(tln, NULL);
			is(tln->data, i+j, NULL);
		}
	}

	node_inline = gds_inline_dlist_get(fi, -1);
	isnull(node_inline, NULL);
	node_inline = gds_inline_dlist_get(fi, 10);
	isnull(node_inline, NULL);

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
		gds_inline_dlist_splice(head, i, 0, NULL, NULL,
			&(nodes[i]->inline_node), NULL, NULL);
	}

	idn = gds_inline_dlist_get_head(head);
	isntnull(idn, NULL);
	is(idn, head, NULL);

	for (i = 0; i < 10; i++) {
		idn = gds_inline_dlist_get_head(&(nodes[i]->inline_node));
		isntnull(idn, NULL);
		is(idn, head, NULL);
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
		gds_inline_dlist_splice(head, i, 0, NULL, NULL,
			&(nodes[i]->inline_node), NULL, &tail);
	}

	idn = gds_inline_dlist_get_tail(tail);
	isntnull(idn, NULL);
	is(idn, tail, NULL);

	for (i = 0; i < 10; i++) {
		idn = gds_inline_dlist_get_tail(&(nodes[i]->inline_node));
		isntnull(idn, NULL);
		is(idn, tail, NULL);
	}

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

void t_gds_inline_dlist_size(void)
{
	test_list_node_t *nodes[10];
	gds_inline_dlist_node_t *head;
	unsigned int i;

	for (i = 0; i < 10; i++) {
		nodes[i] = test_list_node_new(i);
	}
	head = &(nodes[0]->inline_node);
	is(gds_inline_dlist_size(head), 1, NULL);
	for (i = 1; i < 10; i++) {
		gds_inline_dlist_splice(head, i, 0, NULL, NULL,
			&(nodes[i]->inline_node), NULL, NULL);
		is(gds_inline_dlist_size(head), i+1, NULL);
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
	is(gds_inline_dlist_size(head), 1, NULL);
	for (i = 1; i < 10; i++) {
		gds_inline_dlist_splice(head, i, 0, NULL, NULL,
			&(nodes[i]->inline_node), NULL, NULL);
	}

	int sum = 0;
	gds_inline_dlist_map(head, lambda(void, (gds_inline_dlist_node_t *n, int i, int *sum) {
		if (i) {} // prevent compiler warnings
		*sum += test_list_node_get_container(n)->data;
	}), &sum);
	is(sum, 1+2+3+4+5+6+7+8+9, NULL);

	for (i = 0; i < 10; i++) {
		test_list_node_free(nodes[i]);
	}
}

int main()
{
	plan(464);

	t_gds_inline_dlist_splice();
	t_gds_inline_dlist_get();
	t_gds_inline_dlist_get_head();
	t_gds_inline_dlist_get_tail();
	t_gds_inline_dlist_size();
	t_gds_inline_dlist_map();

	return EXIT_SUCCESS;
}
