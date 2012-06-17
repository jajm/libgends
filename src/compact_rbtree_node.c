#include <stdlib.h>
#include "log.h"
#include "compact_rbtree_node.h"
#include "callbacks.h"

gds_compact_rbtree_node_t * gds_compact_rbtree_node_new(void *data,
	gds_alloc_cb alloc_cb)
{
	gds_compact_rbtree_node_t *node;
	
	node = malloc(sizeof(gds_compact_rbtree_node_t));

	if(node == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}

	if (alloc_cb != NULL) {
		node->data = alloc_cb(data);
	} else {
		node->data = data;
	}
	node->red = true;
	node->son[0] = NULL;
	node->son[1] = NULL;

	return node;
}

void * gds_compact_rbtree_node_get_data(gds_compact_rbtree_node_t *node,
	gds_alloc_cb alloc_cb)
{
	void *data;

	if (node == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	if (alloc_cb != NULL) {
		data = alloc_cb(node->data);
	} else {
		data = node->data;
	}

	return data;
}

int8_t gds_compact_rbtree_node_set_data(gds_compact_rbtree_node_t *node,
	void *data, gds_alloc_cb alloc_cb, gds_free_cb free_cb)
{
	void *d;

	if (node == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

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

bool gds_compact_rbtree_node_is_red(gds_compact_rbtree_node_t *node)
{
	return (node != NULL && node->red);
}

void gds_compact_rbtree_node_free(gds_compact_rbtree_node_t *node,
	gds_free_cb free_cb)
{
	if(node) {
		if (free_cb != NULL) {
			free_cb(node->data);
		}
		free(node);
	}
}
