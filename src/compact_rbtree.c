#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "inline_compact_rbtree.h"
#include "compact_rbtree_node.h"
#include "compact_rbtree.h"
#include "callbacks.h"
#include "slist_node.h"
#include "slist.h"

#define crbt_containerof(ptr) \
	((ptr) != NULL) \
	? (gds_compact_rbtree_node_t *)((char *)ptr - offsetof(gds_compact_rbtree_node_t, rbtree)) \
	: NULL

typedef struct {
	gds_getkey_cb getkey_cb;
	gds_cmpkey_cb cmpkey_cb;
} gds_compact_rbtree_callbacks_t;

int32_t gds_compact_rbtree_node_cmp_with_key(
	gds_inline_compact_rbtree_node_t *inode, void *key,
	gds_compact_rbtree_callbacks_t *callbacks)
{
	gds_compact_rbtree_node_t *node;
	int32_t cmp;

	node = crbt_containerof(inode);
	cmp = callbacks->cmpkey_cb(key, callbacks->getkey_cb(node->data));
	return cmp;
}

int32_t gds_compact_rbtree_node_cmp(gds_inline_compact_rbtree_node_t *inode1,
	gds_inline_compact_rbtree_node_t *inode2,
	gds_compact_rbtree_callbacks_t *callbacks)
{
	gds_compact_rbtree_node_t *node1, *node2;
	int32_t cmp;

	node1 = crbt_containerof(inode1);
	node2 = crbt_containerof(inode2);

	cmp = callbacks->cmpkey_cb(callbacks->getkey_cb(node1->data),
		callbacks->getkey_cb(node2->data));

	return cmp;
}

void gds_compact_rbtree_node_replace(gds_inline_compact_rbtree_node_t *inode1,
	gds_inline_compact_rbtree_node_t *inode2, gds_free_cb free_cb)
{
	gds_compact_rbtree_node_t *node1, *node2;

	node1 = crbt_containerof(inode1);
	node2 = crbt_containerof(inode2);
	if (free_cb) {
		free_cb(node1->data);
	}
	node1->data = node2->data;
	free(node2);
}

int8_t gds_compact_rbtree_add(gds_compact_rbtree_node_t **root, void *data,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_alloc_cb alloc_cb)
{
	gds_compact_rbtree_node_t *node;
	gds_compact_rbtree_callbacks_t callbacks;
	gds_inline_compact_rbtree_node_t *inode;
	int8_t added = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	callbacks.getkey_cb = getkey_cb;
	callbacks.cmpkey_cb = cmpkey_cb;

	if (*root == NULL) {
		*root = gds_compact_rbtree_node_new(data, alloc_cb);
		(*root)->rbtree.red = false;
		added = 1;
	} else {
		inode = gds_inline_compact_rbtree_get_node(&((*root)->rbtree),
			getkey_cb(data), (gds_crbt_cmp_with_key_cb)
			gds_compact_rbtree_node_cmp_with_key, &callbacks);
		if (inode == NULL) {
			node = gds_compact_rbtree_node_new(data, alloc_cb);
			inode = &((*root)->rbtree);
			added = gds_inline_compact_rbtree_add(
				&inode, &(node->rbtree),
				(gds_crbt_cmp_cb)gds_compact_rbtree_node_cmp,
				&callbacks);
			*root = crbt_containerof(inode);
		}
	}

	return added ? 0 : 1;
}

gds_compact_rbtree_node_t * gds_compact_rbtree_get_node(
	gds_compact_rbtree_node_t *root, void *key, gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	gds_compact_rbtree_callbacks_t callbacks;
	gds_inline_compact_rbtree_node_t *inode = NULL;

	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (root == NULL) {
		return NULL;
	}

	callbacks.getkey_cb = getkey_cb;
	callbacks.cmpkey_cb = cmpkey_cb;

	inode = gds_inline_compact_rbtree_get_node(&(root->rbtree), key,
		(gds_crbt_cmp_with_key_cb)gds_compact_rbtree_node_cmp_with_key,
		&callbacks);

	return crbt_containerof(inode);
}

void * gds_compact_rbtree_get(gds_compact_rbtree_node_t *root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_alloc_cb alloc_cb)
{
	gds_compact_rbtree_node_t *node;

	node = gds_compact_rbtree_get_node(root, key, getkey_cb, cmpkey_cb);

	if (node == NULL) {
		return NULL;
	} else {
		return gds_compact_rbtree_node_get_data(node, alloc_cb);
	}
}

int8_t gds_compact_rbtree_set(gds_compact_rbtree_node_t **root, void *data,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_free_cb free_cb,
	gds_alloc_cb alloc_cb)
{
	void *key;
	gds_compact_rbtree_node_t *node;
	int8_t ret;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	key = getkey_cb(data);
	node = gds_compact_rbtree_get_node(*root, key, getkey_cb, cmpkey_cb);
	if (node != NULL) {
		/* Node found: the key already exists in the tree */
		if (free_cb != NULL) {
			free_cb(node->data);
		}
		if (alloc_cb != NULL) {
			node->data = alloc_cb(data);
		} else {
			node->data = data;
		}
		ret = 0;
	} else {
		/* Node not found: add a new node */
		gds_compact_rbtree_add(root, data, getkey_cb, cmpkey_cb,
			alloc_cb);
		ret = 1;
	}

	return ret;
}

int8_t gds_compact_rbtree_del(gds_compact_rbtree_node_t **root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_free_cb free_cb)
{
	gds_inline_compact_rbtree_node_t *inode;
	gds_compact_rbtree_callbacks_t callbacks;
	int8_t deleted = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if(*root == NULL) {
		GDS_LOG_WARNING("Tree is empty");
		return 1;
	}

	callbacks.getkey_cb = getkey_cb;
	callbacks.cmpkey_cb = cmpkey_cb;

	inode = &((*root)->rbtree);
	deleted = gds_inline_compact_rbtree_del(&inode, key,
		(gds_crbt_cmp_with_key_cb)gds_compact_rbtree_node_cmp_with_key,
		&callbacks, (gds_crbt_replace_cb)gds_compact_rbtree_node_replace,
		free_cb);
	*root = crbt_containerof(inode);

	return deleted ? 0 : 1;
}

void gds_compact_rbtree_free(gds_compact_rbtree_node_t *root,
	gds_free_cb free_cb)
{
	gds_compact_rbtree_node_t *node;

	if (root != NULL) {
		node = crbt_containerof(root->rbtree.son[0]);
		gds_compact_rbtree_free(node, free_cb);

		node = crbt_containerof(root->rbtree.son[1]);
		gds_compact_rbtree_free(node, free_cb);

		gds_compact_rbtree_node_free(root, free_cb);
	}
}

void gds_compact_rbtree_build_values_list(gds_compact_rbtree_node_t *root,
	gds_alloc_cb alloc_cb, gds_slist_node_t **head)
{
	if (root != NULL) {
		gds_compact_rbtree_build_values_list(
			crbt_containerof(root->rbtree.son[1]), alloc_cb, head);
		*head = gds_slist_add_first(*head, root->data, alloc_cb);
		gds_compact_rbtree_build_values_list(
			crbt_containerof(root->rbtree.son[0]), alloc_cb, head);
	}
}

gds_slist_node_t * gds_compact_rbtree_values(gds_compact_rbtree_node_t *root,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *head = NULL;

	gds_compact_rbtree_build_values_list(root, alloc_cb, &head);

	return head;
}

