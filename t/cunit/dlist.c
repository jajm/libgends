#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "types.h"
#include "dlist.h"

typedef struct {
	int32_t i;
	char c;
	double f;
} test_structure_t;

test_structure_t * test_alloc(test_structure_t *t)
{
	test_structure_t *copy = malloc(sizeof(test_structure_t));
	if(copy && t) {
		copy->i = t->i;
		copy->c = t->c;
		copy->f = t->f;
		return copy;
	}
	return NULL;
}

void test_free(test_structure_t *t)
{
	free(t);
}

int8_t test_cmp(test_structure_t *t1, test_structure_t *t2)
{
	if(t1 == NULL && t2 == NULL) {
		return 0;
	} else if(t1 == NULL && t2 != NULL) {
		return -1;
	} else if(t1 != NULL && t2 == NULL) {
		return 1;
	} else if(t1->i != t2->i) {
		return t1->i - t2->i;
	} else if(t1->c != t2->c) {
		return t1->c - t2->c;
	}
	return memcmp(&(t1->f), &(t2->f), sizeof(double));
}

int init_suite(void){
	gds_types_init(0);
	return 0;
}

int clean_suite(void){
	gds_types_free();
	return 0;
}

void t_gds_dlist_new(void)
{
	gds_dlist_t *l;

	CU_ASSERT(NULL == gds_dlist_new(NULL));
	CU_ASSERT(NULL != (l = gds_dlist_new("")));
	gds_dlist_free(l, false);
	CU_ASSERT(NULL != (l = gds_dlist_new("type")));
	gds_dlist_free(l, false);
}

void t_gds_dlist_empty(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };

	gds_type_register(type_name);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(true == gds_dlist_empty(NULL));
	CU_ASSERT(true == gds_dlist_empty(l));

	assert(NULL != gds_dlist_add_first(l, &test, false));
	CU_ASSERT(false == gds_dlist_empty(l));
	gds_dlist_free(l, false);

	gds_type_unregister(type_name);
}


void t_gds_dlist_first(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node;

	gds_type_register(type_name);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_first(NULL));
	CU_ASSERT(NULL == gds_dlist_first(l));

	node = gds_dlist_add_first(l, &test, false);
	assert(node != NULL);
	CU_ASSERT(NULL != gds_dlist_first(l));
	CU_ASSERT(node == gds_dlist_first(l));

	gds_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_dlist_last(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node;

	gds_type_register(type_name);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_last(NULL));
	CU_ASSERT(NULL == gds_dlist_last(l));

	node = gds_dlist_add_last(l, &test, false);
	assert(node != NULL);
	CU_ASSERT(NULL != gds_dlist_last(l));
	CU_ASSERT(node == gds_dlist_last(l));

	gds_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_dlist_next(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_next(NULL, NULL));
	CU_ASSERT(NULL == gds_dlist_next(l, NULL));
	
	node = gds_dlist_add_first(l, &test, false);
	assert(node != NULL);
	CU_ASSERT(NULL == gds_dlist_next(NULL, node));
	CU_ASSERT(NULL == gds_dlist_next(l, node));

	node2 = gds_dlist_add_first(l, &test, false);
	assert(node2 != NULL);
	CU_ASSERT(NULL == gds_dlist_next(NULL, node));
	CU_ASSERT(NULL == gds_dlist_next(l, node));
	CU_ASSERT(NULL == gds_dlist_next(NULL, node2));
	CU_ASSERT(node == gds_dlist_next(l, node2));

	gds_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_dlist_prev(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_prev(NULL, NULL));
	CU_ASSERT(NULL == gds_dlist_prev(l, NULL));
	
	node = gds_dlist_add_last(l, &test, false);
	assert(node != NULL);
	CU_ASSERT(NULL == gds_dlist_prev(NULL, node));
	CU_ASSERT(NULL == gds_dlist_prev(l, node));

	node2 = gds_dlist_add_last(l, &test, false);
	assert(node2 != NULL);
	CU_ASSERT(NULL == gds_dlist_prev(NULL, node));
	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(NULL == gds_dlist_prev(NULL, node2));
	CU_ASSERT(node == gds_dlist_prev(l, node2));

	gds_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_dlist_has_next(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(false == gds_dlist_has_next(NULL, NULL));
	CU_ASSERT(false == gds_dlist_has_next(l, NULL));

	node = gds_dlist_add_first(l, &test, false);
	assert(node != NULL);
	CU_ASSERT(false == gds_dlist_has_next(NULL, node));
	CU_ASSERT(false == gds_dlist_has_next(l, node));

	node2 = gds_dlist_add_first(l, &test, false);
	assert(node2 != NULL);
	CU_ASSERT(false == gds_dlist_has_next(NULL, node));
	CU_ASSERT(false == gds_dlist_has_next(l, node));
	CU_ASSERT(false == gds_dlist_has_next(NULL, node2));
	CU_ASSERT(true == gds_dlist_has_next(l, node2));

	gds_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_dlist_has_prev(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(false == gds_dlist_has_prev(NULL, NULL));
	CU_ASSERT(false == gds_dlist_has_prev(l, NULL));

	node = gds_dlist_add_last(l, &test, false);
	assert(node != NULL);
	CU_ASSERT(false == gds_dlist_has_prev(NULL, node));
	CU_ASSERT(false == gds_dlist_has_prev(l, node));

	node2 = gds_dlist_add_last(l, &test, false);
	assert(node2 != NULL);
	CU_ASSERT(false == gds_dlist_has_prev(NULL, node));
	CU_ASSERT(false == gds_dlist_has_prev(l, node));
	CU_ASSERT(false == gds_dlist_has_prev(NULL, node2));
	CU_ASSERT(true == gds_dlist_has_prev(l, node2));

	gds_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_dlist_add_before(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3, *node4;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_add_before(NULL, NULL, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_before(NULL, NULL, NULL, true));
	CU_ASSERT(NULL == gds_dlist_add_before(NULL, NULL, &test, false));
	CU_ASSERT(NULL == gds_dlist_add_before(NULL, NULL, &test, true));

	node = gds_dlist_node_new(&test, NULL);
	assert(node != NULL);
	CU_ASSERT(NULL == gds_dlist_add_before(NULL, node, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_before(NULL, node, NULL, true));
	CU_ASSERT(NULL == gds_dlist_add_before(NULL, node, &test, false));
	CU_ASSERT(NULL == gds_dlist_add_before(NULL, node, &test, true));
	gds_dlist_node_free(node, NULL);

	CU_ASSERT(NULL == gds_dlist_add_before(l, NULL, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_before(l, NULL, NULL, true));
	CU_ASSERT(NULL != (node4 = gds_dlist_add_before(l, NULL, test_ptr, false)));
	CU_ASSERT(NULL != (node2 = gds_dlist_add_before(l, NULL, &test, true)));

	test_ptr = calloc(1, sizeof(test_structure_t));

	CU_ASSERT(NULL == gds_dlist_add_before(l, node, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_before(l, node, NULL, true));
	CU_ASSERT(NULL != (node3 = gds_dlist_add_before(l, node4, test_ptr, false)));
	CU_ASSERT(NULL != (node = gds_dlist_add_before(l, node2, &test, true)));

	CU_ASSERT(NULL == gds_dlist_next(l, node4));
	CU_ASSERT(node4 == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_prev(l, node4));
	CU_ASSERT(node4 == gds_dlist_last(l));

	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_add_after(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3, *node4;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_add_after(NULL, NULL, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_after(NULL, NULL, NULL, true));
	CU_ASSERT(NULL == gds_dlist_add_after(NULL, NULL, &test, false));
	CU_ASSERT(NULL == gds_dlist_add_after(NULL, NULL, &test, true));

	node = gds_dlist_node_new(&test, NULL);
	assert(node != NULL);
	CU_ASSERT(NULL == gds_dlist_add_after(NULL, node, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_after(NULL, node, NULL, true));
	CU_ASSERT(NULL == gds_dlist_add_after(NULL, node, &test, false));
	CU_ASSERT(NULL == gds_dlist_add_after(NULL, node, &test, true));
	gds_dlist_node_free(node, NULL);

	CU_ASSERT(NULL == gds_dlist_add_after(l, NULL, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_after(l, NULL, NULL, true));
	CU_ASSERT(NULL != (node = gds_dlist_add_after(l, NULL, test_ptr, false)));
	CU_ASSERT(NULL != (node3 = gds_dlist_add_after(l, NULL, &test, true)));

	test_ptr = calloc(1, sizeof(test_structure_t));

	CU_ASSERT(NULL == gds_dlist_add_after(l, node, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_after(l, node, NULL, true));
	CU_ASSERT(NULL != (node4 = gds_dlist_add_after(l, node3, test_ptr, false)));
	CU_ASSERT(NULL != (node2 = gds_dlist_add_after(l, node, &test, true)));

	CU_ASSERT(NULL == gds_dlist_next(l, node4));
	CU_ASSERT(node4 == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_prev(l, node4));
	CU_ASSERT(node4 == gds_dlist_last(l));

	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_add_first(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_add_first(NULL, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_first(NULL, NULL, true));
	CU_ASSERT(NULL == gds_dlist_add_first(NULL, &test, false));
	CU_ASSERT(NULL == gds_dlist_add_first(NULL, &test, true));
	CU_ASSERT(NULL == gds_dlist_add_first(l, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_first(l, NULL, true));
	CU_ASSERT(NULL != (node2 = gds_dlist_add_first(l, test_ptr, false)));
	CU_ASSERT(NULL != (node = gds_dlist_add_first(l, &test, true)));

	CU_ASSERT(NULL == gds_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_dlist_last(l));

	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_add_last(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_add_last(NULL, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_last(NULL, NULL, true));
	CU_ASSERT(NULL == gds_dlist_add_last(NULL, &test, false));
	CU_ASSERT(NULL == gds_dlist_add_last(NULL, &test, true));
	CU_ASSERT(NULL == gds_dlist_add_last(l, NULL, false));
	CU_ASSERT(NULL == gds_dlist_add_last(l, NULL, true));
	CU_ASSERT(NULL != (node = gds_dlist_add_last(l, test_ptr, false)));
	CU_ASSERT(NULL != (node2 = gds_dlist_add_last(l, &test, true)));

	CU_ASSERT(NULL == gds_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_dlist_last(l));

	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_it_add_before(void)
{
	gds_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3, *node4;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);
	it = gds_dlist_iterator_new(l);
	assert(it != NULL);

	node4 = gds_dlist_add_first(l, &test, true);
	node2 = gds_dlist_add_first(l, &test, true);
	gds_iterator_reset(it);
	gds_iterator_step(it);

	CU_ASSERT(NULL == gds_dlist_it_add_before(NULL, NULL, false));
	CU_ASSERT(NULL == gds_dlist_it_add_before(NULL, NULL, true));
	CU_ASSERT(NULL == gds_dlist_it_add_before(NULL, &test, false));
	CU_ASSERT(NULL == gds_dlist_it_add_before(NULL, &test, true));
	CU_ASSERT(NULL == gds_dlist_it_add_before(it, NULL, false));
	CU_ASSERT(NULL == gds_dlist_it_add_before(it, NULL, true));
	CU_ASSERT(NULL != (node = gds_dlist_it_add_before(it, test_ptr, false)));
	gds_iterator_step(it);
	CU_ASSERT(NULL != (node3 = gds_dlist_it_add_before(it, &test, true)));

	CU_ASSERT(NULL == gds_dlist_next(l, node4));
	CU_ASSERT(node4 == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_prev(l, node4));
	CU_ASSERT(node4 == gds_dlist_last(l));

	gds_dlist_iterator_free(it);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_it_add_after(void)
{
	gds_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3, *node4;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);
	it = gds_dlist_iterator_new(l);
	assert(it != NULL);

	node4 = gds_dlist_add_first(l, &test, true);
	node = gds_dlist_add_first(l, &test, true);
	gds_iterator_reset(it);
	gds_iterator_step(it);

	CU_ASSERT(NULL == gds_dlist_it_add_after(NULL, NULL, false));
	CU_ASSERT(NULL == gds_dlist_it_add_after(NULL, NULL, true));
	CU_ASSERT(NULL == gds_dlist_it_add_after(NULL, &test, false));
	CU_ASSERT(NULL == gds_dlist_it_add_after(NULL, &test, true));
	CU_ASSERT(NULL == gds_dlist_it_add_after(it, NULL, false));
	CU_ASSERT(NULL == gds_dlist_it_add_after(it, NULL, true));
	CU_ASSERT(NULL != (node2 = gds_dlist_it_add_after(it, test_ptr, false)));
	gds_iterator_step(it);
	CU_ASSERT(NULL != (node3 = gds_dlist_it_add_after(it, &test, true)));

	CU_ASSERT(NULL == gds_dlist_next(l, node4));
	CU_ASSERT(node4 == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_prev(l, node4));
	CU_ASSERT(node4 == gds_dlist_last(l));

	gds_dlist_iterator_free(it);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_pop(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;
	void *data;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	assert((node = gds_dlist_add_last(l, test_ptr, true)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);
	assert((node3 = gds_dlist_add_last(l, test_ptr, true)) != NULL);

	assert(NULL == gds_dlist_next(l, node3));
	assert(node3 == gds_dlist_next(l, node2));
	assert(node2 == gds_dlist_next(l, node));
	assert(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_pop(NULL, NULL));
	CU_ASSERT(NULL == gds_dlist_pop(NULL, node));
	CU_ASSERT(NULL == gds_dlist_pop(l, NULL));

	data = gds_dlist_pop(l, node2);
	CU_ASSERT(NULL != data)
	CU_ASSERT(0 == memcmp(data, &test, sizeof(test_structure_t)));

	CU_ASSERT(NULL == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_last(l));

	test_free((test_structure_t *)data);
	test_free(test_ptr);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_pop_first(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;
	void *data;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_pop_first(NULL));
	CU_ASSERT(NULL == gds_dlist_pop_first(l));

	assert((node = gds_dlist_add_last(l, &test, true)) != NULL);
	assert((node2 = gds_dlist_add_last(l, test_ptr, true)) != NULL);
	assert((node3 = gds_dlist_add_last(l, test_ptr, true)) != NULL);

	assert(NULL == gds_dlist_next(l, node3));
	assert(node3 == gds_dlist_next(l, node2));
	assert(node2 == gds_dlist_next(l, node));
	assert(node == gds_dlist_first(l));

	data = gds_dlist_pop_first(l);
	CU_ASSERT(NULL != data)
	CU_ASSERT(0 == memcmp(data, &test, sizeof(test_structure_t)));

	CU_ASSERT(NULL == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_last(l));

	test_free((test_structure_t *)data);
	test_free(test_ptr);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_pop_last(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;
	void *data;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_pop_last(NULL));
	CU_ASSERT(NULL == gds_dlist_pop_last(l));

	assert((node = gds_dlist_add_last(l, test_ptr, true)) != NULL);
	assert((node2 = gds_dlist_add_last(l, test_ptr, true)) != NULL);
	assert((node3 = gds_dlist_add_last(l, &test, true)) != NULL);

	assert(NULL == gds_dlist_next(l, node3));
	assert(node3 == gds_dlist_next(l, node2));
	assert(node2 == gds_dlist_next(l, node));
	assert(node == gds_dlist_first(l));

	data = gds_dlist_pop_last(l);
	CU_ASSERT(NULL != data)
	CU_ASSERT(0 == memcmp(data, &test, sizeof(test_structure_t)));

	CU_ASSERT(NULL == gds_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_dlist_last(l));

	test_free((test_structure_t *)data);
	test_free(test_ptr);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_it_pop(void)
{
	gds_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;
	void *data;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);
	it = gds_dlist_iterator_new(l);
	assert(it != NULL);

	CU_ASSERT(NULL == gds_dlist_it_pop(NULL));
	CU_ASSERT(NULL == gds_dlist_it_pop(it));

	assert((node = gds_dlist_add_last(l, test_ptr, true)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);
	assert((node3 = gds_dlist_add_last(l, test_ptr, true)) != NULL);

	assert(NULL == gds_dlist_next(l, node3));
	assert(node3 == gds_dlist_next(l, node2));
	assert(node2 == gds_dlist_next(l, node));
	assert(node == gds_dlist_first(l));

	gds_iterator_reset(it);
	gds_iterator_step(it);
	gds_iterator_step(it);

	data = gds_dlist_it_pop(it);
	CU_ASSERT(NULL != data)
	if(data != NULL) {
		CU_ASSERT(0 == memcmp(data, &test, sizeof(test_structure_t)));
	}

	CU_ASSERT(NULL == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_last(l));

	test_free((test_structure_t *)data);
	gds_dlist_iterator_free(it);
	test_free(test_ptr);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_del(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	assert((node = gds_dlist_add_last(l, test_ptr, false)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);
	assert((node3 = gds_dlist_add_last(l, test_ptr, true)) != NULL);

	assert(NULL == gds_dlist_next(l, node3));
	assert(node3 == gds_dlist_next(l, node2));
	assert(node2 == gds_dlist_next(l, node));
	assert(node == gds_dlist_first(l));

	CU_ASSERT(0 > gds_dlist_del(NULL, NULL, false));
	CU_ASSERT(0 > gds_dlist_del(NULL, NULL, true));
	CU_ASSERT(0 > gds_dlist_del(NULL, node, false));
	CU_ASSERT(0 > gds_dlist_del(NULL, node, true));
	CU_ASSERT(0 > gds_dlist_del(l, NULL, false));
	CU_ASSERT(0 > gds_dlist_del(l, NULL, true));
	CU_ASSERT(0 == gds_dlist_del(l, node, false));
	CU_ASSERT(0 == gds_dlist_del(l, node2, true));

	CU_ASSERT(NULL == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_last(l));

	test_free(test_ptr);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_del_first(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	assert((node = gds_dlist_add_last(l, test_ptr, false)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);
	assert((node3 = gds_dlist_add_last(l, test_ptr, true)) != NULL);

	assert(NULL == gds_dlist_next(l, node3));
	assert(node3 == gds_dlist_next(l, node2));
	assert(node2 == gds_dlist_next(l, node));
	assert(node == gds_dlist_first(l));

	CU_ASSERT(0 > gds_dlist_del_first(NULL, false));
	CU_ASSERT(0 > gds_dlist_del_first(NULL, true));
	CU_ASSERT(0 == gds_dlist_del_first(l, false));
	CU_ASSERT(0 == gds_dlist_del_first(l, true));

	CU_ASSERT(NULL == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_last(l));

	test_free(test_ptr);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_del_last(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	assert((node = gds_dlist_add_last(l, test_ptr, true)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);
	assert((node3 = gds_dlist_add_last(l, test_ptr, false)) != NULL);

	assert(NULL == gds_dlist_next(l, node3));
	assert(node3 == gds_dlist_next(l, node2));
	assert(node2 == gds_dlist_next(l, node));
	assert(node == gds_dlist_first(l));

	CU_ASSERT(0 > gds_dlist_del_last(NULL, false));
	CU_ASSERT(0 > gds_dlist_del_last(NULL, true));
	CU_ASSERT(0 == gds_dlist_del_last(l, false));
	CU_ASSERT(0 == gds_dlist_del_last(l, true));

	CU_ASSERT(NULL == gds_dlist_next(l, node));
	CU_ASSERT(node == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node));
	CU_ASSERT(node == gds_dlist_last(l));

	test_free(test_ptr);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_it_del(void)
{
	gds_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);
	it = gds_dlist_iterator_new(l);
	assert(it != NULL);

	CU_ASSERT(0 > gds_dlist_it_del(NULL, false));
	CU_ASSERT(0 > gds_dlist_it_del(NULL, true));
	CU_ASSERT(0 > gds_dlist_it_del(it, false));
	CU_ASSERT(0 > gds_dlist_it_del(it, true));

	assert((node = gds_dlist_add_last(l, test_ptr, false)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);
	assert((node3 = gds_dlist_add_last(l, test_ptr, true)) != NULL);

	assert(NULL == gds_dlist_next(l, node3));
	assert(node3 == gds_dlist_next(l, node2));
	assert(node2 == gds_dlist_next(l, node));
	assert(node == gds_dlist_first(l));

	gds_iterator_reset(it);
	CU_ASSERT(0 > gds_dlist_it_del(it, false));
	CU_ASSERT(0 > gds_dlist_it_del(it, true));
	gds_iterator_step(it);
	CU_ASSERT(0 == gds_dlist_it_del(it, false));
	CU_ASSERT(0 == gds_dlist_it_del(it, true));

	CU_ASSERT(NULL == gds_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_dlist_first(l));

	CU_ASSERT(NULL == gds_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_dlist_last(l));

	gds_dlist_iterator_free(it);
	test_free(test_ptr);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_get(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	void *data = NULL, *data2 = NULL;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	assert((node = gds_dlist_add_last(l, test_ptr, false)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);

	CU_ASSERT(NULL == gds_dlist_get(NULL, NULL, false));
	CU_ASSERT(NULL == gds_dlist_get(NULL, NULL, true));
	CU_ASSERT(NULL == gds_dlist_get(NULL, node, false));
	CU_ASSERT(NULL == gds_dlist_get(NULL, node, true));
	CU_ASSERT(NULL == gds_dlist_get(l, NULL, false));
	CU_ASSERT(NULL == gds_dlist_get(l, NULL, true));
	CU_ASSERT(NULL != (data = gds_dlist_get(l, node, false)));
	CU_ASSERT(NULL != (data2 = gds_dlist_get(l, node, true)));

	CU_ASSERT(data == test_ptr);
	CU_ASSERT(data != data2);
	CU_ASSERT(0 == memcmp(data, data2, sizeof(test_structure_t)));

	gds_dlist_del(l, node, false);
	test_free((test_structure_t *)data2);
	test_free(test_ptr);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_get_first(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	void *data, *data2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_get_first(NULL, false));
	CU_ASSERT(NULL == gds_dlist_get_first(NULL, true));
	CU_ASSERT(NULL == gds_dlist_get_first(l, false));
	CU_ASSERT(NULL == gds_dlist_get_first(l, true));

	assert((node = gds_dlist_add_last(l, test_ptr, false)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);

	CU_ASSERT(NULL != (data = gds_dlist_get_first(l, false)));
	CU_ASSERT(NULL != (data2 = gds_dlist_get_first(l, true)));

	CU_ASSERT(data == test_ptr);
	CU_ASSERT(data != data2);
	CU_ASSERT(0 == memcmp(data, data2, sizeof(test_structure_t)));

	test_free((test_structure_t *)data2);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_get_last(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	void *data, *data2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_get_last(NULL, false));
	CU_ASSERT(NULL == gds_dlist_get_last(NULL, true));
	CU_ASSERT(NULL == gds_dlist_get_last(l, false));
	CU_ASSERT(NULL == gds_dlist_get_last(l, true));

	assert((node = gds_dlist_add_last(l, test_ptr, false)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);

	CU_ASSERT(NULL != (data = gds_dlist_get_last(l, false)));
	CU_ASSERT(NULL != (data2 = gds_dlist_get_last(l, true)));

	CU_ASSERT(data != test_ptr);
	CU_ASSERT(data != data2);
	CU_ASSERT(0 == memcmp(data, data2, sizeof(test_structure_t)));

	test_free((test_structure_t *)data2);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_it_get(void)
{
	gds_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	void *data, *data2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);
	it = gds_dlist_iterator_new(l);
	assert(it != NULL);

	CU_ASSERT(NULL == gds_dlist_it_get(NULL, false));
	CU_ASSERT(NULL == gds_dlist_it_get(NULL, true));
	CU_ASSERT(NULL == gds_dlist_it_get(it, false));
	CU_ASSERT(NULL == gds_dlist_it_get(it, true));

	assert((node = gds_dlist_add_last(l, test_ptr, false)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);

	gds_iterator_reset(it);
	gds_iterator_step(it);
	CU_ASSERT(NULL != (data = gds_dlist_it_get(it, false)));
	CU_ASSERT(NULL != (data2 = gds_dlist_it_get(it, true)));

	CU_ASSERT(data == test_ptr);
	CU_ASSERT(data != data2);
	if(data != NULL && data2 != NULL) {
		CU_ASSERT(0 == memcmp(data, data2, sizeof(test_structure_t)));
	}

	gds_dlist_iterator_free(it);
	test_free((test_structure_t *)data2);
	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_dlist_iterator_new(void)
{
	gds_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };

	gds_type_register(type_name);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_iterator_new(NULL));
	CU_ASSERT(NULL != (it = gds_dlist_iterator_new(l)));
	CU_ASSERT(0 == gds_iterator_reset(it));
	CU_ASSERT(0 < gds_iterator_step(it));
	CU_ASSERT(NULL == gds_iterator_get(it));

	gds_dlist_add_last(l, &test, false);
	CU_ASSERT(0 == gds_iterator_step(it));
	CU_ASSERT(&test == gds_iterator_get(it));

	CU_ASSERT(0 < gds_iterator_step(it));

	gds_dlist_iterator_free(it);
	gds_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_dlist_reverse_iterator_new(void)
{
	gds_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };

	gds_type_register(type_name);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	CU_ASSERT(NULL == gds_dlist_reverse_iterator_new(NULL));
	CU_ASSERT(NULL != (it = gds_dlist_reverse_iterator_new(l)));
	CU_ASSERT(0 == gds_iterator_reset(it));
	CU_ASSERT(0 < gds_iterator_step(it));
	CU_ASSERT(NULL == gds_iterator_get(it));

	gds_dlist_add_last(l, &test, false);
	CU_ASSERT(0 == gds_iterator_step(it));
	CU_ASSERT(&test == gds_iterator_get(it));

	CU_ASSERT(0 < gds_iterator_step(it));

	gds_dlist_iterator_free(it);
	gds_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_dlist_chk(void)
{
	gds_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	gds_func_ptr_t cmp_f = (gds_func_ptr_t)&test_cmp;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func(type_name, "cmp", cmp_f);

	l = gds_dlist_new(type_name);
	assert(l != NULL);

	assert((node = gds_dlist_add_last(l, test_ptr, false)) != NULL);
	assert((node2 = gds_dlist_add_last(l, &test, true)) != NULL);

	CU_ASSERT(NULL == gds_dlist_chk(NULL, NULL));
	CU_ASSERT(NULL == gds_dlist_chk(NULL, &test));
	CU_ASSERT(NULL == gds_dlist_chk(l, NULL));
	if(cmp_f(&test, &test) == 0) {
		CU_ASSERT(node2 == gds_dlist_chk(l, &test));
	} else {
		CU_ASSERT(NULL == gds_dlist_chk(l, &test));
	}

	gds_dlist_free(l, true);
	gds_type_unregister(type_name);
}

int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Doubly linked list data structure",
		init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if(
	   (NULL == CU_add_test(pSuite, "gds_dlist_new()", t_gds_dlist_new))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_empty()", t_gds_dlist_empty))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_first()", t_gds_dlist_first))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_last()", t_gds_dlist_last))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_next()", t_gds_dlist_next))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_prev()", t_gds_dlist_prev))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_has_next()", t_gds_dlist_has_next))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_has_prev()", t_gds_dlist_has_prev))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_add_before()", t_gds_dlist_add_before))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_add_after()", t_gds_dlist_add_after))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_add_first()", t_gds_dlist_add_first))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_add_last()", t_gds_dlist_add_last))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_it_add_before()", t_gds_dlist_it_add_before))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_it_add_after()", t_gds_dlist_it_add_after))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_pop()", t_gds_dlist_pop))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_pop_first()", t_gds_dlist_pop_first))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_pop_last()", t_gds_dlist_pop_last))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_it_pop()", t_gds_dlist_it_pop))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_del()", t_gds_dlist_del))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_del_first()", t_gds_dlist_del_first))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_del_last()", t_gds_dlist_del_last))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_it_del()", t_gds_dlist_it_del))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_get()", t_gds_dlist_get))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_get_first()", t_gds_dlist_get_first))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_get_last()", t_gds_dlist_get_last))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_it_get()", t_gds_dlist_it_get))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_iterator_new()", t_gds_dlist_iterator_new))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_reverse_iterator_new()", t_gds_dlist_reverse_iterator_new))
	|| (NULL == CU_add_test(pSuite, "gds_dlist_chk()", t_gds_dlist_chk))
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
