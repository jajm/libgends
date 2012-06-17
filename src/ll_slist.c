#include <stdint.h>
#include <stdlib.h>
#include "callbacks.h"
#include "log.h"
#include "slist_node.h"
#include "ll_slist.h"

gds_slist_node_t * gds_ll_slist_add_first(gds_slist_node_t *head, void *data,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *n;

	n = gds_slist_node_new(data, alloc_cb);
	if (n == NULL) {
		GDS_LOG_ERROR("Failed to create gds_slist_node_t");
		return NULL;
	}

	n->next = head;

	return n;
}

gds_slist_node_t * gds_ll_slist_add_last(gds_slist_node_t *head, void *data,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *n, *tmp;

	n = gds_slist_node_new(data, alloc_cb);
	if (n == NULL) {
		GDS_LOG_ERROR("Failed to create gds_slist_node_t");
		return NULL;
	}

	if (head != NULL) {
		tmp = head;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = n;
	}

	return n;
}

gds_slist_node_t * gds_ll_slist_add_after(gds_slist_node_t *node, void *data,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *n;

	n = gds_slist_node_new(data, alloc_cb);
	if (n == NULL) {
		GDS_LOG_ERROR("Failed to create gds_slist_node_t");
		return NULL;
	}
		
	if (node != NULL) {
		n->next = node->next;
		node->next = n;
	}

	return n;
}

gds_slist_node_t * gds_ll_slist_get_last(gds_slist_node_t *head)
{
	gds_slist_node_t *tmp = NULL;

	if (head != NULL) {
		tmp = head;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
	}

	return tmp;
}

gds_slist_node_t * gds_ll_slist_del_first(gds_slist_node_t *head,
	gds_free_cb free_cb)
{
	gds_slist_node_t *n = NULL;

	if (head != NULL) {
		n = head->next;
		gds_slist_node_free(head, free_cb);
	}

	return n;
}

gds_slist_node_t * gds_ll_slist_del_last(gds_slist_node_t *head,
	gds_free_cb free_cb)
{
	gds_slist_node_t *tmp, *prev = NULL;

	if (head != NULL) {
		tmp = head;
		while (tmp->next != NULL) {
			prev = tmp;
			tmp = tmp->next;
		}
		gds_slist_node_free(tmp, free_cb);
		if (prev != NULL) {
			prev->next = NULL;
		} else {
			head = NULL;
		}
	}

	return head;
}

int8_t gds_ll_slist_del_after(gds_slist_node_t *node, gds_free_cb free_cb)
{
	gds_slist_node_t *n;

	if (node == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	if (node->next == NULL) {
		GDS_LOG_WARNING("node is the last node");
		return 1;
	}

	n = node->next->next;
	gds_slist_node_free(node->next, free_cb);
	node->next = n;

	return 0;
}

void gds_ll_slist_free(gds_slist_node_t *head, gds_free_cb free_cb)
{
	gds_slist_node_t *tmp, *tmp2;

	tmp = head;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		gds_slist_node_free(tmp, free_cb);
		tmp = tmp2;
	}
}

typedef struct {
	gds_slist_node_t *head;
	gds_slist_node_t *cur;
} gds_ll_slist_iterator_data_t;

int8_t gds_ll_slist_iterator_reset(gds_ll_slist_iterator_data_t *it_data)
{
	if (it_data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}
	it_data->cur = NULL;

	return 0;
}

int8_t gds_ll_slist_iterator_step(gds_ll_slist_iterator_data_t *it_data)
{
	gds_slist_node_t *next;

	if (it_data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return -1;
	}

	next = (it_data->cur != NULL) ? it_data->cur->next : it_data->head;
	if (next == NULL) {
		return 1;
	}

	it_data->cur = next;

	return 0;
}

void * gds_ll_slist_iterator_get(gds_ll_slist_iterator_data_t *it_data)
{
	if (it_data == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	return gds_slist_node_get_data(it_data->cur, NULL);
}

gds_iterator_t * gds_ll_slist_iterator_new(gds_slist_node_t *head)
{
	gds_ll_slist_iterator_data_t *it_data;
	gds_iterator_t *it;

	it_data = malloc(sizeof(gds_ll_slist_iterator_data_t));
	if (it_data == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}

	it_data->head = head;
	it_data->cur = NULL;

	it = gds_iterator_new(it_data,
		(gds_iterator_reset_cb) &gds_ll_slist_iterator_reset,
		(gds_iterator_step_cb) &gds_ll_slist_iterator_step,
		(gds_iterator_get_cb) &gds_ll_slist_iterator_get);
	
	return it;
}
