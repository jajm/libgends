#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree_keyin_fast_node.h"
#include "callbacks.h"

gds_rbtree_keyin_fast_node_t * gds_rbtree_keyin_fast_node_new(void *data)
{
	gds_rbtree_keyin_fast_node_t *n;

	n = malloc(sizeof(gds_rbtree_keyin_fast_node_t));
	if(n == NULL) {
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_rbtree_keyin_fast_node_t));
	}
	
	n->data = data;

	gds_inline_rbtree_fast_node_init(&(n->rbtree));
	
	return n;
}

int8_t gds_rbtree_keyin_fast_node_set_data(gds_rbtree_keyin_fast_node_t *node,
	void *data, gds_free_cb free_cb)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	if (free_cb != NULL) {
		free_cb(node->data);
	}
	node->data = data;

	return 0;
}

void * gds_rbtree_keyin_fast_node_get_data(gds_rbtree_keyin_fast_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->data;
}

void gds_rbtree_keyin_fast_node_free(gds_rbtree_keyin_fast_node_t *node,
	gds_free_cb free_cb)
{
	if(node) {
		if(free_cb != NULL) {
			free_cb(node->data);
		}
		free(node);
	}
}
