#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "inline_compact_rbtree.h"
#include "kod_compact_rbtree_node.h"
#include "kod_compact_rbtree.h"
#include "callbacks.h"
#include "slist_node.h"
#include "slist.h"

#define crbt_containerof(ptr) \
	((ptr) != NULL) \
	? (gds_kod_compact_rbtree_node_t *)((char *)ptr - offsetof(gds_kod_compact_rbtree_node_t, rbtree)) \
	: NULL

int8_t gds_kod_compact_rbtree_node_cmp_with_key(
	gds_inline_compact_rbtree_node_t *inode, void *key,
	gds_cmpkey_cb cmpkey_cb)
{
	gds_kod_compact_rbtree_node_t *node;
	int8_t cmp;

	node = crbt_containerof(inode);
	cmp = cmpkey_cb(key, node->key);
	return cmp;
}

int8_t gds_kod_compact_rbtree_node_cmp(gds_inline_compact_rbtree_node_t *inode1,
	gds_inline_compact_rbtree_node_t *inode2, gds_cmpkey_cb cmpkey_cb)
{
	gds_kod_compact_rbtree_node_t *node1, *node2;
	int8_t cmp;

	node1 = crbt_containerof(inode1);
	node2 = crbt_containerof(inode2);

	cmp = cmpkey_cb(node1->key, node2->key);

	return cmp;
}

typedef struct {
	gds_free_cb key_free_cb;
	gds_free_cb free_cb;
} gds_kod_compact_rbtree_callbacks_t;

void gds_kod_compact_rbtree_node_replace(
	gds_inline_compact_rbtree_node_t *inode1,
	gds_inline_compact_rbtree_node_t *inode2,
	gds_kod_compact_rbtree_callbacks_t *callbacks)
{
	gds_kod_compact_rbtree_node_t *node1, *node2;

	node1 = crbt_containerof(inode1);
	node2 = crbt_containerof(inode2);
	if (callbacks->key_free_cb) {
		callbacks->key_free_cb(node1->key);
	}
	if (callbacks->free_cb) {
		callbacks->free_cb(node1->data);
	}
	node1->key = node2->key;
	node1->data = node2->data;
	free(node2);
}

int8_t gds_kod_compact_rbtree_add(gds_kod_compact_rbtree_node_t **root,
	void *key, void *data, gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb key_alloc_cb, gds_alloc_cb alloc_cb)
{
	gds_kod_compact_rbtree_node_t *node;
	gds_inline_compact_rbtree_node_t *inode;
	int8_t added = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (*root == NULL) {
		*root = gds_kod_compact_rbtree_node_new(key, key_alloc_cb, data,
			alloc_cb);
		(*root)->rbtree.red = false;
		added = 1;
	} else {
		inode = gds_inline_compact_rbtree_get_node(&((*root)->rbtree),
			key, (gds_crbt_cmp_with_key_cb)
			gds_kod_compact_rbtree_node_cmp_with_key, cmpkey_cb);
		if (inode == NULL) {
			node = gds_kod_compact_rbtree_node_new(key,
				key_alloc_cb,data, alloc_cb);
			inode = &((*root)->rbtree);
			added = gds_inline_compact_rbtree_add(
				&inode, &(node->rbtree), (gds_crbt_cmp_cb)
				gds_kod_compact_rbtree_node_cmp, cmpkey_cb);
			*root = crbt_containerof(inode);
		}
	}

	return added ? 0 : 1;
}

gds_kod_compact_rbtree_node_t * gds_kod_compact_rbtree_get_node(
	gds_kod_compact_rbtree_node_t *root, void *key, gds_cmpkey_cb cmpkey_cb)
{
	gds_inline_compact_rbtree_node_t *inode = NULL;

	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (root == NULL) {
		return NULL;
	}

	inode = gds_inline_compact_rbtree_get_node(&(root->rbtree), key,
		(gds_crbt_cmp_with_key_cb)
		gds_kod_compact_rbtree_node_cmp_with_key, cmpkey_cb);

	return crbt_containerof(inode);
}

void * gds_kod_compact_rbtree_get(gds_kod_compact_rbtree_node_t *root,
	void *key, gds_cmpkey_cb cmpkey_cb, gds_alloc_cb alloc_cb)
{
	gds_kod_compact_rbtree_node_t *node;

	node = gds_kod_compact_rbtree_get_node(root, key, cmpkey_cb);

	if (node == NULL) {
		return NULL;
	} else {
		return gds_kod_compact_rbtree_node_get_data(node, alloc_cb);
	}
}

int8_t gds_kod_compact_rbtree_set(gds_kod_compact_rbtree_node_t **root,
	void *key, void *data, gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb key_alloc_cb, gds_free_cb free_cb, gds_alloc_cb alloc_cb)
{
	gds_kod_compact_rbtree_node_t *node;
	int8_t ret;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	node = gds_kod_compact_rbtree_get_node(*root, key, cmpkey_cb);
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
		gds_kod_compact_rbtree_add(root, key, data, cmpkey_cb,
			key_alloc_cb, alloc_cb);
		ret = 1;
	}

	return ret;
}

int8_t gds_kod_compact_rbtree_del(gds_kod_compact_rbtree_node_t **root,
	void *key, gds_cmpkey_cb cmpkey_cb, gds_free_cb key_free_cb,
	gds_free_cb free_cb)
{
	gds_inline_compact_rbtree_node_t *inode;
	gds_kod_compact_rbtree_callbacks_t callbacks;
	int8_t deleted = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if(*root == NULL) {
		GDS_LOG_WARNING("Tree is empty");
		return 1;
	}

	callbacks.key_free_cb = key_free_cb;
	callbacks.free_cb = free_cb;

	inode = &((*root)->rbtree);
	deleted = gds_inline_compact_rbtree_del(&inode, key,
		(gds_crbt_cmp_with_key_cb)
		gds_kod_compact_rbtree_node_cmp_with_key, cmpkey_cb,
		(gds_crbt_replace_cb)gds_kod_compact_rbtree_node_replace,
		&callbacks);
	*root = crbt_containerof(inode);

	return deleted ? 0 : 1;
}

void gds_kod_compact_rbtree_free(gds_kod_compact_rbtree_node_t *root,
	gds_free_cb key_free_cb, gds_free_cb free_cb)
{
	gds_kod_compact_rbtree_node_t *node;

	if (root != NULL) {
		node = crbt_containerof(root->rbtree.son[0]);
		gds_kod_compact_rbtree_free(node, key_free_cb, free_cb);

		node = crbt_containerof(root->rbtree.son[1]);
		gds_kod_compact_rbtree_free(node, key_free_cb, free_cb);

		gds_kod_compact_rbtree_node_free(root, key_free_cb, free_cb);
	}
}

void gds_kod_compact_rbtree_build_keys_list(
	gds_kod_compact_rbtree_node_t *root, gds_alloc_cb key_alloc_cb,
	gds_slist_node_t **head)
{
	if (root != NULL) {
		gds_kod_compact_rbtree_build_keys_list(crbt_containerof(
			root->rbtree.son[1]), key_alloc_cb, head);
		*head = gds_slist_add_first(*head, root->key, key_alloc_cb);
		gds_kod_compact_rbtree_build_keys_list(crbt_containerof(
			root->rbtree.son[0]), key_alloc_cb, head);
	}
}

gds_slist_node_t * gds_kod_compact_rbtree_keys(
	gds_kod_compact_rbtree_node_t *root, gds_alloc_cb key_alloc_cb)
{
	gds_slist_node_t *head = NULL;

	gds_kod_compact_rbtree_build_keys_list(root, key_alloc_cb, &head);

	return head;
}

void gds_kod_compact_rbtree_build_values_list(
	gds_kod_compact_rbtree_node_t *root, gds_alloc_cb alloc_cb,
	gds_slist_node_t **head)
{
	if (root != NULL) {
		gds_kod_compact_rbtree_build_values_list(
			crbt_containerof(root->rbtree.son[1]), alloc_cb, head);
		*head = gds_slist_add_first(*head, root->data, alloc_cb);
		gds_kod_compact_rbtree_build_values_list(
			crbt_containerof(root->rbtree.son[0]), alloc_cb, head);
	}
}

gds_slist_node_t * gds_kod_compact_rbtree_values(
	gds_kod_compact_rbtree_node_t *root, gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *head = NULL;

	gds_kod_compact_rbtree_build_values_list(root, alloc_cb, &head);

	return head;
}

gds_kod_compact_rbtree_list_node_t * gds_kod_compact_rbtree_list_node_new(
	void *key, void *data, gds_alloc_cb key_alloc_cb, gds_alloc_cb alloc_cb)
{
	gds_kod_compact_rbtree_list_node_t *list_node;

	list_node = malloc(sizeof(gds_kod_compact_rbtree_list_node_t));
	if (list_node == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_kod_compact_rbtree_list_node_t));
	}

	if (key_alloc_cb) {
		list_node->key = key_alloc_cb(key);
	} else {
		list_node->key = key;
	}
	if (alloc_cb) {
		list_node->data = alloc_cb(data);
	} else {
		list_node->data = data;
	}

	return list_node;
}

void gds_kod_compact_rbtree_build_nodes_list(
	gds_kod_compact_rbtree_node_t *root, gds_alloc_cb key_alloc_cb,
	gds_alloc_cb alloc_cb, gds_slist_node_t **head)
{
	gds_kod_compact_rbtree_list_node_t *list_node;
	if (root != NULL) {
		gds_kod_compact_rbtree_build_nodes_list(
			crbt_containerof(root->rbtree.son[1]), key_alloc_cb,
			alloc_cb, head);

		list_node = gds_kod_compact_rbtree_list_node_new(root->key,
			root->data, key_alloc_cb, alloc_cb);
		*head = gds_slist_add_first(*head, list_node, alloc_cb);

		gds_kod_compact_rbtree_build_nodes_list(
			crbt_containerof(root->rbtree.son[0]), key_alloc_cb,
			alloc_cb, head);
	}
}

gds_slist_node_t * gds_kod_compact_rbtree_nodes(
	gds_kod_compact_rbtree_node_t *root, gds_alloc_cb key_alloc_cb,
	gds_alloc_cb alloc_cb)
{
	gds_slist_node_t *head = NULL;

	gds_kod_compact_rbtree_build_nodes_list(root, key_alloc_cb, alloc_cb,
		&head);

	return head;
}

