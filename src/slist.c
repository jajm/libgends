#include <stdint.h>
#include <stdlib.h>
#include "exception.h"
#include "check_arg.h"
#include "callbacks.h"
#include "log.h"
#include "slist_node.h"
#include "slist.h"

gds_slist_node_t * gds_slist_add_first(gds_slist_node_t **head, void *data,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *n;

	GDS_CHECK_ARG_NOT_NULL(head);

	n = gds_slist_node_new(data, alloc_cb);
	n->next = *head;
	*head = n;

	return n;
}

gds_slist_node_t * gds_slist_add_last(gds_slist_node_t **head, void *data,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *n, *tmp;

	GDS_CHECK_ARG_NOT_NULL(head);

	n = gds_slist_node_new(data, alloc_cb);

	if (*head != NULL) {
		tmp = *head;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = n;
	} else {
		*head = n;
	}

	return n;
}

gds_slist_node_t * gds_slist_add_after(gds_slist_node_t *node, void *data,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *n;

	n = gds_slist_node_new(data, alloc_cb);
		
	if (node != NULL) {
		n->next = node->next;
		node->next = n;
	}

	return n;
}

void gds_slist_add_list_first(gds_slist_node_t **head,
	gds_slist_node_t *list)
{
	gds_slist_node_t *node;
	GDS_CHECK_ARG_NOT_NULL(head);
	GDS_CHECK_ARG_NOT_NULL(list);

	node = list;
	while (node->next != NULL) {
		node = node->next;
	}
	node->next = *head;
	*head = list;
}

void gds_slist_add_list_last(gds_slist_node_t **head,
	gds_slist_node_t *list)
{
	gds_slist_node_t *node;
	GDS_CHECK_ARG_NOT_NULL(head);
	GDS_CHECK_ARG_NOT_NULL(list);

	if (*head == NULL) {
		*head = list;
	} else {
		node = *head;
		while (node->next != NULL) {
			node = node->next;
		}
		node->next = list;
	}
}

void gds_slist_add_list_after(gds_slist_node_t *node,
	gds_slist_node_t *list)
{
	gds_slist_node_t *n;
	GDS_CHECK_ARG_NOT_NULL(node);
	GDS_CHECK_ARG_NOT_NULL(list);

	n = list;
	while (n->next != NULL) {
		n = n->next;
	}
	n->next = node->next;
	node->next = list;
}

gds_slist_node_t * gds_slist_get_last(gds_slist_node_t *head)
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

void gds_slist_del_first(gds_slist_node_t **head, gds_free_cb free_cb)
{
	gds_slist_node_t *n = NULL;

	GDS_CHECK_ARG_NOT_NULL(head);

	if (*head != NULL) {
		n = (*head)->next;
		gds_slist_node_free(*head, free_cb);
		*head = n;
	}
}

void gds_slist_del_last(gds_slist_node_t **head, gds_free_cb free_cb)
{
	gds_slist_node_t *tmp, *prev = NULL;

	GDS_CHECK_ARG_NOT_NULL(head);

	if (*head != NULL) {
		tmp = *head;
		while (tmp->next != NULL) {
			prev = tmp;
			tmp = tmp->next;
		}
		gds_slist_node_free(tmp, free_cb);
		if (prev != NULL) {
			prev->next = NULL;
		} else {
			*head = NULL;
		}
	}
}

int8_t gds_slist_del_after(gds_slist_node_t *node, gds_free_cb free_cb)
{
	gds_slist_node_t *n;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (node->next == NULL) {
		GDS_LOG_WARNING("node is the last node");
		return -1;
	}

	n = node->next->next;
	gds_slist_node_free(node->next, free_cb);
	node->next = n;

	return 0;
}

void gds_slist_free(gds_slist_node_t *head, gds_free_cb free_cb)
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
} gds_slist_iterator_data_t;

int8_t gds_slist_iterator_reset(gds_slist_iterator_data_t *it_data)
{
	GDS_CHECK_ARG_NOT_NULL(it_data);

	it_data->cur = NULL;

	return 0;
}

int8_t gds_slist_iterator_step(gds_slist_iterator_data_t *it_data)
{
	gds_slist_node_t *next;

	GDS_CHECK_ARG_NOT_NULL(it_data);

	next = (it_data->cur != NULL) ? it_data->cur->next : it_data->head;
	if (next == NULL) {
		return 1;
	}

	it_data->cur = next;

	return 0;
}

void * gds_slist_iterator_get(gds_slist_iterator_data_t *it_data)
{
	GDS_CHECK_ARG_NOT_NULL(it_data);

	return gds_slist_node_get_data(it_data->cur, NULL);
}

gds_iterator_t * gds_slist_iterator_new(gds_slist_node_t *head)
{
	gds_slist_iterator_data_t *it_data;
	gds_iterator_t *it;

	it_data = malloc(sizeof(gds_slist_iterator_data_t));
	if (it_data == NULL) {
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_slist_iterator_data_t));
	}

	it_data->head = head;
	it_data->cur = NULL;

	it = gds_iterator_new(it_data,
		(gds_iterator_reset_cb) &gds_slist_iterator_reset,
		(gds_iterator_step_cb) &gds_slist_iterator_step,
		(gds_iterator_get_cb) &gds_slist_iterator_get,
		(gds_free_cb) &free);
	
	return it;
}
