#include <stdlib.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree_node.h"
#include "callbacks.h"

gds_rbtree_node_t * gds_rbtree_node_new(void *key, void *data)
{
	gds_rbtree_node_t *node;
	
	node = malloc(sizeof(gds_rbtree_node_t));

	if(node == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_node_t));
	}

	node->key = key;
	node->data = data;

	gds_rbtree_inline_node_init(&(node->rbtree));

	return node;
}

void * gds_rbtree_node_get_key(gds_rbtree_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->key;
}

void gds_rbtree_node_set_key(gds_rbtree_node_t *node, void *key,
	gds_free_cb key_free_cb)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	if (key_free_cb != NULL) {
		key_free_cb(node->key);
	}
	node->key = key;
}

void * gds_rbtree_node_get_data(gds_rbtree_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->data;
}

void gds_rbtree_node_set_data(gds_rbtree_node_t *node, void *data,
	gds_free_cb free_cb)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	if (free_cb != NULL) {
		free_cb(node->data);
	}
	node->data = data;
}

void gds_rbtree_node_free(gds_rbtree_node_t *node,
	gds_free_cb key_free_cb, gds_free_cb free_cb)
{
	if(node) {
		if (key_free_cb != NULL) {
			key_free_cb(node->key);
		}
		if (free_cb != NULL) {
			free_cb(node->data);
		}
		free(node);
	}
}
