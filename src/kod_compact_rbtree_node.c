#include <stdlib.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "kod_compact_rbtree_node.h"
#include "callbacks.h"

gds_kod_compact_rbtree_node_t * gds_kod_compact_rbtree_node_new(void *key,
	gds_alloc_cb key_alloc_cb, void *data, gds_alloc_cb alloc_cb)
{
	gds_kod_compact_rbtree_node_t *node;
	
	node = malloc(sizeof(gds_kod_compact_rbtree_node_t));

	if(node == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_kod_compact_rbtree_node_t));
	}

	if (key_alloc_cb != NULL) {
		node->key = key_alloc_cb(key);
	} else {
		node->key = key;
	}

	if (alloc_cb != NULL) {
		node->data = alloc_cb(data);
	} else {
		node->data = data;
	}

	gds_inline_compact_rbtree_node_init(&(node->rbtree));

	return node;
}

void * gds_kod_compact_rbtree_node_get_key(gds_kod_compact_rbtree_node_t *node,
	gds_alloc_cb key_alloc_cb)
{
	void *key;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (key_alloc_cb != NULL) {
		key = key_alloc_cb(node->key);
	} else {
		key = node->key;
	}

	return key;
}

int8_t gds_kod_compact_rbtree_node_set_key(gds_kod_compact_rbtree_node_t *node,
	void *key, gds_alloc_cb key_alloc_cb, gds_free_cb key_free_cb)
{
	void *d;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (key_alloc_cb != NULL) {
		d = key_alloc_cb(key);
	} else {
		d = key;
	}

	if (key_free_cb != NULL) {
		key_free_cb(node->key);
	}
	node->key = d;

	return 0;
}

void * gds_kod_compact_rbtree_node_get_data(gds_kod_compact_rbtree_node_t *node,
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

int8_t gds_kod_compact_rbtree_node_set_data(gds_kod_compact_rbtree_node_t *node,
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

void gds_kod_compact_rbtree_node_free(gds_kod_compact_rbtree_node_t *node,
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
