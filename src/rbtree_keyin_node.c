#include <stdlib.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree_keyin_node.h"
#include "callbacks.h"

gds_rbtree_keyin_node_t * gds_rbtree_keyin_node_new(void *data)
{
	gds_rbtree_keyin_node_t *node;
	
	node = malloc(sizeof(gds_rbtree_keyin_node_t));

	if(node == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_keyin_node_t));
	}

	node->data = data;
	gds_inline_rbtree_node_init(&(node->rbtree));

	return node;
}

void * gds_rbtree_keyin_node_get_data(gds_rbtree_keyin_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->data;
}

int8_t gds_rbtree_keyin_node_set_data(gds_rbtree_keyin_node_t *node,
	void *data, gds_free_cb free_cb)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	if (free_cb != NULL) {
		free_cb(node->data);
	}
	node->data = data;

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
