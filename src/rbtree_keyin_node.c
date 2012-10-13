#include <stdlib.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree_keyin_node.h"
#include "callbacks.h"

gds_rbtree_keyin_node_t * gds_rbtree_keyin_node_new(void *data,
	gds_alloc_cb alloc_cb)
{
	gds_rbtree_keyin_node_t *node;
	
	node = malloc(sizeof(gds_rbtree_keyin_node_t));

	if(node == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_keyin_node_t));
	}

	if (alloc_cb != NULL) {
		node->data = alloc_cb(data);
	} else {
		node->data = data;
	}
	gds_rbtree_inline_node_init(&(node->rbtree));

	return node;
}

void * gds_rbtree_keyin_node_get_data(gds_rbtree_keyin_node_t *node,
	gds_alloc_cb alloc_cb)
{
	void *data;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (alloc_cb != NULL) {
		data = alloc_cb(node->data);
	} else {
		data = node->data;
	}

	return data;
}

int8_t gds_rbtree_keyin_node_set_data(gds_rbtree_keyin_node_t *node,
	void *data, gds_alloc_cb alloc_cb, gds_free_cb free_cb)
{
	void *d;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (alloc_cb != NULL) {
		d = alloc_cb(data);
	} else {
		d = data;
	}

	if (free_cb != NULL) {
		free_cb(node->data);
	}
	node->data = d;

	return 0;
}

void gds_rbtree_keyin_node_free(gds_rbtree_keyin_node_t *node,
	gds_free_cb free_cb)
{
	if(node) {
		if (free_cb != NULL) {
			free_cb(node->data);
		}
		free(node);
	}
}
