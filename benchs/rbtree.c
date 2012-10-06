#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/time.h>
#include "rbtree.h"
#include "compact_rbtree_node.h"
#include "compact_rbtree.h"
#include <eina-1/eina/eina_rbtree.h>

typedef struct {
	intptr_t i;
} test_t;

intptr_t getkey(test_t *t)
{
	return (t != NULL) ? t->i : 0;
}

int32_t cmpkey(intptr_t a, intptr_t b)
{
	return a-b;
}

test_t * test_new(intptr_t i)
{
	test_t *t = malloc(sizeof(test_t));
	if(t != NULL)
		t->i = i;
	return t;
}

test_t * test_alloc(test_t *t)
{
	test_t *c = malloc(sizeof(test_t));
	c->i = t->i;
	return c;
}

void test_free(test_t *t)
{
	free(t);
}

typedef struct {
	EINA_RBTREE;
	int i;
} test2_t;

test2_t * test2_new(int i)
{
	test2_t *t = malloc(sizeof(test2_t));
	if(t) t->i = i;
	return t;
}

Eina_Rbtree_Direction cmp(const Eina_Rbtree *left, const Eina_Rbtree *right)
{
	test2_t *p, *q;

	p = EINA_RBTREE_CONTAINER_GET(left, test2_t);
	q = EINA_RBTREE_CONTAINER_GET(right, test2_t);

	if(p->i > q->i)
		return EINA_RBTREE_LEFT;
	
	return EINA_RBTREE_RIGHT;
}

int cmpkey2(const Eina_Rbtree *node, intptr_t key)
{
	test2_t *p;

	p = EINA_RBTREE_CONTAINER_GET(node, test2_t);

	if(p->i > key)
		return -1;
	else if(p->i < key)
		return 1;
	
	return 0;
}

void timeval_diff(struct timeval *start, struct timeval *end, struct timeval *diff)
{
	diff->tv_sec = end->tv_sec - start->tv_sec;
	diff->tv_usec = end->tv_usec - start->tv_usec;
	if (diff->tv_usec < 0) {
		diff->tv_usec += 1000000;
		diff->tv_sec--;
	}
}

int main()
{
	const intptr_t max = 10000000;
	intptr_t i;
	struct timeval g_start, start, end, diff;
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb) cmpkey;
	gds_getkey_cb getkey_cb = (gds_getkey_cb) getkey;
	gds_free_cb free_cb = (gds_free_cb) test_free;

	/* gds_compact_rbtree */
	printf("gds_compact_rbtree\n");
	gettimeofday(&g_start, NULL);
	printf("\tInserting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	gds_compact_rbtree_node_t *croot = NULL;
	test_t *t;
	for(i=0; i<max; i++) {
		t = test_new(i);
		gds_compact_rbtree_add(&croot, t, getkey_cb, cmpkey_cb, NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tGetting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		gds_compact_rbtree_get(croot, (void *)i, getkey_cb, cmpkey_cb,
			NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tDeleting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		gds_compact_rbtree_del(&croot, (void*)i, getkey_cb, cmpkey_cb,
			free_cb);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	timeval_diff(&g_start, &end, &diff);
	printf("\tTotal time: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tEstimated size: %ld bytes\n", max * (sizeof(gds_compact_rbtree_node_t) + sizeof(test_t)));

	/* gds_rbtree */
	printf("gds_rbtree\n");
	gettimeofday(&g_start, NULL);
	printf("\tInserting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	gds_rbtree_node_t *root = NULL;
	for(i=0; i<max; i++) {
		t = test_new(i);
		gds_rbtree_add(&root, t, getkey_cb, cmpkey_cb, NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tGetting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		gds_rbtree_get(root, (void *)i, getkey_cb, cmpkey_cb, NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tDeleting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		gds_rbtree_del(&root, (void*)i, getkey_cb, cmpkey_cb, free_cb);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	timeval_diff(&g_start, &end, &diff);
	printf("\tTotal time: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tEstimated size: %ld bytes\n", max * (sizeof(gds_rbtree_node_t) + sizeof(test_t)));
	
	/* eina */
	printf("eina\n");
	gettimeofday(&g_start, NULL);
	printf("\tInserting %ld nodes\n", max);
	gettimeofday(&start, NULL);
	Eina_Rbtree *eroot = NULL;
	test2_t *t2;
	for(i=0; i<max; i++) {
		t2 = test2_new(i);
		eroot = eina_rbtree_inline_insert(eroot, EINA_RBTREE_GET(t2), EINA_RBTREE_CMP_NODE_CB(cmp), NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tGetting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	Eina_Rbtree *n = NULL;
	for(i=0; i<max; i++) {
		n = eina_rbtree_inline_lookup(eroot, (void *)i, 1, EINA_RBTREE_CMP_KEY_CB(cmpkey2), NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tDeleting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		n = eina_rbtree_inline_lookup(eroot, (void *)i, 1, EINA_RBTREE_CMP_KEY_CB(cmpkey2), NULL);
		eroot = eina_rbtree_inline_remove(eroot, n, EINA_RBTREE_CMP_NODE_CB(cmp), NULL);
		t2 = EINA_RBTREE_CONTAINER_GET(n, test2_t);
		free(t2);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	timeval_diff(&g_start, &end, &diff);
	printf("\tTotal time: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tEstimated size: %ld bytes\n", max * (sizeof(test2_t)));

	return 0;
}
