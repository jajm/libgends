#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "log.h"
#include "rbtree_node.h"
#include "callbacks.h"

gds_rbtree_node_t * gds_rbtree_node_new(void *data, gds_alloc_cb alloc_cb)
{
	gds_rbtree_node_t *n;

	n = malloc(sizeof(gds_rbtree_node_t));
	if(n == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	
	if (alloc_cb != NULL) {
		n->data = alloc_cb(data);
	} else {
		n->data = data;
	}

	n->red = true;
	n->parent = n->left = n->right = NULL;
	
	return n;
}

bool gds_rbtree_node_is_red(gds_rbtree_node_t *node)
{
	return (node != NULL && node->red == true);
}

int8_t gds_rbtree_node_set_data(gds_rbtree_node_t *node, void *data,
	gds_alloc_cb alloc_cb, gds_free_cb free_cb)
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

void * gds_rbtree_node_get_data(gds_rbtree_node_t *node, gds_alloc_cb alloc_cb)
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

void gds_rbtree_node_free(gds_rbtree_node_t *node, gds_free_cb free_cb)
{
	if(node) {
		if(free_cb != NULL) {
			free_cb(node->data);
		}
		free(node);
	}
}
