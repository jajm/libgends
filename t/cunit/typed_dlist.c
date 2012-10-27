#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "exception.h"
#include "test_macros.h"
#include "types.h"
#include "typed_dlist.h"

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

void t_gds_typed_dlist_new(void)
{
	gds_typed_dlist_t *l;

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_new(NULL));
	CU_ASSERT_PTR_NOT_NULL((l = gds_typed_dlist_new("")));
	gds_typed_dlist_free(l, false);
	CU_ASSERT_PTR_NOT_NULL((l = gds_typed_dlist_new("type")));
	gds_typed_dlist_free(l, false);
}

void t_gds_typed_dlist_empty(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };

	gds_type_register(type_name);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	CU_ASSERT(true == gds_typed_dlist_empty(NULL));
	CU_ASSERT(true == gds_typed_dlist_empty(l));

	CU_ASSERT_FATAL(NULL != gds_typed_dlist_add_first(l, &test, false));
	CU_ASSERT(false == gds_typed_dlist_empty(l));
	gds_typed_dlist_free(l, false);

	gds_type_unregister(type_name);
}


void t_gds_typed_dlist_first(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node;

	gds_type_register(type_name);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_first(NULL));
	CU_ASSERT_PTR_NULL(gds_typed_dlist_first(l));

	node = gds_typed_dlist_add_first(l, &test, false);
	CU_ASSERT_FATAL(node != NULL);
	CU_ASSERT_PTR_NOT_NULL(gds_typed_dlist_first(l));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	gds_typed_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_last(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node;

	gds_type_register(type_name);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_last(NULL));
	CU_ASSERT_PTR_NULL(gds_typed_dlist_last(l));

	node = gds_typed_dlist_add_last(l, &test, false);
	CU_ASSERT_FATAL(node != NULL);
	CU_ASSERT_PTR_NOT_NULL(gds_typed_dlist_last(l));
	CU_ASSERT(node == gds_typed_dlist_last(l));

	gds_typed_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_next(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_next(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_next(l, NULL));
	
	node = gds_typed_dlist_add_first(l, &test, false);
	CU_ASSERT_FATAL(node != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_next(NULL, node));
	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node));

	node2 = gds_typed_dlist_add_first(l, &test, false);
	CU_ASSERT_FATAL(node2 != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_next(NULL, node));
	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_next(NULL, node2));
	CU_ASSERT(node == gds_typed_dlist_next(l, node2));

	gds_typed_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_prev(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_prev(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_prev(l, NULL));
	
	node = gds_typed_dlist_add_last(l, &test, false);
	CU_ASSERT_FATAL(node != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_prev(NULL, node));
	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));

	node2 = gds_typed_dlist_add_last(l, &test, false);
	CU_ASSERT_FATAL(node2 != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_prev(NULL, node));
	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_prev(NULL, node2));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node2));

	gds_typed_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_has_next(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_next(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_next(l, NULL));

	node = gds_typed_dlist_add_first(l, &test, false);
	CU_ASSERT_FATAL(node != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_next(NULL, node));
	CU_ASSERT(false == gds_typed_dlist_has_next(l, node));

	node2 = gds_typed_dlist_add_first(l, &test, false);
	CU_ASSERT_FATAL(node2 != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_next(NULL, node));
	CU_ASSERT(false == gds_typed_dlist_has_next(l, node));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_next(NULL, node2));
	CU_ASSERT(true == gds_typed_dlist_has_next(l, node2));

	gds_typed_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_has_prev(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_prev(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_prev(l, NULL));

	node = gds_typed_dlist_add_last(l, &test, false);
	CU_ASSERT_FATAL(node != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_prev(NULL, node));
	CU_ASSERT(false == gds_typed_dlist_has_prev(l, node));

	node2 = gds_typed_dlist_add_last(l, &test, false);
	CU_ASSERT_FATAL(node2 != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_prev(NULL, node));
	CU_ASSERT(false == gds_typed_dlist_has_prev(l, node));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_has_prev(NULL, node2));
	CU_ASSERT(true == gds_typed_dlist_has_prev(l, node2));

	gds_typed_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_add_before(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3, *node4;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(NULL, NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(NULL, NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(NULL, NULL, &test, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(NULL, NULL, &test, true));

	node = gds_dlist_node_new(&test, NULL);
	CU_ASSERT_FATAL(node != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(NULL, node, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(NULL, node, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(NULL, node, &test, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(NULL, node, &test, true));
	gds_dlist_node_free(node, NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(l, NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(l, NULL, NULL, true));
	CU_ASSERT_PTR_NOT_NULL((node4 = gds_typed_dlist_add_before(l, NULL, test_ptr, false)));
	CU_ASSERT_PTR_NOT_NULL((node2 = gds_typed_dlist_add_before(l, NULL, &test, true)));

	test_ptr = calloc(1, sizeof(test_structure_t));

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(l, node, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_before(l, node, NULL, true));
	CU_ASSERT_PTR_NOT_NULL((node3 = gds_typed_dlist_add_before(l, node4, test_ptr, false)));
	CU_ASSERT_PTR_NOT_NULL((node = gds_typed_dlist_add_before(l, node2, &test, true)));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node4));
	CU_ASSERT(node4 == gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_prev(l, node4));
	CU_ASSERT(node4 == gds_typed_dlist_last(l));

	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_add_after(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3, *node4;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(NULL, NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(NULL, NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(NULL, NULL, &test, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(NULL, NULL, &test, true));

	node = gds_dlist_node_new(&test, NULL);
	CU_ASSERT_FATAL(node != NULL);
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(NULL, node, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(NULL, node, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(NULL, node, &test, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(NULL, node, &test, true));
	gds_dlist_node_free(node, NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(l, NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(l, NULL, NULL, true));
	CU_ASSERT_PTR_NOT_NULL((node = gds_typed_dlist_add_after(l, NULL, test_ptr, false)));
	CU_ASSERT_PTR_NOT_NULL((node3 = gds_typed_dlist_add_after(l, NULL, &test, true)));

	test_ptr = calloc(1, sizeof(test_structure_t));

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(l, node, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_after(l, node, NULL, true));
	CU_ASSERT_PTR_NOT_NULL((node4 = gds_typed_dlist_add_after(l, node3, test_ptr, false)));
	CU_ASSERT_PTR_NOT_NULL((node2 = gds_typed_dlist_add_after(l, node, &test, true)));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node4));
	CU_ASSERT(node4 == gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_prev(l, node4));
	CU_ASSERT(node4 == gds_typed_dlist_last(l));

	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_add_first(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_first(NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_first(NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_first(NULL, &test, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_first(NULL, &test, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_first(l, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_first(l, NULL, true));
	CU_ASSERT_PTR_NOT_NULL((node2 = gds_typed_dlist_add_first(l, test_ptr, false)));
	CU_ASSERT_PTR_NOT_NULL((node = gds_typed_dlist_add_first(l, &test, true)));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_last(l));

	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_add_last(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_last(NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_last(NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_last(NULL, &test, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_last(NULL, &test, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_last(l, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_add_last(l, NULL, true));
	CU_ASSERT_PTR_NOT_NULL((node = gds_typed_dlist_add_last(l, test_ptr, false)));
	CU_ASSERT_PTR_NOT_NULL((node2 = gds_typed_dlist_add_last(l, &test, true)));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_last(l));

	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_it_add_before(void)
{
	gds_typed_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3, *node4;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);
	it = gds_typed_dlist_iterator_new(l);
	CU_ASSERT_FATAL(it != NULL);

	node4 = gds_typed_dlist_add_first(l, &test, true);
	node2 = gds_typed_dlist_add_first(l, &test, true);
	gds_iterator_reset(it);
	gds_iterator_step(it);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_before(NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_before(NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_before(NULL, &test, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_before(NULL, &test, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_before(it, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_before(it, NULL, true));
	CU_ASSERT_PTR_NOT_NULL((node = gds_typed_dlist_it_add_before(it, test_ptr, false)));
	gds_iterator_step(it);
	CU_ASSERT_PTR_NOT_NULL((node3 = gds_typed_dlist_it_add_before(it, &test, true)));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node4));
	CU_ASSERT(node4 == gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_prev(l, node4));
	CU_ASSERT(node4 == gds_typed_dlist_last(l));

	gds_iterator_free(it);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_it_add_after(void)
{
	gds_typed_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3, *node4;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);
	it = gds_typed_dlist_iterator_new(l);
	CU_ASSERT_FATAL(it != NULL);

	node4 = gds_typed_dlist_add_first(l, &test, true);
	node = gds_typed_dlist_add_first(l, &test, true);
	gds_iterator_reset(it);
	gds_iterator_step(it);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_after(NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_after(NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_after(NULL, &test, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_after(NULL, &test, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_after(it, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_add_after(it, NULL, true));
	CU_ASSERT_PTR_NOT_NULL((node2 = gds_typed_dlist_it_add_after(it, test_ptr, false)));
	gds_iterator_step(it);
	CU_ASSERT_PTR_NOT_NULL((node3 = gds_typed_dlist_it_add_after(it, &test, true)));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node4));
	CU_ASSERT(node4 == gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_prev(l, node4));
	CU_ASSERT(node4 == gds_typed_dlist_last(l));

	gds_iterator_free(it);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_pop(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;
	void *data;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, true)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);
	CU_ASSERT_FATAL((node3 = gds_typed_dlist_add_last(l, test_ptr, true)) != NULL);

	CU_ASSERT_FATAL(NULL == gds_typed_dlist_next(l, node3));
	CU_ASSERT_FATAL(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT_FATAL(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT_FATAL(node == gds_typed_dlist_first(l));

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_pop(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_pop(NULL, node));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_pop(l, NULL));

	data = gds_typed_dlist_pop(l, node2);
	CU_ASSERT_PTR_NOT_NULL(data)
	CU_ASSERT(0 == test_cmp(data, &test));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_last(l));

	test_free((test_structure_t *)data);
	test_free(test_ptr);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_pop_first(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;
	void *data;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_pop_first(NULL));
	GDS_ASSERT_THROW(EmptyListException, gds_typed_dlist_pop_first(l));

	CU_ASSERT_PTR_NOT_NULL((node = gds_typed_dlist_add_last(l, &test, true)));
	CU_ASSERT_PTR_NOT_NULL((node2 = gds_typed_dlist_add_last(l, test_ptr, true)));
	CU_ASSERT_PTR_NOT_NULL((node3 = gds_typed_dlist_add_last(l, test_ptr, true)));

	CU_ASSERT(NULL == gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	data = (test_structure_t *)gds_typed_dlist_pop_first(l);
	CU_ASSERT_PTR_NOT_NULL(data);
	CU_ASSERT(0 == test_cmp(data, &test));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_last(l));
	test_free((test_structure_t *)data);

	test_free(test_ptr);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_pop_last(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;
	void *data;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_pop_last(NULL));
	GDS_ASSERT_THROW(EmptyListException, gds_typed_dlist_pop_last(l));

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, true)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, test_ptr, true)) != NULL);
	CU_ASSERT_FATAL((node3 = gds_typed_dlist_add_last(l, &test, true)) != NULL);

	CU_ASSERT_FATAL(NULL == gds_typed_dlist_next(l, node3));
	CU_ASSERT_FATAL(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT_FATAL(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT_FATAL(node == gds_typed_dlist_first(l));

	data = gds_typed_dlist_pop_last(l);
	CU_ASSERT_PTR_NOT_NULL(data)
	CU_ASSERT(0 == test_cmp(data, &test));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_last(l));

	test_free((test_structure_t *)data);
	test_free(test_ptr);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_it_pop(void)
{
	gds_typed_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;
	void *data;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);
	it = gds_typed_dlist_iterator_new(l);
	CU_ASSERT_FATAL(it != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_pop(NULL));
	GDS_ASSERT_THROW(InvalidIteratorException, gds_typed_dlist_it_pop(it));

	CU_ASSERT_PTR_NOT_NULL((node = gds_typed_dlist_add_last(l, test_ptr, true)));
	CU_ASSERT_PTR_NOT_NULL((node2 = gds_typed_dlist_add_last(l, &test, true)));
	CU_ASSERT_PTR_NOT_NULL((node3 = gds_typed_dlist_add_last(l, test_ptr, true)));

	CU_ASSERT(NULL == gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT(0 == gds_iterator_reset(it));
	CU_ASSERT(0 == gds_iterator_step(it));
	CU_ASSERT(0 == gds_iterator_step(it));

	data = gds_typed_dlist_it_pop(it);
	CU_ASSERT_PTR_NOT_NULL(data)
	CU_ASSERT(0 == test_cmp(data, &test));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_last(l));

	test_free((test_structure_t *)data);

	gds_iterator_free(it);
	test_free(test_ptr);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_del(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, false)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);
	CU_ASSERT_FATAL((node3 = gds_typed_dlist_add_last(l, test_ptr, true)) != NULL);

	CU_ASSERT_FATAL(NULL == gds_typed_dlist_next(l, node3));
	CU_ASSERT_FATAL(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT_FATAL(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT_FATAL(node == gds_typed_dlist_first(l));

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del(NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del(NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del(NULL, node, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del(NULL, node, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del(l, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del(l, NULL, true));
	CU_ASSERT(0 == gds_typed_dlist_del(l, node, false));
	CU_ASSERT(0 == gds_typed_dlist_del(l, node2, true));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_last(l));

	test_free(test_ptr);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_del_first(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, false)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);
	CU_ASSERT_FATAL((node3 = gds_typed_dlist_add_last(l, test_ptr, true)) != NULL);

	CU_ASSERT_FATAL(NULL == gds_typed_dlist_next(l, node3));
	CU_ASSERT_FATAL(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT_FATAL(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT_FATAL(node == gds_typed_dlist_first(l));

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del_first(NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del_first(NULL, true));
	CU_ASSERT(0 == gds_typed_dlist_del_first(l, false));
	CU_ASSERT(0 == gds_typed_dlist_del_first(l, true));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_last(l));

	test_free(test_ptr);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_del_last(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, true)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);
	CU_ASSERT_FATAL((node3 = gds_typed_dlist_add_last(l, test_ptr, false)) != NULL);

	CU_ASSERT_FATAL(NULL == gds_typed_dlist_next(l, node3));
	CU_ASSERT_FATAL(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT_FATAL(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT_FATAL(node == gds_typed_dlist_first(l));

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del_last(NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_del_last(NULL, true));
	CU_ASSERT(0 == gds_typed_dlist_del_last(l, false));
	CU_ASSERT(0 == gds_typed_dlist_del_last(l, true));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node));
	CU_ASSERT(node == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node));
	CU_ASSERT(node == gds_typed_dlist_last(l));

	test_free(test_ptr);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_it_del(void)
{
	gds_typed_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2, *node3;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);
	it = gds_typed_dlist_iterator_new(l);
	CU_ASSERT_FATAL(it != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_del(NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_del(NULL, true));
	GDS_ASSERT_THROW(InvalidIteratorException, gds_typed_dlist_it_del(it, false));
	GDS_ASSERT_THROW(InvalidIteratorException, gds_typed_dlist_it_del(it, true));

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, false)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);
	CU_ASSERT_FATAL((node3 = gds_typed_dlist_add_last(l, test_ptr, true)) != NULL);

	CU_ASSERT_FATAL(NULL == gds_typed_dlist_next(l, node3));
	CU_ASSERT_FATAL(node3 == gds_typed_dlist_next(l, node2));
	CU_ASSERT_FATAL(node2 == gds_typed_dlist_next(l, node));
	CU_ASSERT_FATAL(node == gds_typed_dlist_first(l));

	gds_iterator_reset(it);
	GDS_ASSERT_THROW(InvalidIteratorException, gds_typed_dlist_it_del(it, false));
	GDS_ASSERT_THROW(InvalidIteratorException, gds_typed_dlist_it_del(it, true));
	gds_iterator_step(it);
	CU_ASSERT(0 == gds_typed_dlist_it_del(it, false));
	CU_ASSERT(0 == gds_typed_dlist_it_del(it, true));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_next(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_first(l));

	CU_ASSERT_PTR_NULL(gds_typed_dlist_prev(l, node3));
	CU_ASSERT(node3 == gds_typed_dlist_last(l));

	gds_iterator_free(it);
	test_free(test_ptr);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_get(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	void *data = NULL, *data2 = NULL;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, false)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get(NULL, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get(NULL, NULL, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get(NULL, node, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get(NULL, node, true));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get(l, NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get(l, NULL, true));
	CU_ASSERT_PTR_NOT_NULL((data = gds_typed_dlist_get(l, node, false)));
	CU_ASSERT_PTR_NOT_NULL((data2 = gds_typed_dlist_get(l, node, true)));

	CU_ASSERT(data == test_ptr);
	CU_ASSERT(data != data2);
	CU_ASSERT(0 == test_cmp(data, data2));

	gds_typed_dlist_del(l, node, false);
	test_free((test_structure_t *)data2);
	test_free(test_ptr);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_get_first(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	void *data, *data2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get_first(NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get_first(NULL, true));
	GDS_ASSERT_THROW(EmptyListException, gds_typed_dlist_get_first(l, false));
	GDS_ASSERT_THROW(EmptyListException, gds_typed_dlist_get_first(l, true));

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, false)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);

	CU_ASSERT_PTR_NOT_NULL((data = gds_typed_dlist_get_first(l, false)));
	CU_ASSERT_PTR_NOT_NULL((data2 = gds_typed_dlist_get_first(l, true)));

	CU_ASSERT(data == test_ptr);
	CU_ASSERT(data != data2);
	CU_ASSERT(0 == test_cmp(data, data2));

	test_free((test_structure_t *)data2);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_get_last(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	void *data, *data2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get_last(NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_get_last(NULL, true));
	GDS_ASSERT_THROW(EmptyListException, gds_typed_dlist_get_last(l, false));
	GDS_ASSERT_THROW(EmptyListException, gds_typed_dlist_get_last(l, true));

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, false)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);

	CU_ASSERT_PTR_NOT_NULL((data = gds_typed_dlist_get_last(l, false)));
	CU_ASSERT_PTR_NOT_NULL((data2 = gds_typed_dlist_get_last(l, true)));

	CU_ASSERT(data != test_ptr);
	CU_ASSERT(data != data2);
	CU_ASSERT(0 == test_cmp(data, data2));

	test_free((test_structure_t *)data2);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_it_get(void)
{
	gds_typed_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	void *data, *data2;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);
	it = gds_typed_dlist_iterator_new(l);
	CU_ASSERT_FATAL(it != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_get(NULL, false));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_it_get(NULL, true));
	GDS_ASSERT_THROW(EmptyListException, gds_typed_dlist_it_get(it, false));
	GDS_ASSERT_THROW(EmptyListException, gds_typed_dlist_it_get(it, true));

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, false)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);

	gds_iterator_reset(it);
	gds_iterator_step(it);
	CU_ASSERT_PTR_NOT_NULL((data = gds_typed_dlist_it_get(it, false)));
	CU_ASSERT_PTR_NOT_NULL((data2 = gds_typed_dlist_it_get(it, true)));

	CU_ASSERT(data == test_ptr);
	CU_ASSERT(data != data2);
	if(data != NULL && data2 != NULL) {
		CU_ASSERT(0 == test_cmp(data, data2));
	}

	gds_iterator_free(it);
	test_free((test_structure_t *)data2);
	gds_typed_dlist_free(l, true);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_iterator_new(void)
{
	gds_typed_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };

	gds_type_register(type_name);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_iterator_new(NULL));
	CU_ASSERT_PTR_NOT_NULL((it = gds_typed_dlist_iterator_new(l)));
	CU_ASSERT(0 == gds_iterator_reset(it));
	CU_ASSERT(0 < gds_iterator_step(it));
	GDS_ASSERT_THROW(InvalidIteratorException, gds_iterator_get(it));

	gds_typed_dlist_add_last(l, &test, false);
	CU_ASSERT(0 == gds_iterator_step(it));
	CU_ASSERT(&test == gds_iterator_get(it));

	CU_ASSERT(0 < gds_iterator_step(it));

	gds_iterator_free(it);
	gds_typed_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_reverse_iterator_new(void)
{
	gds_typed_dlist_t *l;
	gds_iterator_t *it;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };

	gds_type_register(type_name);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_reverse_iterator_new(NULL));
	CU_ASSERT_PTR_NOT_NULL((it = gds_typed_dlist_reverse_iterator_new(l)));
	CU_ASSERT(0 == gds_iterator_reset(it));
	CU_ASSERT(0 < gds_iterator_step(it));
	GDS_ASSERT_THROW(InvalidIteratorException, gds_iterator_get(it));

	gds_typed_dlist_add_last(l, &test, false);
	CU_ASSERT(0 == gds_iterator_step(it));
	CU_ASSERT(&test == gds_iterator_get(it));

	CU_ASSERT(0 < gds_iterator_step(it));

	gds_iterator_free(it);
	gds_typed_dlist_free(l, false);
	gds_type_unregister(type_name);
}

void t_gds_typed_dlist_chk(void)
{
	gds_typed_dlist_t *l;
	char *type_name = "type_name";
	test_structure_t test = { .i = 1, .c = '1', .f = 1.0 };
	test_structure_t *test_ptr = calloc(1, sizeof(test_structure_t));
	gds_dlist_node_t *node, *node2;
	gds_func_ptr_t cmp_f = (gds_func_ptr_t)&test_cmp;

	gds_type_register(type_name);
	gds_type_register_func(type_name, "free", (gds_func_ptr_t)&test_free);
	gds_type_register_func(type_name, "alloc", (gds_func_ptr_t)&test_alloc);
	gds_type_register_func(type_name, "cmp", cmp_f);

	l = gds_typed_dlist_new(type_name);
	CU_ASSERT_FATAL(l != NULL);

	CU_ASSERT_FATAL((node = gds_typed_dlist_add_last(l, test_ptr, false)) != NULL);
	CU_ASSERT_FATAL((node2 = gds_typed_dlist_add_last(l, &test, true)) != NULL);

	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_chk(NULL, NULL));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_chk(NULL, &test));
	GDS_ASSERT_THROW(BadArgumentException, gds_typed_dlist_chk(l, NULL));
	if(cmp_f(&test, &test) == 0) {
		CU_ASSERT(node2 == gds_typed_dlist_chk(l, &test));
	} else {
		CU_ASSERT_PTR_NULL(gds_typed_dlist_chk(l, &test));
	}

	gds_typed_dlist_free(l, true);
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
	   (NULL == CU_add_test(pSuite, "gds_typed_dlist_new()", t_gds_typed_dlist_new))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_empty()", t_gds_typed_dlist_empty))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_first()", t_gds_typed_dlist_first))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_last()", t_gds_typed_dlist_last))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_next()", t_gds_typed_dlist_next))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_prev()", t_gds_typed_dlist_prev))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_has_next()", t_gds_typed_dlist_has_next))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_has_prev()", t_gds_typed_dlist_has_prev))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_add_before()", t_gds_typed_dlist_add_before))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_add_after()", t_gds_typed_dlist_add_after))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_add_first()", t_gds_typed_dlist_add_first))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_add_last()", t_gds_typed_dlist_add_last))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_it_add_before()", t_gds_typed_dlist_it_add_before))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_it_add_after()", t_gds_typed_dlist_it_add_after))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_pop()", t_gds_typed_dlist_pop))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_pop_first()", t_gds_typed_dlist_pop_first))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_pop_last()", t_gds_typed_dlist_pop_last))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_it_pop()", t_gds_typed_dlist_it_pop))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_del()", t_gds_typed_dlist_del))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_del_first()", t_gds_typed_dlist_del_first))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_del_last()", t_gds_typed_dlist_del_last))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_it_del()", t_gds_typed_dlist_it_del))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_get()", t_gds_typed_dlist_get))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_get_first()", t_gds_typed_dlist_get_first))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_get_last()", t_gds_typed_dlist_get_last))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_it_get()", t_gds_typed_dlist_it_get))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_iterator_new()", t_gds_typed_dlist_iterator_new))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_reverse_iterator_new()", t_gds_typed_dlist_reverse_iterator_new))
	|| (NULL == CU_add_test(pSuite, "gds_typed_dlist_chk()", t_gds_typed_dlist_chk))
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	try {
		CU_basic_run_tests();
	} catch() as (e) {
		fprintf(stderr, "\nTests returned an unexpected exception\n");
		fprintf(stderr, "\tType: %s\n", e->type());
		fprintf(stderr, "\tMessage: %s\n", e->message());
		fprintf(stderr, "\tFile: %s\n", e->filename());
		fprintf(stderr, "\tFunction: %s\n", e->function());
		fprintf(stderr, "\tLine: %d\n", e->line());
		return EXIT_FAILURE;
	}
	CU_cleanup_registry();
	return CU_get_error();
}
