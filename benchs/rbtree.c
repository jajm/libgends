#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/time.h>
#include "inline/rbtree_fast.h"
#include "inline/rbtree.h"
#include <eina-1/eina/eina_rbtree.h>

#define container_of(ptr, type, member) ({                   \
	const __typeof__( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - offsetof(type,member) );  \
})

typedef struct {
	intptr_t i;
	gds_inline_rbtree_node_t rbtree;
} test_rbtree_node_t;

test_rbtree_node_t * trn_new(intptr_t i)
{
	test_rbtree_node_t *t = malloc(sizeof(test_rbtree_node_t));
	if(t != NULL) {
		t->i = i;
		gds_inline_rbtree_node_init(&(t->rbtree));
	}
	return t;
}

test_rbtree_node_t *trn_container_of(gds_inline_rbtree_node_t *inode)
{
	return container_of(inode, test_rbtree_node_t, rbtree);
}

int trn_cmp(gds_inline_rbtree_node_t *inode1, gds_inline_rbtree_node_t *inode2)
{
	test_rbtree_node_t *trn1, *trn2;

	trn1 = trn_container_of(inode1);
	trn2 = trn_container_of(inode2);

	return (trn1->i) - (trn2->i);
}

int trn_cmp_with_key(gds_inline_rbtree_node_t *inode, const int *key)
{
	test_rbtree_node_t *trn;

	trn = trn_container_of(inode);

	return (*key) - trn->i;
}

void trn_free(test_rbtree_node_t *t)
{
	free(t);
}

typedef struct {
	intptr_t i;
	gds_inline_rbtree_fast_node_t rbtree;
} test_rbtree_fast_node_t;

test_rbtree_fast_node_t * trfn_new(intptr_t i)
{
	test_rbtree_fast_node_t *t = malloc(sizeof(test_rbtree_fast_node_t));
	if(t != NULL) {
		t->i = i;
		gds_inline_rbtree_fast_node_init(&(t->rbtree));
	}
	return t;
}

test_rbtree_fast_node_t *trfn_container_of(gds_inline_rbtree_fast_node_t *inode)
{
	return container_of(inode, test_rbtree_fast_node_t, rbtree);
}

int trfn_cmp(gds_inline_rbtree_fast_node_t *inode1, gds_inline_rbtree_fast_node_t *inode2)
{
	test_rbtree_fast_node_t *trfn1, *trfn2;

	trfn1 = trfn_container_of(inode1);
	trfn2 = trfn_container_of(inode2);

	return (trfn1->i) - (trfn2->i);
}

int trfn_cmp_with_key(gds_inline_rbtree_fast_node_t *inode, const int *key)
{
	test_rbtree_fast_node_t *trfn;

	trfn = trfn_container_of(inode);

	return (*key) - trfn->i;
}

void trfn_free(test_rbtree_fast_node_t *t)
{
	free(t);
}

typedef struct {
	EINA_RBTREE;
	int i;
} test_eina_rbtree_node_t;

test_eina_rbtree_node_t * tern_new(int i)
{
	test_eina_rbtree_node_t *tern = malloc(sizeof(test_eina_rbtree_node_t));
	if (tern) tern->i = i;
	return tern;
}

Eina_Rbtree_Direction tern_cmp(const Eina_Rbtree *left, const Eina_Rbtree *right)
{
	test_eina_rbtree_node_t *p, *q;

	p = EINA_RBTREE_CONTAINER_GET(left, test_eina_rbtree_node_t);
	q = EINA_RBTREE_CONTAINER_GET(right, test_eina_rbtree_node_t);

	if(p->i > q->i)
		return EINA_RBTREE_LEFT;
	
	return EINA_RBTREE_RIGHT;
}

int tern_cmpkey(const Eina_Rbtree *node, int *key)
{
	test_eina_rbtree_node_t *p;

	p = EINA_RBTREE_CONTAINER_GET(node, test_eina_rbtree_node_t);

	if(p->i > (*key))
		return -1;
	else if(p->i < (*key))
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
	test_rbtree_node_t *trn;
	test_rbtree_fast_node_t *trfn;
	gds_inline_rbtree_node_t *irn;
	gds_inline_rbtree_fast_node_t *irfn;

	/* gds_inline_rbtree */
	printf("gds_inline_rbtree\n");
	gettimeofday(&g_start, NULL);
	printf("\tInserting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	gds_inline_rbtree_node_t *root = NULL;
	for(i=0; i<max; i++) {
		trn = trn_new(i);
		gds_inline_rbtree_add(&root, &(trn->rbtree), trn_cmp, NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tGetting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		gds_inline_rbtree_get_node(root, &i, trn_cmp_with_key, NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tDeleting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		irn = gds_inline_rbtree_del(&root, &i, trn_cmp_with_key, NULL);
		trn = trn_container_of(irn);
		trn_free(trn);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	timeval_diff(&g_start, &end, &diff);
	printf("\tTotal time: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tEstimated size: %ld bytes\n", max * (sizeof(test_rbtree_node_t)));

	/* gds_inline_rbtree_fast */
	printf("gds_inline_rbtree_fast\n");
	gettimeofday(&g_start, NULL);
	printf("\tInserting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	gds_inline_rbtree_fast_node_t *froot = NULL;
	for(i=0; i<max; i++) {
		trfn = trfn_new(i);
		gds_inline_rbtree_fast_add(&froot, &(trfn->rbtree), trfn_cmp, NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tGetting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		gds_inline_rbtree_fast_get_node(froot, &i, trfn_cmp_with_key, NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tDeleting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		irfn = gds_inline_rbtree_fast_del(&froot, &i, trfn_cmp_with_key, NULL);
		trfn = trfn_container_of(irfn);
		trfn_free(trfn);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	timeval_diff(&g_start, &end, &diff);
	printf("\tTotal time: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tEstimated size: %ld bytes\n", max * (sizeof(test_rbtree_fast_node_t)));
	
	/* eina */
	printf("eina\n");
	gettimeofday(&g_start, NULL);
	printf("\tInserting %ld nodes\n", max);
	gettimeofday(&start, NULL);
	Eina_Rbtree *eroot = NULL;
	test_eina_rbtree_node_t *tern;
	for(i=0; i<max; i++) {
		tern = tern_new(i);
		eroot = eina_rbtree_inline_insert(eroot, EINA_RBTREE_GET(tern), EINA_RBTREE_CMP_NODE_CB(tern_cmp), NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tGetting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	Eina_Rbtree *n = NULL;
	for(i=0; i<max; i++) {
		n = eina_rbtree_inline_lookup(eroot, &i, 1, EINA_RBTREE_CMP_KEY_CB(tern_cmpkey), NULL);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tDeleting %ld nodes...\n", max);
	gettimeofday(&start, NULL);
	for(i=0; i<max; i++) {
		n = eina_rbtree_inline_lookup(eroot, &i, 1, EINA_RBTREE_CMP_KEY_CB(tern_cmpkey), NULL);
		eroot = eina_rbtree_inline_remove(eroot, n, EINA_RBTREE_CMP_NODE_CB(tern_cmp), NULL);
		tern = EINA_RBTREE_CONTAINER_GET(n, test_eina_rbtree_node_t);
		free(tern);
	}
	gettimeofday(&end, NULL);
	timeval_diff(&start, &end, &diff);
	printf("\tTime: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	timeval_diff(&g_start, &end, &diff);
	printf("\tTotal time: %lds %ldus\n", diff.tv_sec, diff.tv_usec);
	printf("\tEstimated size: %ld bytes\n", max * (sizeof(test_eina_rbtree_node_t)));

	return 0;
}
