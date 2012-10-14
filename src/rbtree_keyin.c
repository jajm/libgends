#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree_inline.h"
#include "rbtree_keyin_node.h"
#include "rbtree_keyin.h"
#include "callbacks.h"
#include "slist_node.h"
#include "slist.h"

#define rbt_containerof(ptr) \
	((ptr) != NULL) \
	? (gds_rbtree_keyin_node_t *)((char *)ptr - offsetof(gds_rbtree_keyin_node_t, rbtree)) \
	: NULL

typedef struct {
	gds_getkey_cb getkey_cb;
	gds_cmpkey_cb cmpkey_cb;
} gds_rbtree_keyin_callbacks_t;

int32_t gds_rbtree_keyin_node_cmp_with_key(
	gds_rbtree_inline_node_t *inode, void *key,
	gds_rbtree_keyin_callbacks_t *callbacks)
{
	gds_rbtree_keyin_node_t *node;
	int32_t cmp;

	node = rbt_containerof(inode);
	cmp = callbacks->cmpkey_cb(key, callbacks->getkey_cb(node->data));
	return cmp;
}

int32_t gds_rbtree_keyin_node_cmp(gds_rbtree_inline_node_t *inode1,
	gds_rbtree_inline_node_t *inode2,
	gds_rbtree_keyin_callbacks_t *callbacks)
{
	gds_rbtree_keyin_node_t *node1, *node2;
	int32_t cmp;

	node1 = rbt_containerof(inode1);
	node2 = rbt_containerof(inode2);

	cmp = callbacks->cmpkey_cb(callbacks->getkey_cb(node1->data),
		callbacks->getkey_cb(node2->data));

	return cmp;
}

void gds_rbtree_keyin_node_replace(gds_rbtree_inline_node_t *inode1,
	gds_rbtree_inline_node_t *inode2, gds_free_cb free_cb)
{
	gds_rbtree_keyin_node_t *node1, *node2;

	node1 = rbt_containerof(inode1);
	node2 = rbt_containerof(inode2);
	if (free_cb) {
		free_cb(node1->data);
	}
	node1->data = node2->data;
	free(node2);
}

int8_t gds_rbtree_keyin_add(gds_rbtree_keyin_node_t **root, void *data,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_alloc_cb alloc_cb)
{
	gds_rbtree_keyin_node_t *node;
	gds_rbtree_keyin_callbacks_t callbacks;
	gds_rbtree_inline_node_t *inode;
	int8_t added = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	callbacks.getkey_cb = getkey_cb;
	callbacks.cmpkey_cb = cmpkey_cb;

	if (*root == NULL) {
		*root = gds_rbtree_keyin_node_new(data, alloc_cb);
		(*root)->rbtree.red = false;
		added = 1;
	} else {
		inode = gds_rbtree_inline_get_node(&((*root)->rbtree),
			getkey_cb(data), (gds_rbt_cmp_with_key_cb)
			gds_rbtree_keyin_node_cmp_with_key, &callbacks);
		if (inode == NULL) {
			node = gds_rbtree_keyin_node_new(data, alloc_cb);
			inode = &((*root)->rbtree);
			added = gds_rbtree_inline_add(
				&inode, &(node->rbtree),
				(gds_rbt_cmp_cb)gds_rbtree_keyin_node_cmp,
				&callbacks);
			*root = rbt_containerof(inode);
		}
	}

	return added ? 0 : 1;
}

gds_rbtree_keyin_node_t * gds_rbtree_keyin_get_node(
	gds_rbtree_keyin_node_t *root, void *key, gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	gds_rbtree_keyin_callbacks_t callbacks;
	gds_rbtree_inline_node_t *inode = NULL;

	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (root == NULL) {
		return NULL;
	}

	callbacks.getkey_cb = getkey_cb;
	callbacks.cmpkey_cb = cmpkey_cb;

	inode = gds_rbtree_inline_get_node(&(root->rbtree), key,
		(gds_rbt_cmp_with_key_cb)gds_rbtree_keyin_node_cmp_with_key,
		&callbacks);

	return rbt_containerof(inode);
}

void * gds_rbtree_keyin_get(gds_rbtree_keyin_node_t *root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_alloc_cb alloc_cb)
{
	gds_rbtree_keyin_node_t *node;

	node = gds_rbtree_keyin_get_node(root, key, getkey_cb, cmpkey_cb);

	if (node == NULL) {
		return NULL;
	} else {
		return gds_rbtree_keyin_node_get_data(node, alloc_cb);
	}
}

int8_t gds_rbtree_keyin_set(gds_rbtree_keyin_node_t **root, void *data,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_free_cb free_cb,
	gds_alloc_cb alloc_cb)
{
	void *key;
	gds_rbtree_keyin_node_t *node;
	int8_t ret;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	key = getkey_cb(data);
	node = gds_rbtree_keyin_get_node(*root, key, getkey_cb, cmpkey_cb);
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
		gds_rbtree_keyin_add(root, data, getkey_cb, cmpkey_cb,
			alloc_cb);
		ret = 1;
	}

	return ret;
}

int8_t gds_rbtree_keyin_del(gds_rbtree_keyin_node_t **root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_free_cb free_cb)
{
	gds_rbtree_inline_node_t *inode;
	gds_rbtree_keyin_callbacks_t callbacks;
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
	deleted = gds_rbtree_inline_del(&inode, key,
		(gds_rbt_cmp_with_key_cb)gds_rbtree_keyin_node_cmp_with_key,
		&callbacks, (gds_rbt_replace_cb)gds_rbtree_keyin_node_replace,
		free_cb);
	*root = rbt_containerof(inode);

	return deleted ? 0 : 1;
}

void gds_rbtree_keyin_free(gds_rbtree_keyin_node_t *root,
	gds_free_cb free_cb)
{
	gds_rbtree_keyin_node_t *node;

	if (root != NULL) {
		node = rbt_containerof(root->rbtree.son[0]);
		gds_rbtree_keyin_free(node, free_cb);

		node = rbt_containerof(root->rbtree.son[1]);
		gds_rbtree_keyin_free(node, free_cb);

		gds_rbtree_keyin_node_free(root, free_cb);
	}
}

void gds_rbtree_keyin_build_values_list(gds_rbtree_keyin_node_t *root,
	gds_alloc_cb alloc_cb, gds_slist_node_t **head)
{
	if (root != NULL) {
		gds_rbtree_keyin_build_values_list(
			rbt_containerof(root->rbtree.son[1]), alloc_cb, head);
		gds_slist_add_first(head, root->data, alloc_cb);
		gds_rbtree_keyin_build_values_list(
			rbt_containerof(root->rbtree.son[0]), alloc_cb, head);
	}
}

gds_slist_node_t * gds_rbtree_keyin_values(gds_rbtree_keyin_node_t *root,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *head = NULL;

	gds_rbtree_keyin_build_values_list(root, alloc_cb, &head);

	return head;
}

